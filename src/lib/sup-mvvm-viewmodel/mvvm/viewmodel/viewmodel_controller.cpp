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

#include "viewmodel_controller.h"

#include <mvvm/core/exceptions.h>

namespace mvvm
{

ViewModelController::ViewModelController(std::unique_ptr<IViewModelController> impl)
    : p_impl(std::move(impl))
{
  if (!p_impl)
  {
    throw RuntimeException("Invalid controller");
  }
}

ViewModelController::~ViewModelController() = default;

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

int ViewModelController::GetColumnCount() const
{
  return p_impl->GetColumnCount();
}

QStringList ViewModelController::GetHorizontalHeaderLabels() const
{
  return p_impl->GetHorizontalHeaderLabels();
}

void ViewModelController::SetRootItemImpl(SessionItem *root_item)
{
  p_impl->SetRootItem(root_item);
}

}  // namespace mvvm
