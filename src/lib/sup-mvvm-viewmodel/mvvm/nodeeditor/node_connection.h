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

#ifndef MVVM_NODEEDITOR_NODE_CONNECTION_H_
#define MVVM_NODEEDITOR_NODE_CONNECTION_H_

#include <mvvm/nodeeditor/i_node_connection.h>

namespace mvvm
{

/**
 * @brief The NodeConnection is a default implementation of INodeConnection to carry basic
 * connection logic.
 */
class NodeConnection : public INodeConnection
{
public:
  NodeConnection() = default;
  NodeConnection(INodePort* start_port, INodePort* end_port);
  ~NodeConnection() override;

  NodeConnection(const NodeConnection&) = delete;
  NodeConnection& operator=(const NodeConnection&) = delete;
  NodeConnection(NodeConnection&&) = delete;
  NodeConnection& operator=(NodeConnection&&) = delete;

  const INodePort* GetStartPort() const override;

  void SetStartPort(INodePort* port) override;

  const INodePort* GetEndPort() const override;

  void SetEndPort(INodePort* port) override;

  bool HasPort(const INodePort* port) const override;

  void ForgetPort(INodePort* port) override;

  void UpdateConnectionShape(const NodeOperationStates& operation_states) override;

  void SetNotifyShapeCallback(shape_callback_t callback) override;

private:
  void SetStartPortImpl(INodePort* port);
  void SetEndPortImpl(INodePort* port);

  INodePort* m_start_port{nullptr};
  INodePort* m_end_port{nullptr};
  shape_callback_t m_notify_shape_callback;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_CONNECTION_H_
