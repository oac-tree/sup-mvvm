/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewmodel_controller_impl.h"

#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <iostream>
#include <stack>
#include <stdexcept>

namespace mvvm
{

ViewModelControllerImpl::ViewModelControllerImpl(SessionModelInterface *model,
                                                 ViewModelBase *view_model)
    : m_model(model), m_view_model(view_model)
{
}

void ViewModelControllerImpl::SetChildrenStrategy(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy)
{
  m_children_strategy = std::move(children_strategy);
}

void ViewModelControllerImpl::SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy)
{
  m_row_strategy = std::move(row_strategy);
}

void ViewModelControllerImpl::CheckInitialState() const
{
  if (!m_model)
  {
    throw std::runtime_error("Error in ViewModewlController: model is absent");
  }

  if (!m_view_model)
  {
    throw std::runtime_error("Error in ViewModewlController: viewmodel is absent");
  }

  if (!m_children_strategy)
  {
    throw std::runtime_error("Error in ViewModewlController: children strategy is not defined");
  }

  if (!m_row_strategy)
  {
    throw std::runtime_error("Error in ViewModewlController: row strategy is not defined");
  }
}

const SessionItem *ViewModelControllerImpl::GetRootItem() const
{
  return utils::GetItemFromView<SessionItem>(m_view_model->rootItem());
}

void ViewModelControllerImpl::Iterate(const SessionItem *item, ViewItem *parent_view)
{
  for (auto *child : m_children_strategy->GetChildren(item))
  {
    auto *next_parent_view = ProcessItem(child, parent_view, parent_view->rowCount());
    if (next_parent_view)
    {
      Iterate(child, next_parent_view);
    }
  }
}

ViewItem *ViewModelControllerImpl::ProcessItem(SessionItem *item, ViewItem *parent_view, int row)
{
  auto row_of_views = m_row_strategy->ConstructRow(item);
  if (!row_of_views.empty())
  {
    auto *next_parent_view = row_of_views.at(0).get();
    if (m_mute_notify)
    {
      // to avoid emiting signals rowsAboutToBeInserted, rowsInserted (used when full rebuild)
      parent_view->insertRow(row, std::move(row_of_views));
    }
    else
    {
      m_view_model->insertRow(parent_view, row, std::move(row_of_views));
    }
    m_view_item_map.Update(item, next_parent_view);
    return next_parent_view;
  }
  return nullptr;
}

int ViewModelControllerImpl::GetInsertViewIndexOfChild(const SessionItem *parent,
                                                       const SessionItem *child)
{
  // children that should get their views
  auto children = m_children_strategy->GetChildren(parent);

  return utils::IndexOfItem(children, child);
}

void ViewModelControllerImpl::InsertView(SessionItem *parent, const TagIndex &tag_index)
{
  auto new_child = parent->GetItem(tag_index);
  int insert_view_index = GetInsertViewIndexOfChild(parent, new_child);
  if (insert_view_index == -1)
  {
    return;
  }

  if (auto parent_view = m_view_item_map.FindView(parent); parent_view)
  {
    m_view_model->insertRow(parent_view, insert_view_index, CreateRow(*new_child));
  }
}

void ViewModelControllerImpl::RemoveRowOfViews(SessionItem *item)
{
  if (auto view = m_view_item_map.FindView(item); view)
  {
    m_view_model->removeRow(view->parent(), view->row());
    m_view_item_map.Remove(item);
  }
}

void ViewModelControllerImpl::Init(SessionItem *root_item)
{
  CheckInitialState();

  SessionItem *root_item2 = root_item ? root_item : m_model->GetRootItem();
  m_root_item_path = utils::PathFromItem(root_item);
  if (root_item2->GetModel() != m_model)
  {
    throw std::runtime_error("Error: atttemp to use item from alien model as new root.");
  }

  m_view_item_map.Clear();
  m_view_model->ResetRootViewItem(std::move(CreateRow(*root_item2, true).at(0)));
}

