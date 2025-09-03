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

namespace mvvm
{

/**
 * @brief The INodePort defines an interface to handle the data and logic on board of
 * NodePortShape.
 *
 * The node port is a little round element on a graphics item which can be used to connect things
 * together.
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

};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ALL_ITEMS_VIEWMODEL_H_
