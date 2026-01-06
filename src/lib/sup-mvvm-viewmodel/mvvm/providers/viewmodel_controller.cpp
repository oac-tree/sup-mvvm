/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "viewmodel_controller.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace mvvm
{

ViewModelController::ViewModelController(std::unique_ptr<IViewModelController> impl)
    : p_impl(std::move(impl))
{
  if (!p_impl)
  {
    throw RuntimeException("Invalid controller implementation");
  }
}

ViewModelController::~ViewModelController() = default;

void ViewModelController::OnItemInsertedEvent(const ItemInsertedEvent& event)
{
  p_impl->OnItemInsertedEvent(event);
}

void ViewModelController::OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event)
{
  p_impl->OnAboutToRemoveItemEvent(event);
}

void ViewModelController::OnDataChangedEvent(const DataChangedEvent& event)
{
  p_impl->OnDataChangedEvent(event);
}

void ViewModelController::OnModelAboutToBeResetEvent(const ModelAboutToBeResetEvent& event)
{
  p_impl->OnModelAboutToBeResetEvent(event);
}

void ViewModelController::OnModelResetEvent(const ModelResetEvent& event)
{
  p_impl->OnModelResetEvent(event);
}

void ViewModelController::OnModelAboutToBeDestroyedEvent(const ModelAboutToBeDestroyedEvent& event)
{
  p_impl->OnModelAboutToBeDestroyedEvent(event);
}

const SessionItem* ViewModelController::GetRootItem() const
{
  return p_impl->GetRootItem();
}

std::size_t ViewModelController::GetColumnCount() const
{
  return p_impl->GetColumnCount();
}

QStringList ViewModelController::GetHorizontalHeaderLabels() const
{
  return p_impl->GetHorizontalHeaderLabels();
}

void ViewModelController::SetRootItemImpl(SessionItem* root_item)
{
  p_impl->SetRootItem(root_item);
}

}  // namespace mvvm
