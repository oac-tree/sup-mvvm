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

#include "notifying_connectable_shape_adapter.h"

#include <mvvm/signals/item_listener.h>

namespace mvvm
{

NotifyingConnectableShapeAdapter::NotifyingConnectableShapeAdapter(
    std::unique_ptr<IConnectableShapeAdapter> shape_adapter,
    const std::function<void()> &update_shape_callback)
    : m_shape_adapter(std::move(shape_adapter)), m_update_shape_callback(update_shape_callback)
{
}

NotifyingConnectableShapeAdapter::~NotifyingConnectableShapeAdapter() = default;

SessionItem *NotifyingConnectableShapeAdapter::GetItem() const
{
  return m_shape_adapter->GetItem();
}

QString NotifyingConnectableShapeAdapter::GetDisplayName() const
{
  return m_shape_adapter->GetDisplayName();
}

QColor NotifyingConnectableShapeAdapter::GetBaseColor() const
{
  return m_shape_adapter->GetBaseColor();
}
double NotifyingConnectableShapeAdapter::GetX() const
{
  return m_shape_adapter->GetX();
}
void NotifyingConnectableShapeAdapter::SetX(double value)
{
  m_shape_adapter->SetX(value);
}
double NotifyingConnectableShapeAdapter::GetY() const
{
  return m_shape_adapter->GetY();
}
void NotifyingConnectableShapeAdapter::SetY(double value)
{
  m_shape_adapter->SetY(value);
}
std::vector<PortInfo> NotifyingConnectableShapeAdapter::GetPortInfos() const
{
  return m_shape_adapter->GetPortInfos();
}

}  // namespace mvvm
