/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewmodel_controller_virtual_parent_impl.h"

#include "viewitem_factory.h"
#include "viewmodel_utils.h"

#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionitem.h>

#include <stack>

namespace mvvm
{

ViewModelControllerVirtualParentImpl::ViewModelControllerVirtualParentImpl(
    ViewModelBase *viewmodel, std::unique_ptr<ChildrenStrategyInterface> children_strategy,
    std::unique_ptr<RowStrategyInterface> row_strategy)
    : m_viewmodel(viewmodel)
    , m_children_strategy(std::move(children_strategy))
    , m_row_strategy(std::move(row_strategy))
{
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const AboutToInsertItemEvent &event)
{
  // nothing to do
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const ItemInsertedEvent &event)
{
  const auto added_item = event.m_item->GetItem(event.m_tag_index);

  std::stack<SessionItem *> stack{{added_item}};
  while (!stack.empty())
  {
    const auto item = stack.top();
    stack.pop();

    const auto [parent_view, insert_view_index] = GetParentViewAndIndex(item);
    if (parent_view != nullptr)
    {
      m_viewmodel->insertRow(parent_view, insert_view_index, CreateTreeOfRows(*item));
      continue;
    }

    // No parent wants to show this child
    // Add its children to the stack as they may be visible under a different parent
    auto children = item->GetAllItems();
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push(*it);
    }
  }
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  const auto item_to_remove = event.m_item->GetItem(event.m_tag_index);

  if (item_to_remove == GetRootItem() || utils::IsItemAncestor(GetRootItem(), item_to_remove))
  {
    // special case when user removes SessionItem which is one of ancestors of our root item
    // or root item itself
    m_view_item_map.Clear();
    m_viewmodel->ResetRootViewItem(CreateRootViewItem(nullptr));
    return;
  }

  const auto remove_item_func = [this](SessionItem *const item)
  {
    const auto view = m_view_item_map.FindView(item);
    if (view)
    {
      m_viewmodel->removeRow(view->GetParent(), view->Row());
      m_view_item_map.OnItemRemove(item);
    }
  };
  // Remove views for all children of the item being removed
  utils::iterate(item_to_remove, remove_item_func);
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const ItemRemovedEvent &event)
{
  // nothing to do
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const DataChangedEvent &event)
{
  for (auto view : utils::FindViewsForItem(m_viewmodel, event.m_item))
  {
    if (auto roles = utils::GetQtRoles(view, event.m_data_role); !roles.empty())
    {
      auto index = m_viewmodel->indexFromItem(view);
      emit m_viewmodel->dataChanged(index, index, roles);
    }
  }
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const ModelAboutToBeResetEvent &event)
{
  (void)event;
  // Here we are notified that the model content will be destroyed soon.
  // To let all views looking at ViewModelBase to perform necessary bookkeeping we have to
  // emit internal QAbstractViewModel::beginResetModel already now, while `model` content is still
  // alive.
  m_viewmodel->BeginResetModelNotify();
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const ModelResetEvent &event)
{
  m_view_item_map.Clear();
  auto root_view_item = std::move(CreateTreeOfRows(*event.m_model->GetRootItem(), true).at(0));
  m_viewmodel->ResetRootViewItem(std::move(root_view_item), /*notify*/ false);
  m_viewmodel->EndResetModelNotify();  //  BeginResetModel was already called
}

void ViewModelControllerVirtualParentImpl::OnModelEvent(const ModelAboutToBeDestroyedEvent &event)
{
  (void)event;
  m_viewmodel->ResetRootViewItem(CreateRootViewItem(nullptr));
}

const SessionItem *ViewModelControllerVirtualParentImpl::GetRootItem() const
{
  return utils::GetItemFromView<SessionItem>(m_viewmodel->rootItem());
}

void ViewModelControllerVirtualParentImpl::SetRootItem(SessionItem *root_item)
{
  CheckInitialState();

  if (root_item)
  {
    m_view_item_map.Clear();
    auto root_view_item = std::move(CreateTreeOfRows(*root_item, true).at(0));
    m_viewmodel->ResetRootViewItem(std::move(root_view_item));
  }
  else
  {
    m_view_item_map.Clear();
    m_viewmodel->ResetRootViewItem(CreateRootViewItem(nullptr));
  }
}

int ViewModelControllerVirtualParentImpl::GetColumnCount() const
{
  return m_row_strategy->GetSize();
}

QStringList ViewModelControllerVirtualParentImpl::GetHorizontalHeaderLabels() const
{
  return m_row_strategy->GetHorizontalHeaderLabels();
}

void ViewModelControllerVirtualParentImpl::CheckInitialState() const
{
  if (!m_viewmodel)
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

int ViewModelControllerVirtualParentImpl::GetInsertViewIndexOfChild(
    const SessionItem *const parent, const SessionItem *const child) const
{
  // children that should get their views
  auto children = m_children_strategy->GetChildren(parent);

  return utils::IndexOfItem(children, child);
}

std::tuple<ViewItem *, int> ViewModelControllerVirtualParentImpl::GetParentViewAndIndex(
    const SessionItem *const item) const
{
  const auto root_item = GetRootItem();

  auto parent{item};
  int insert_view_index{-1};
  while (insert_view_index == -1 && parent != nullptr && parent != root_item)
  {
    // Handle items with virtual/hidden parents
    // If the parent doesn't return index of the child, go up the tree until we find one that does
    parent = parent->GetParent();
    insert_view_index = GetInsertViewIndexOfChild(parent, item);
  }

  return insert_view_index != -1
             ? std::make_tuple(m_view_item_map.FindView(parent), insert_view_index)
             : std::make_tuple(nullptr, -1);
}

std::vector<std::unique_ptr<ViewItem> > ViewModelControllerVirtualParentImpl::CreateTreeOfRows(
    SessionItem &item, const bool is_root)
{
  // Every SessionItem will be represented by the vector of ViewItem's.  The first element of this
  // vector plays the role of parent view for SessionItem's children. So it might contain another
  // ViewItem vectors.

  // A helper structure to visit SessionItem hierarchy in non iterative manner.
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
    row_of_views = std::move(m_row_strategy->ConstructRow(&item));
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

  return row_of_views;
}

ViewItemMap &ViewModelControllerVirtualParentImpl::GetViewItemMap()
{
  return m_view_item_map;
}

}  // namespace mvvm
