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

#include "mock_connectable_shape_adapter.h"

namespace mvvm::test
{

ConnectableShapeAdapterDecorator::ConnectableShapeAdapterDecorator(
    IConnectableShapeAdapter *decoratee)
    : m_decoratee(decoratee)
{
}

SessionItem *ConnectableShapeAdapterDecorator::GetItem() const
{
  return m_decoratee->GetItem();
}

ConnectableShapeAdapterDecorator::~ConnectableShapeAdapterDecorator() = default;

QString ConnectableShapeAdapterDecorator::GetDisplayName() const
{
  return m_decoratee->GetDisplayName();
}

QColor ConnectableShapeAdapterDecorator::GetBaseColor() const
{
  return m_decoratee->GetBaseColor();
}

double ConnectableShapeAdapterDecorator::GetX() const
{
  return m_decoratee->GetX();
}

void ConnectableShapeAdapterDecorator::SetX(double value)
{
  m_decoratee->SetX(value);
}

double ConnectableShapeAdapterDecorator::GetY() const
{
  return m_decoratee->GetY();
}

void ConnectableShapeAdapterDecorator::SetY(double value)
{
  m_decoratee->SetY(value);
}

std::vector<PortInfo> ConnectableShapeAdapterDecorator::GetPortInfos() const
{
  return m_decoratee->GetPortInfos();
}

NodeOperationStates ConnectableShapeAdapterDecorator::GetOperationStates() const
{
  return m_decoratee->GetOperationStates();
}

void ConnectableShapeAdapterDecorator::ProcessEvent(const node_event_t &event) const
{
  m_decoratee->ProcessEvent(event);
}

ConnectableShapeStyle ConnectableShapeAdapterDecorator::GetStyle() const
{
  return m_decoratee->GetStyle();
}

}  // namespace mvvm::test
