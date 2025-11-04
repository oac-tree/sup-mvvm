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

#include "mock_node_port.h"

namespace mvvm::test
{

NodePortDecorator::NodePortDecorator(INodePort* decoratee) : m_decoratee(decoratee) {}

PortDirection NodePortDecorator::GetPortDirection() const
{
  return m_decoratee->GetPortDirection();
}

std::string NodePortDecorator::GetPortType() const
{
  return m_decoratee->GetPortType();
}

std::vector<const INodeConnection*> NodePortDecorator::GetConnections() const
{
  return m_decoratee->GetConnections();
}

void NodePortDecorator::AppendConnection(const INodeConnection* connection)
{
  m_decoratee->AppendConnection(connection);
}

void NodePortDecorator::RemoveConnection(const INodeConnection* connection)
{
  m_decoratee->RemoveConnection(connection);
}

bool NodePortDecorator::IsConnected(const INodePort* port) const
{
  return m_decoratee->IsConnected(port);
}

bool NodePortDecorator::IsConnectable(const INodePort* port) const
{
  return m_decoratee->IsConnectable(port);
}

std::pair<double, double> NodePortDecorator::GetScenePosition() const
{
  return m_decoratee->GetScenePosition();
}

void NodePortDecorator::SetScenePosition(double xpos, double ypos)
{
  m_decoratee->SetScenePosition(xpos, ypos);
}

PortInfo NodePortDecorator::GetPortInfo() const
{
  return m_decoratee->GetPortInfo();
}

void NodePortDecorator::UpdatePortShape(const NodeOperationStates& operation_states)
{
  m_decoratee->UpdatePortShape(operation_states);
}

void NodePortDecorator::SetNotifyShapeCallback(
    const std::function<void(const NodeOperationStates& operation_states)>& callback)
{
  m_decoratee->SetNotifyShapeCallback(callback);
}

}  // namespace mvvm::test
