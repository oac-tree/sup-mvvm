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
#include <testutils/test_node_editor_helper.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for helper functions from graphics_scene_helper.h
 */
class GraphicsSceneHelperTests : public ::testing::Test
{
};

TEST_F(GraphicsSceneHelperTests, GetShape)
{
  QGraphicsScene scene;

  auto port1 = CreateNodePortShape();
  auto port2 = CreateNodePortShape();
  scene.addItem(port1.get());
  scene.addItem(port2.get());
  auto ports = GetShapes<NodePortShape>(scene);
  ASSERT_EQ(ports.size(), 2);
  ASSERT_EQ(ports.at(0), port1.get());
  ASSERT_EQ(ports.at(1), port2.get());
}

}  // namespace mvvm::test
