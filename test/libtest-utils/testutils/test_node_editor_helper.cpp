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

#include "test_node_editor_helper.h"

#include <mvvm/nodeeditor/node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>

namespace mvvm::test
{

std::unique_ptr<NodePortShape> CreateNodePortShape(PortDirection port_direction)
{
  auto node_port = std::make_unique<NodePort>(PortInfo{port_direction, "type_abc"});
  const PortAppearance port_appearance(PortLocation::kLeft, "TestPort", QColor(Qt::red));
  return std::make_unique<NodePortShape>(port_appearance, std::move(node_port), nullptr);
}

}  // namespace mvvm::test
