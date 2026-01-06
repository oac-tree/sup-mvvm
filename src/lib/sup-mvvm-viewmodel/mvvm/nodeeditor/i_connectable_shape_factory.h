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

#ifndef MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_FACTORY_H_
#define MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_FACTORY_H_

#include <memory>

namespace mvvm
{

class SessionItem;
class ConnectableShape;

/**
 * @brief The IConnectableShapeFactory creates connectable shapes to place on the graphics scene.
 *
 */
class IConnectableShapeFactory
{
public:
  IConnectableShapeFactory() = default;
  virtual ~IConnectableShapeFactory() = default;

  IConnectableShapeFactory(const IConnectableShapeFactory&) = delete;
  IConnectableShapeFactory& operator=(const IConnectableShapeFactory&) = delete;
  IConnectableShapeFactory(IConnectableShapeFactory&&) = delete;
  IConnectableShapeFactory& operator=(IConnectableShapeFactory&&) = delete;

  /**
   * @brief Creates and returns a new ConnectableShape instance for the provided item.
   *
   * @param item The item for which the adapter is to be created.
   * @return A unique pointer to the newly created adapter instance.
   */
  virtual std::unique_ptr<ConnectableShape> CreateShape(SessionItem* item) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_FACTORY_H_
