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

ConnectableShapeDecorator::ConnectableShapeDecorator(IConnectableShapeAdapter *decoratee)
    : m_decoratee(decoratee)
{
}

SessionItem *ConnectableShapeDecorator::GetItem() const
{
  return m_decoratee->GetItem();
}

ConnectableShapeDecorator::~ConnectableShapeDecorator() = default;

QString ConnectableShapeDecorator::GetDisplayName() const
{
  return m_decoratee->GetDisplayName();
}

QColor ConnectableShapeDecorator::GetBaseColor() const
{
  return m_decoratee->GetBaseColor();
}

double ConnectableShapeDecorator::GetX() const
{
  return m_decoratee->GetX();
}

void ConnectableShapeDecorator::SetX(double value)
{
  m_decoratee->SetX(value);
}

double ConnectableShapeDecorator::GetY() const
{
  return m_decoratee->GetY();
}

void ConnectableShapeDecorator::SetY(double value)
{
  m_decoratee->SetY(value);
}

std::vector<PortInfo> ConnectableShapeDecorator::GetPortInfos() const
{
  return m_decoratee->GetPortInfos();
}

}  // namespace mvvm::test
