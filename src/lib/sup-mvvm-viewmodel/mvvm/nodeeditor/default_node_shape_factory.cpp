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

#include "default_node_shape_factory.h"

#include "connectable_shape.h"
#include "node_item_connectable_shape_adapter.h"
#include "top_bottom_port_position_strategy.h"

#include <mvvm/standarditems/node_item.h>

namespace mvvm
{

std::unique_ptr<ConnectableShape> DefaultNodeShapeFactory::CreateShape(SessionItem *item) const
{
  auto adapter = std::make_unique<NodeItemConnectableShapeAdapter>(dynamic_cast<NodeItem *>(item));
  auto position_strategy = std::make_unique<TopBottomPortPositionStrategy>();
  return std::make_unique<ConnectableShape>(std::move(adapter), std::move(position_strategy));
}

}  // namespace mvvm
