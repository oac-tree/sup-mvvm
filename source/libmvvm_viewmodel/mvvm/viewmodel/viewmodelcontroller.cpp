/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewmodelcontroller.h"

#include "mvvm/interfaces/childrenstrategyinterface.h"
#include "mvvm/interfaces/rowstrategyinterface.h"
#include "mvvm/model/itemutils.h"
#include "mvvm/model/modelutils.h"
#include "mvvm/model/path.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/viewmodel/viewitemfactory.h"
#include "mvvm/viewmodel/viewitemmap.h"
#include "mvvm/viewmodel/viewmodelutils.h"
#include "mvvm/viewmodelbase/viewmodelbase.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"
#include "mvvm/viewmodel/viewitemfactory.h"
#include "mvvm/viewmodel/standardpresentationitems.h"

#include <QDebug>
#include <map>
#include <stdexcept>

namespace
{
//! Returns true if given SessionItem role is valid for view
bool isValidItemRole(const mvvm::ViewItem *view, int item_role)
{

  if (auto presentation = dynamic_cast<const mvvm::SessionItemPresentation*>(view->item());
      presentation)
  {
    if (presentation->GetDataRole() == item_role)
    {
      return true;
    }
  }

  return item_role == mvvm::DataRole::kAppearance || item_role == mvvm::DataRole::kTooltip;
}

}  // namespace

namespace mvvm
{
struct ViewModelController::ViewModelControllerImpl
{
  SessionModel *m_model{nullptr};
  ViewModelBase *m_view_model{nullptr};
  ViewItemMap m_view_item_map;
  std::unique_ptr<ChildrenStrategyInterface> m_children_strategy;
  std::unique_ptr<RowStrategyInterface> m_row_strategy;
  bool m_mute_notify{false};  // allows to build ViewModel without notification
  Path m_root_item_path;      // saves path to custom root item, to restore it on model reset

  ViewModelControllerImpl(SessionModel *model, ViewModelBase *view_model)
      : m_model(model), m_view_model(view_model)
  {
  }

  void CheckInitialState() const
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

  const SessionItem *GetRootItem() const
  {
    return utils::GetItemFromView<SessionItem>(m_view_model->rootItem());
  }

  void Iterate(const SessionItem *item, ViewItem *parent_view)
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

  //! Constructs row of views for given `item` and inserts them into a `parent_view`.
  ViewItem *ProcessItem(SessionItem *item, ViewItem *parent_view, int row)
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

  //! Insert views for parent's child at position `tag_index`.
  void InsertView(SessionItem *parent, const TagIndex &tag_index)
  {
    auto new_child = parent->GetItem(tag_index.tag, tag_index.index);
    if (auto parent_view = m_view_item_map.FindView(parent); parent_view)
    {
      auto next_parent_view = ProcessItem(new_child, parent_view, tag_index.index);
      if (next_parent_view)
      {
        Iterate(new_child, next_parent_view);
      }
    }
  }

  //! Remove row of ViewItem's corresponding to given item.
  void RemoveRowOfViews(SessionItem *item)
  {
    if (auto view = m_view_item_map.FindView(item); view)
    {
      m_view_model->removeRow(view->parent(), view->row());
      m_view_item_map.Remove(item);
    }
  }

  void InitViewModel()
  {
    CheckInitialState();
    m_view_item_map.Clear();
    m_view_item_map.Update(GetRootItem(), m_view_model->rootItem());
    Iterate(GetRootItem(), m_view_model->rootItem());
  }

  void SetRootSessionItemIntern(SessionItem *item)
  {
    SessionItem *root_item = item ? item : m_model->GetRootItem();
    m_root_item_path = utils::PathFromItem(item);

    if (root_item->GetModel() != m_model)
    {
      throw std::runtime_error("Error: atttemp to use item from alien model as new root.");
    }

    m_view_model->ResetRootViewItem(CreateRootViewItem(root_item), /*notify*/ false);
  }
};

ViewModelController::ViewModelController(SessionModel *model, ViewModelBase *view_model)
    : p_impl(std::make_unique<ViewModelControllerImpl>(model, view_model))
{
}

void ViewModelController::SetChildrenStrategy(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy)
{
  p_impl->m_children_strategy = std::move(children_strategy);
}

void ViewModelController::SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy)
{
  p_impl->m_row_strategy = std::move(row_strategy);
}

ViewModelController::~ViewModelController() = default;

void ViewModelController::OnItemInserted(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->InsertView(parent, tag_index);
}

void ViewModelController::OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index)
{
  auto item_to_remove = parent->GetItem(tag_index.tag, tag_index.index);

  if (item_to_remove == p_impl->GetRootItem()
      || utils::IsItemAncestor(p_impl->GetRootItem(), item_to_remove))
  {
    // special case when user removes SessionItem which is one of ancestors of our root item
    // or root item itself
    p_impl->m_root_item_path = {};
    p_impl->m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
  }
  else
  {
    p_impl->RemoveRowOfViews(item_to_remove);
  }
}

void ViewModelController::OnDataChanged(SessionItem *item, int role)
{
  for (auto view : utils::FindViews(p_impl->m_view_model, item))
  {
    if (isValidItemRole(view, role))
    {
      auto index = p_impl->m_view_model->indexFromItem(view);
      emit p_impl->m_view_model->dataChanged(index, index, utils::ItemRoleToQtRole(role));
    }
  }
}

void ViewModelController::OnModelAboutToBeReset(SessionModel *model)
{
  (void)model;
  // Here we are notified that the model content will be destroyed soon.
  // To let all views looking at ViewModelBase to perform necessary bookkeeping we have to
  // emit internal QAbstractViewModel::beginResetModel already now, while `model` content is still
  // alive.
  p_impl->m_view_model->BeginResetModelNotify();
}

void ViewModelController::OnModelReset(SessionModel *model)
{
  auto custom_root_item = utils::ItemFromPath(*model, p_impl->m_root_item_path);

  p_impl->m_mute_notify = true;
  p_impl->SetRootSessionItemIntern(custom_root_item);
  p_impl->InitViewModel();
  p_impl->m_view_model->EndResetModelNotify();  //  BeginResetModel was already called
  p_impl->m_mute_notify = false;
}

void ViewModelController::OnModelAboutToBeDestroyed(SessionModel *model)
{
  (void)model;
  p_impl->m_root_item_path = {};
  p_impl->m_view_model->ResetRootViewItem(CreateRootViewItem(nullptr));
}

//! Inits ViewModel by iterating through SessionModel.

void ViewModelController::Init(SessionItem *root_item)
{
  p_impl->CheckInitialState();

  p_impl->m_mute_notify = true;
  p_impl->m_view_model->BeginResetModelNotify();
  p_impl->SetRootSessionItemIntern(root_item);
  p_impl->InitViewModel();
  p_impl->m_view_model->EndResetModelNotify();
  p_impl->m_mute_notify = false;
}

QStringList ViewModelController::GetHorizontalHeaderLabels() const
{
  return p_impl->m_row_strategy->GetHorizontalHeaderLabels();
}

}  // namespace mvvm
