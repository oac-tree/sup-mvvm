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
#include <mvvm/nodeeditor/i_node_connection.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace mvvm
{

NodePort::NodePort(PortInfo port_info) : m_port_info(std::move(port_info)) {}

NodePort::~NodePort()
{
  for (auto connection : utils::RemoveConst(m_connections))
  {
    connection->ForgetPort(this);
  }
}

PortDirection NodePort::GetPortDirection() const
{
  return m_port_info.GetPortDirection();
}

std::string NodePort::GetPortType() const
{
  return m_port_info.GetPortType();
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

bool NodePort::IsConnected(const INodePort *other) const
{
  if (other == nullptr)
  {
    return false;
  }

  return std::any_of(m_connections.begin(), m_connections.end(),
                     [other](const INodeConnection *conn) { return conn->HasPort(other); });
}

bool NodePort::IsConnectable(const INodePort *other) const
{
  if (other == nullptr)
  {
    return false;
  }

  const bool direction_matches = (GetPortDirection() == PortDirection::kInput
                                  && other->GetPortDirection() == PortDirection::kOutput)
                                 || (GetPortDirection() == PortDirection::kOutput
                                     && other->GetPortDirection() == PortDirection::kInput);

  const bool type_matches = GetPortType() == other->GetPortType();
  return direction_matches && type_matches;
}

PortInfo NodePort::GetPortInfo() const
{
  return m_port_info;
}

}  // namespace mvvm