void ViewModelControllerImpl::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  auto item_to_remove = event.m_item->GetItem(event.m_tag_index);

  if (item_to_remove == GetRootItem() || utils::IsItemAncestor(GetRootItem(), item_to_remove))
  {
    // special case when user removes SessionItem which is one of ancestors of our root item
    // or root item itself
    m_root_item_path = {};
    m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
  }
  else
  {
    RemoveRowOfViews(item_to_remove);
  }
}

void ViewModelControllerImpl::OnModelEvent(const DataChangedEvent &event)
{
  for (auto view : utils::FindViewsForItem(m_view_model, event.m_item))
  {
    if (isValidItemRole(view, event.m_data_role))
    {
      auto index = m_view_model->indexFromItem(view);
      emit m_view_model->dataChanged(index, index, utils::ItemRoleToQtRole(event.m_data_role));
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
  auto custom_root_item = utils::ItemFromPath(*event.m_model, m_root_item_path);
  m_mute_notify = true;

  SessionItem *root_item = custom_root_item ? custom_root_item : m_model->GetRootItem();
  m_root_item_path = utils::PathFromItem(custom_root_item);

  if (root_item->GetModel() != m_model)
  {
    throw std::runtime_error("Error: atttemp to use item from alien model as new root.");
  }

  m_view_item_map.Clear();
  m_view_model->ResetRootViewItem(std::move(CreateRow(*root_item, true).at(0)), /*notify*/ false);
  m_view_model->EndResetModelNotify();  //  BeginResetModel was already called
}

void ViewModelControllerImpl::OnModelEvent(const ModelAboutToBeDestroyedEvent &event)
{
  (void)event;
  m_root_item_path = {};
  m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
}

QStringList ViewModelControllerImpl::GetHorizontalHeaderLabels() const
{
  return m_row_strategy->GetHorizontalHeaderLabels();
}

std::vector<std::unique_ptr<ViewItem> > ViewModelControllerImpl::CreateRow(SessionItem &item,
                                                                           bool is_root)
{
  struct Node
  {
    SessionItem *item{nullptr};
    ViewItem *view_item{nullptr};
  };
  std::stack<Node> stack;

  std::vector<std::unique_ptr<ViewItem> > row_of_views;
  if (is_root)
  {
    row_of_views.push_back(CreateRootViewItem(&item));
  }
  else
  {
    row_of_views = std::move(m_row_strategy->ConstructRow(&item));
  }

  if (!row_of_views.empty())
  {
    auto *next_parent_view = row_of_views.at(0).get();
    stack.push({&item, next_parent_view});
  }

  while (!stack.empty())
  {
    auto *current_parent = stack.top().item;
    auto *current_parent_view = stack.top().view_item;

    stack.pop();

    m_view_item_map.Update(current_parent, current_parent_view);

    auto children = m_children_strategy->GetChildren(current_parent);

    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      auto row = m_row_strategy->ConstructRow(*it);

      if (!row.empty())
      {
        auto *next_parent_view = row.at(0).get();
        //        int insert_view_index = GetInsertViewIndexOfChild(current_parent, *it);
        //        std::cout << "XXX " << (*it)->GetDisplayName() << " " << insert_view_index << " "
        //        << (children.size() - 1 - insert_view_index) << std::endl;;
        current_parent_view->insertRow(0, std::move(row));
        stack.push({*it, next_parent_view});
      }
    }
  }

  return row_of_views;
}

ViewItemMap &ViewModelControllerImpl::GetViewItemMap()
{
  return m_view_item_map;
}

//! Returns true if given SessionItem role is valid for view

bool ViewModelControllerImpl::isValidItemRole(const ViewItem *view, int item_role)
{
  if (auto presentation = dynamic_cast<const mvvm::SessionItemPresentation *>(view->item());
      presentation)
  {
    if (presentation->GetDataRole() == item_role)
    {
      return true;
    }
  }

  return item_role == mvvm::DataRole::kAppearance || item_role == mvvm::DataRole::kTooltip;
}

}  // namespace mvvm
