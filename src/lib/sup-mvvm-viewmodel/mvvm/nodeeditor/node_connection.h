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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_H_

#include <mvvm/nodeeditor/i_node_connection.h>

namespace mvvm
{

class NodePort;

/**
 * @brief The NodeConnection is a default implementation of INodeConnection to carry basic
 * connection logic.
 */
class NodeConnection : public INodeConnection
{
public:
  NodeConnection() = default;
  ~NodeConnection() override = default;

  NodeConnection(const NodeConnection&) = delete;
  NodeConnection& operator=(const NodeConnection&) = delete;
  NodeConnection(NodeConnection&&) = delete;
  NodeConnection& operator=(NodeConnection&&) = delete;

  const NodePort* GetStartPort() const override;

  void SetStartPort(const NodePort* port) override;

  const NodePort* GetEndPort() const override;

  void SetEndPort(const NodePort* port) override;

private:
  const NodePort* m_start_port{nullptr};
  const NodePort* m_end_port{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_H_
