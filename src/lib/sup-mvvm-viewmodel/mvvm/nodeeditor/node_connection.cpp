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

#include "node_connection.h"

#include "i_node_port.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace mvvm
{

NodeConnection::NodeConnection(INodePort* start_port, INodePort* end_port)
{
  SetStartPortImpl(start_port);
  SetEndPortImpl(end_port);
}

NodeConnection::~NodeConnection()
{
  if (m_start_port != nullptr)
  {
    m_start_port->RemoveConnection(this);
  }

  if (m_end_port != nullptr)
  {
    m_end_port->RemoveConnection(this);
  }
}

const INodePort* NodeConnection::GetStartPort() const
{
  return m_start_port;
}

void NodeConnection::SetStartPort(INodePort* port)
{
  SetStartPortImpl(port);
}

const INodePort* NodeConnection::GetEndPort() const
{
  return m_end_port;
}

void NodeConnection::SetEndPort(INodePort* port)
{
  SetEndPortImpl(port);
}

bool NodeConnection::HasPort(const INodePort* port) const
{
  if (port == nullptr)
  {
    return false;
  }

  return (m_start_port == port) || (m_end_port == port);
}

void NodeConnection::ForgetPort(INodePort* port)
{
  if (m_start_port == port)
  {
    m_start_port = nullptr;
  }

  if (m_end_port == port)
  {
    m_end_port = nullptr;
  }
}

void NodeConnection::UpdateConnectionShape(const NodeOperationStates& operation_states)
{
  if (m_notify_shape_callback)
  {
    m_notify_shape_callback(operation_states);
  }
}

void NodeConnection::SetNotifyShapeCallback(shape_callback_t callback)
{
  m_notify_shape_callback = std::move(callback);
}

void NodeConnection::SetStartPortImpl(INodePort* port)
{
  if (m_start_port != nullptr)
  {
    throw RuntimeException("Current logic allows to set connection only once");
  }

  m_start_port = port;
  m_start_port->AppendConnection(this);
}

void NodeConnection::SetEndPortImpl(INodePort* port)
{
  if (m_end_port != nullptr)
  {
    throw RuntimeException("Current logic allows to set connection only once");
  }

  m_end_port = port;
  m_end_port->AppendConnection(this);
}

}  // namespace mvvm
