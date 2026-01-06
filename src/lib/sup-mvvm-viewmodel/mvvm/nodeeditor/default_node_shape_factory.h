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

#ifndef MVVM_NODEEDITOR_DEFAULT_NODE_SHAPE_FACTORY_H_
#define MVVM_NODEEDITOR_DEFAULT_NODE_SHAPE_FACTORY_H_

#include <mvvm/nodeeditor/i_connectable_shape_factory.h>

namespace mvvm
{

class SessionItem;
class ConnectableShape;

/**
 * @brief The DefaultNodeShapeFactory creates shapes representing NodeItems.
 */
class DefaultNodeShapeFactory : public IConnectableShapeFactory
{
public:
  DefaultNodeShapeFactory() = default;
  ~DefaultNodeShapeFactory() override = default;

  DefaultNodeShapeFactory(const DefaultNodeShapeFactory&) = delete;
  DefaultNodeShapeFactory& operator=(const DefaultNodeShapeFactory&) = delete;
  DefaultNodeShapeFactory(DefaultNodeShapeFactory&&) = delete;
  DefaultNodeShapeFactory& operator=(DefaultNodeShapeFactory&&) = delete;

  std::unique_ptr<ConnectableShape> CreateShape(SessionItem* item) const override;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_DEFAULT_NODE_SHAPE_FACTORY_H_
