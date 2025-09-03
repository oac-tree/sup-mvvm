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

#ifndef MVVM_NODEEDITOR_I_NODE_PORT_H_
#define MVVM_NODEEDITOR_I_NODE_PORT_H_

#include <mvvm/nodeeditor/node_editor_types.h>

#include <string>
#include <vector>

namespace mvvm
{

class INodeConnection;

/**
 * @brief The INodePort defines an interface to handle the data and logic on board of
 * NodePortShape.
 *
 * The node port is a little round element on a graphics item which can be used to connect things
 * together. One port can have multiple connections.
 */
class INodePort
{
public:
  INodePort() = default;
  virtual ~INodePort() = default;

  INodePort(const INodePort&) = delete;
  INodePort& operator=(const INodePort&) = delete;
  INodePort(INodePort&&) = delete;
  INodePort& operator=(INodePort&&) = delete;

  /**
   * @brief Returns port direction.
   */
  virtual PortDirection GetPortDirection() const = 0;

  /**
   * @brief Returns a string representing a port type.
   *
   * Ports of the same type can be connected together if their directions matches (input can be
   * connected with output).
   */
  virtual std::string GetPortType() const = 0;

  /**
   * @brief Returns all connections related to this port.
   */
  virtual std::vector<const INodeConnection*> GetConnections() const = 0;

  /**
   * @brief Appends a connection to this port.
   */
  virtual void AppendConnection(const INodeConnection* connection) = 0;

  /**
   * @brief Removes a connection from this port.
   */
  virtual void RemoveConnection(const INodeConnection* connection) = 0;

  /**
   * @brief Checks if this port is connected to the specified other port.
   */
  virtual bool IsConnected(const INodePort* other) const = 0;

  /**
   * @brief Checks if this port can be connected to the specified other port.
   */
  virtual bool IsConnectable(const INodePort* other) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
