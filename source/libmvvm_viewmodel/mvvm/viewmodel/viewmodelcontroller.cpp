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

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/viewmodel/viewitemfactory.h"
#include "mvvm/viewmodel/viewitemmap.h"
#include "mvvm/viewmodel/viewmodelutils.h"
#include "mvvm/viewmodelbase/viewmodelbase.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

#include <map>
#include <stdexcept>

namespace
{
std::vector<std::unique_ptr<ModelView::ViewItem>> ConstructRow(ModelView::SessionItem *item)
{
  std::vector<std::unique_ptr<ModelView::ViewItem>> result;
  result.emplace_back(ModelView::CreateDisplayNameViewItem(item));
  result.emplace_back(ModelView::CreateDataViewItem(item));
  return result;
}

//! Returns true if given SessionItem role is valid for view
//! FIXME consider moving it to row builder
bool isValidItemRole(const ModelView::ViewItem *view, int item_role)
{
  static std::map<int, int> role_to_column = {{ModelView::DataRole::kDisplay, 0},
                                              {ModelView::DataRole::kData, 1}};

  if (item_role == ModelView::DataRole::kAppearance || item_role == ModelView::DataRole::kTooltip)
  {
    return true;
  }

  return role_to_column[item_role] == view->column();
}

}  // namespace

namespace ModelView
{
struct ViewModelController::ViewModelControllerImpl
{
  SessionModel *m_model{nullptr};
  ViewModelBase *m_view_model{nullptr};
  ViewItemMap m_view_item_map;

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

    if (m_view_model->rowCount())
    {
      throw std::runtime_error("Error in ViewModewlController: viewmodel is not empty");
    }
  }

  SessionItem *GetRootItem() const
  {
    // TODO make possible change of root item to particular branch
    return m_model->GetRootItem();
  }

  void Iterate(SessionItem *item, ViewItem *parent_view)
  {
    for (auto *child : item->GetAllItems())
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
    auto row_of_views = ConstructRow(item);
    if (!row_of_views.empty())
    {
      auto *next_parent_view = row_of_views.at(0).get();
      m_view_model->insertRow(parent_view, row, std::move(row_of_views));
      m_view_item_map.Insert(item, next_parent_view);
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
    m_view_item_map.Insert(GetRootItem(), m_view_model->rootItem());
    Iterate(GetRootItem(), m_view_model->rootItem());
  }
};

ViewModelController::ViewModelController(SessionModel *model, ViewModelBase *view_model)
    : p_impl(std::make_unique<ViewModelControllerImpl>(model, view_model))
{
}

ViewModelController::~ViewModelController() = default;

void ViewModelController::OnItemInserted(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->InsertView(parent, tag_index);
}

void ViewModelController::OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->RemoveRowOfViews(parent->GetItem(tag_index.tag, tag_index.index));
}

void ViewModelController::OnDataChanged(SessionItem *item, int role)
{
  for (auto view : Utils::FindViews(p_impl->m_view_model, item))
  {
    if (isValidItemRole(view, role))
    {
      auto index = p_impl->m_view_model->indexFromItem(view);
      emit p_impl->m_view_model->dataChanged(index, index, Utils::ItemRoleToQtRole(role));
    }
  }
}

//! Inits ViewModel by iterating through SessionModel.

void ViewModelController::Init()
{
  p_impl->CheckInitialState();
  p_impl->m_view_model->ResetRootViewItem(
      Utils::CreateLabelViewItem(p_impl->m_model->GetRootItem(), {}));
  p_impl->InitViewModel();
}

}  // namespace ModelView
