/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewmodel_controller_impl.h"

#include "viewitem_factory.h"
#include "viewmodel_utils.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/i_children_strategy.h>
#include <mvvm/viewmodel/i_row_strategy.h>

#include <stack>

namespace mvvm
{

ViewModelControllerImpl::ViewModelControllerImpl(
    ViewModelBase *viewmodel, std::unique_ptr<IChildrenStrategy> children_strategy,
    std::unique_ptr<IRowStrategy> row_strategy)
    : m_view_model(viewmodel)
    , m_children_strategy(std::move(children_strategy))
    , m_row_strategy(std::move(row_strategy))
{
}

ViewModelControllerImpl::~ViewModelControllerImpl() = default;

void ViewModelControllerImpl::OnModelEvent(const AboutToInsertItemEvent &event)
{
  (void)event;
  // nothing to do
}

void ViewModelControllerImpl::OnModelEvent(const ItemInsertedEvent &event)
{
  auto [parent, tag_index] = event;

  auto new_child = parent->GetItem(tag_index);

  const int insert_view_index = GetInsertViewIndexOfChild(parent, new_child);
  auto parent_view = m_view_item_map.FindView(parent);

  if (insert_view_index != -1 && parent_view)
  {
    m_view_model->insertRow(parent_view, insert_view_index, CreateTreeOfRows(*new_child));
  }
}

void ViewModelControllerImpl::OnModelEvent(const ItemRemovedEvent &event)
{
  (void)event;
  // nothing to do
}

void ViewModelControllerImpl::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  auto item_to_remove = event.item->GetItem(event.tag_index);

  if (item_to_remove == GetRootItem() || utils::IsItemAncestor(GetRootItem(), item_to_remove))
  {
    // special case when user removes SessionItem which is one of ancestors of our root item
    // or root item itself
    m_view_item_map.Clear();
    m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
    return;
  }

  if (auto view = m_view_item_map.FindView(item_to_remove); view)
  {
    m_view_model->removeRow(view->GetParent(), view->Row());
    m_view_item_map.OnItemRemove(item_to_remove);
  }
}

void ViewModelControllerImpl::OnModelEvent(const DataChangedEvent &event)
{
  for (auto view : utils::FindViewsForItem(m_view_model, event.item))
  {
    if (auto roles = utils::GetQtRoles(view, event.data_role); !roles.empty())
    {
      auto index = m_view_model->indexFromItem(view);
      emit m_view_model->dataChanged(index, index, roles);
    }
  }
}

void ViewModelControllerImpl::OnModelEvent(const ModelAboutToBeResetEvent &event)
{
  (void)event;
  // Here we are notified that the model content will be destroyed soon.
  // To let all views looking at ViewModelBase to perform necessary bookkeeping we have to
  // emit internal QAbstractViewModel::beginResetModel already now, while `model` content is still
  // alive.
  m_view_model->BeginResetModelNotify();
}

void ViewModelControllerImpl::OnModelEvent(const ModelResetEvent &event)
{
  SessionItem *root_item = event.model->GetRootItem();

  m_view_item_map.Clear();
  auto root_view_item = std::move(CreateTreeOfRows(*root_item, true).at(0));
  m_view_model->ResetRootViewItem(std::move(root_view_item), /*notify*/ false);

  m_view_model->EndResetModelNotify();  //  BeginResetModel was already called
}

void ViewModelControllerImpl::OnModelEvent(const ModelAboutToBeDestroyedEvent &event)
{
  (void)event;
  m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
}

const SessionItem *ViewModelControllerImpl::GetRootItem() const
{
  return utils::GetItemFromView<SessionItem>(m_view_model->rootItem());
}

void ViewModelControllerImpl::SetRootItem(SessionItem *root_item)
{
  CheckInitialState();

  if (root_item)
  {
    m_view_item_map.Clear();
    auto root_view_item = std::move(CreateTreeOfRows(*root_item, true).at(0));
    m_view_model->ResetRootViewItem(std::move(root_view_item));
  }
  else
  {
    m_view_item_map.Clear();
    m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
  }
}

int ViewModelControllerImpl::GetColumnCount() const
{
  return m_row_strategy->GetSize();
}

QStringList ViewModelControllerImpl::GetHorizontalHeaderLabels() const
{
  return m_row_strategy->GetHorizontalHeaderLabels();
}

void ViewModelControllerImpl::CheckInitialState() const
{
  if (!m_view_model)
  {
    throw RuntimeException("Error in ViewModewlController: viewmodel is absent");
  }

  if (!m_children_strategy)
  {
    throw RuntimeException("Error in ViewModewlController: children strategy is not defined");
  }

  if (!m_row_strategy)
  {
    throw RuntimeException("Error in ViewModewlController: row strategy is not defined");
  }
}

int ViewModelControllerImpl::GetInsertViewIndexOfChild(const SessionItem *parent,
                                                       const SessionItem *child)
{
  // children that should get their views
  auto children = m_children_strategy->GetChildren(parent);

  return utils::IndexOfItem(children, child);
}

std::vector<std::unique_ptr<ViewItem> > ViewModelControllerImpl::CreateTreeOfRows(SessionItem &item,
                                                                                  bool is_root)
{
  // Every SessionItem will be represented by the vector of ViewItem's.  The first element of this
  // vector plays the role of parent view for SessionItem's children. So it might contain another
  // ViewItem vectors.

  // A helper structure to visit SessionItem hierarchy in non recursive manner.
  struct Node
  {
    SessionItem *item{nullptr};    // a SessionItem being visited
    ViewItem *view_item{nullptr};  // first ViewItem in a row of item's views
  };

  std::stack<Node> stack;

  std::vector<std::unique_ptr<ViewItem> > row_of_views;
  if (is_root)
  {
    // If the item is marked as root, where is no need to call any strategy to find the row.
    // There can be only one root item an it will be invisible for trees and tables.
    row_of_views.push_back(CreateRootViewItem(&item));
  }
  else
  {
    row_of_views = m_row_strategy->ConstructRow(&item);
  }

  if (!row_of_views.empty())
  {
    auto *view_item = row_of_views.at(0).get();  // a parent for the following row of views
    stack.push({&item, view_item});
  }

  while (!stack.empty())
  {
    auto *current_parent = stack.top().item;
    auto *current_parent_view = stack.top().view_item;
    stack.pop();

    m_view_item_map.Insert(current_parent, current_parent_view);

    auto children = m_children_strategy->GetChildren(current_parent);

    // visited in reverse order to populate the stack properly
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      auto row = m_row_strategy->ConstructRow(*it);

      if (!row.empty())
      {
        auto *next_parent_view = row.at(0).get();

        // Inserting row of views into their parent. We always insert at index 0 to compensate
        // reverse order.
        current_parent_view->InsertRow(0, std::move(row));

        stack.push({*it, next_parent_view});
      }
    }
  }

  // Do not allow to generate empty rows.
  if (row_of_views.empty())
  {
    throw RuntimeException("ViewModelControllerImpl: empty row was generated by the strategy");
  }

  return row_of_views;
}

ViewItemMap &ViewModelControllerImpl::GetViewItemMap()
{
  return m_view_item_map;
}

}  // namespace mvvm
