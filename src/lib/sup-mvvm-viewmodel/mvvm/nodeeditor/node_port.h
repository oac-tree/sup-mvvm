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

#ifndef MVVM_NODEEDITOR_NODE_PORT_H_
#define MVVM_NODEEDITOR_NODE_PORT_H_

#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/port_info.h>

namespace mvvm
{

class NodeConnection;

/**
 * @brief The NodePort is a default implementation of INodePort to carry basic connection logic.
 */
class NodePort : public INodePort
{
public:
  explicit NodePort(PortInfo port_info);
  ~NodePort() override = default;

  NodePort(const NodePort&) = delete;
  NodePort& operator=(const NodePort&) = delete;
  NodePort(NodePort&&) = delete;
  NodePort& operator=(NodePort&&) = delete;

  PortDirection GetPortDirection() const override;

  std::string GetPortType() const override;

  std::vector<const INodeConnection*> GetConnections() const override;

  void AppendConnection(const INodeConnection* connection) override;

  void RemoveConnection(const INodeConnection* connection) override;

  bool IsConnected(const INodePort* other) const override;

  bool IsConnectable(const INodePort* other) const override;

  PortInfo GetPortInfo() const;

private:
  PortInfo m_port_info;
  std::vector<const INodeConnection*> m_connections;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_PORT_H_
