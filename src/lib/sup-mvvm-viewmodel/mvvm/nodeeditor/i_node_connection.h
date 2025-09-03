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

#ifndef MVVM_NODEEDITOR_I_NODE_CONNECTION_H_
#define MVVM_NODEEDITOR_I_NODE_CONNECTION_H_

namespace mvvm
{

class INodePort;

/**
 * @brief The INodeConnection defines an interface to handle the data and logic on board of
 * NodeConnectionShape.
 *
 * The connection is a Bezier curve connecting two node ports. The connection's "start"
 * port corresponds to the port where the user has started the drawing. The connection's "end" port
 * corresponds to the port, where the user has released the mouse.
 */
class INodeConnection
{
public:
  INodeConnection() = default;
  virtual ~INodeConnection() = default;

  INodeConnection(const INodeConnection&) = delete;
  INodeConnection& operator=(const INodeConnection&) = delete;
  INodeConnection(INodeConnection&&) = delete;
  INodeConnection& operator=(INodeConnection&&) = delete;

  /**
   * @brief Returns the start port of the connection.
   */
  virtual const INodePort* GetStartPort() const = 0;

  /**
   * @brief Sets the start port of the connection.
   */
  virtual void SetStartPort(const INodePort* port) = 0;

  /**
   * @brief Sets the end port of the connection.
   */
  virtual void SetEndPort(const INodePort* port) = 0;

  /**
   * @brief Returns the end port of the connection.
   */
  virtual const INodePort* GetEndPort() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
