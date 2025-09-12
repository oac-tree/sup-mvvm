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

#include "connectable_view_model_controller.h"

#include "i_connectable_shape_factory.h"

namespace mvvm
{

ConnectableViewModelController::ConnectableViewModelController(
    std::unique_ptr<IConnectableShapeFactory> shape_factory)
    : m_shape_factory(std::move(shape_factory))
{
}

ConnectableViewModelController::~ConnectableViewModelController() = default;

void ConnectableViewModelController::SetRootItemImpl(SessionItem *root_item)
{
  (void)root_item;
}

void ConnectableViewModelController::SubscribeImpl(ISessionModel *model)
{
  (void)model;
}

}  // namespace mvvm
