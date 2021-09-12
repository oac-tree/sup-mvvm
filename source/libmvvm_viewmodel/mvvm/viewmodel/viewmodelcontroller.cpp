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

#include "mvvm/model/sessionmodel.h"
#include "mvvm/viewmodel/viewmodelbase.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include <stdexcept>

namespace ModelView
{
struct ViewModelController::ViewModelControllerImpl
{
  SessionModel *m_model{nullptr};
  ViewModelBase *m_view_model{nullptr};

  ViewModelControllerImpl(SessionModel *model, ViewModelBase *view_model)
      : m_model(model), m_view_model(view_model)
  {
  }

  void CheckInitialState()
  {
    if (!m_model)
      throw std::runtime_error("Error in ViewModewlController: model is absent");

    if (!m_view_model)
      throw std::runtime_error("Error in ViewModewlController: viewmodel is absent");

    if (m_view_model->rowCount())
      throw std::runtime_error("Error in ViewModewlController: viewmodel is not empty");
  }

  void InitViewModel() {}
};

ViewModelController::ViewModelController(SessionModel *model, ViewModelBase *view_model)
    : p_impl(std::make_unique<ViewModelControllerImpl>(model, view_model))
{
}

ViewModelController::~ViewModelController() = default;

void ViewModelController::OnAboutToInsertItem(SessionItem *parent, const TagIndex &tag_index) {}

void ViewModelController::OnItemInserted(SessionItem *parent, const TagIndex &tag_index) {}

void ViewModelController::OnAboutToRemoveItem(SessionItem *parent, const TagIndex &tag_index) {}

void ViewModelController::OnItemRemoved(SessionItem *parent, const TagIndex &tag_index) {}

void ViewModelController::OnDataChanged(SessionItem *item, int role) {}

//! Inits ViewModel by iterating through SessionModel.

void ViewModelController::Init()
{
  p_impl->CheckInitialState();
  p_impl->m_view_model->ResetRootViewItem(
      Utils::CreateLabelViewItem(p_impl->m_model->GetRootItem(), {}));
  p_impl->InitViewModel();
}

}  // namespace ModelView
