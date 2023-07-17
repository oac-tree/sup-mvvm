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

#include "mvvm/viewmodel/viewmodel_controller.h"

#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel/viewmodel_controller_impl.h>

namespace mvvm
{

ViewModelController::ViewModelController(ViewModelBase *view_model)
    : p_impl(std::make_unique<ViewModelControllerImpl>(view_model))
{
}

ViewModelController::ViewModelController(SessionModelInterface *model, ViewModelBase *view_model)
    : p_impl(std::make_unique<ViewModelControllerImpl>(view_model))
{
}

ViewModelController::~ViewModelController() = default;

void ViewModelController::SetChildrenStrategy(
    std::unique_ptr<ChildrenStrategyInterface> children_strategy)
{
  p_impl->SetChildrenStrategy(std::move(children_strategy));
}

void ViewModelController::SetRowStrategy(std::unique_ptr<RowStrategyInterface> row_strategy)
{
  p_impl->SetRowStrategy(std::move(row_strategy));
}

void ViewModelController::OnModelEvent(const ItemInsertedEvent &event)
{
  p_impl->OnModelEvent(event);
}

void ViewModelController::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  p_impl->OnModelEvent(event);
}

void ViewModelController::OnModelEvent(const DataChangedEvent &event)
{
  p_impl->OnModelEvent(event);
}

void ViewModelController::OnModelEvent(const ModelAboutToBeResetEvent &event)
{
  p_impl->OnModelEvent(event);
}

void ViewModelController::OnModelEvent(const ModelResetEvent &event)
{
  p_impl->OnModelEvent(event);
}

void ViewModelController::OnModelEvent(const ModelAboutToBeDestroyedEvent &event)
{
  p_impl->OnModelEvent(event);
}

const SessionItem *ViewModelController::GetRootItem() const
{
  return p_impl->GetRootItem();
}

//! Inits ViewModel by iterating through SessionModel.

void ViewModelController::SetRootItemImpl(SessionItem *root_item)
{
  p_impl->SetRootItem(root_item);
}

QStringList ViewModelController::GetHorizontalHeaderLabels() const
{
  return p_impl->GetHorizontalHeaderLabels();
}

}  // namespace mvvm
