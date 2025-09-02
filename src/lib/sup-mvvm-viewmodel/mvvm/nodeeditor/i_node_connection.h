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

/**
 * @brief The INodeConnection defines an interface to handle the data and logic on board of
 * NodeConnectionShape.
 *
 * The connection is a bezier curve connecting two node ports together.
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
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
