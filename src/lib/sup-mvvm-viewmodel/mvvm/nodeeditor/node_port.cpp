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

#include "node_port.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <algorithm>

namespace mvvm
{

NodePort::NodePort(PortInfo port_info) : m_port_info(std::move(port_info)) {}

PortInfo NodePort::GetPortInfo() const
{
  return m_port_info;
}

std::vector<const INodeConnection *> NodePort::GetConnections() const
{
  return m_connections;
}

void NodePort::AppendConnection(const INodeConnection *connection)
{
  auto iter = std::find(m_connections.begin(), m_connections.end(), connection);
  if (iter != m_connections.end())
  {
    throw RuntimeException("The connection is already present in the port's connections list.");
  }
  m_connections.push_back(connection);
}

void NodePort::RemoveConnection(const INodeConnection *connection)
{
  auto iter = std::find(m_connections.begin(), m_connections.end(), connection);
  if (iter == m_connections.end())
  {
    throw RuntimeException("The connection is not present in the port's connections list.");
  }
  m_connections.erase(iter);
}

}  // namespace mvvm
