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

#include "mock_node_connection.h"

namespace mvvm::test
{

NodeConnectionDecorator::NodeConnectionDecorator(INodeConnection *decoratee)
    : m_decoratee(decoratee)
{
}

const INodePort *NodeConnectionDecorator::GetStartPort() const
{
  return m_decoratee->GetStartPort();
}

void NodeConnectionDecorator::SetStartPort(INodePort *port)
{
  m_decoratee->SetStartPort(port);
}

const INodePort *NodeConnectionDecorator::GetEndPort() const
{
  return m_decoratee->GetEndPort();
}

void NodeConnectionDecorator::SetEndPort(INodePort *port)
{
  m_decoratee->SetEndPort(port);
}

bool NodeConnectionDecorator::HasPort(const INodePort *port) const
{
  return m_decoratee->HasPort(port);
}

void NodeConnectionDecorator::ForgetPort(INodePort *port)
{
  m_decoratee->ForgetPort(port);
}

void NodeConnectionDecorator::UpdateConnectionShape()
{
  m_decoratee->UpdateConnectionShape();
}

void NodeConnectionDecorator::SetNotifyShapeCallback(std::function<void()> callback)
{
  m_decoratee->SetNotifyShapeCallback(std::move(callback));
}

}  // namespace mvvm::test
