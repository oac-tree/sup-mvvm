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

#include "mvvm/nodeeditor/graphics_scene_helper.h"

#include <mvvm/nodeeditor/node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>

#include <gtest/gtest.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for helper functions from graphics_scene_helper.h
 */
class GraphicsSceneHelperTests : public ::testing::Test
{
public:
  std::unique_ptr<NodePortShape> CreateNodePort()
  {
    auto node_port = std::make_unique<NodePort>(PortInfo{PortDirection::kOutput, "type_abc"});
    const PortAppearance port_appearance(PortLocation::kLeft, "TestPort", QColor(Qt::red));
    const NodePortShape shape(port_appearance, std::move(node_port), nullptr);
    return std::make_unique<NodePortShape>(port_appearance, std::move(node_port), nullptr);
  }
};

TEST_F(GraphicsSceneHelperTests, GetShape)
{
  QGraphicsScene scene;

  auto port1 = CreateNodePort();
  auto port2 = CreateNodePort();
  scene.addItem(port1.get());
  scene.addItem(port2.get());
  auto ports = GetShapes<NodePortShape>(scene);
  ASSERT_EQ(ports.size(), 2);
  ASSERT_EQ(ports.at(0), port1.get());
  ASSERT_EQ(ports.at(1), port2.get());
}

}  // namespace mvvm::test
