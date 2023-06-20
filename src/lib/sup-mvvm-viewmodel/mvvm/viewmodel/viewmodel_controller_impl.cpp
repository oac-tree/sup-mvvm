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

#include <stdexcept>

namespace mvvm
{

ViewModelControllerImpl::ViewModelControllerImpl(SessionModelInterface *model,
                                                 ViewModelBase *view_model)
    : m_model(model), m_view_model(view_model)
{
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
    auto next_parent_view = ProcessItem(new_child, parent_view, insert_view_index);
    if (next_parent_view)
    {
      Iterate(new_child, next_parent_view);
    }
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

void ViewModelControllerImpl::InitViewModel()
{
  CheckInitialState();
  m_view_item_map.Clear();
  m_view_item_map.Update(GetRootItem(), m_view_model->rootItem());
  Iterate(GetRootItem(), m_view_model->rootItem());
}

void ViewModelControllerImpl::SetRootSessionItemIntern(SessionItem *item)
{
  SessionItem *root_item = item ? item : m_model->GetRootItem();
  m_root_item_path = utils::PathFromItem(item);

  if (root_item->GetModel() != m_model)
  {
    throw std::runtime_error("Error: atttemp to use item from alien model as new root.");
  }

  m_view_model->ResetRootViewItem(CreateRootViewItem(root_item), /*notify*/ false);
}

}  // namespace mvvm
