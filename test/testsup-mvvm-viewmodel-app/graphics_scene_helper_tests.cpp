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

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_editor_constants.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>

#include <gtest/gtest.h>
#include <testutils/test_container_helper.h>
#include <testutils/test_node_editor_helper.h>
#include <testutils/toy_items.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for helper functions from graphics_scene_helper.h
 */
class GraphicsSceneHelperTests : public ::testing::Test
{
public:
  static std::unique_ptr<ConnectableShape> CreateConnectableShape(NodeItem* node_item)
  {
    auto adapter = std::make_unique<NodeItemConnectableShapeAdapter>(node_item);
    auto position_strategy = std::make_unique<TopBottomPortPositionStrategy>();
    return std::make_unique<ConnectableShape>(std::move(adapter), std::move(position_strategy));
  }
};

TEST_F(GraphicsSceneHelperTests, GetShapes)
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

TEST_F(GraphicsSceneHelperTests, GetShapesInArea)
{
  QGraphicsScene scene;

  auto port1 = CreateNodePortShape();
  scene.addItem(port1.get());
  port1->setPos(10.0, 20.0);

  auto port2 = CreateNodePortShape();
  scene.addItem(port2.get());
  port2->setPos(100.0, 200.0);

  const double size = 2 * constants::GetDefaultPortRadius();
  QRectF area(0, 0, size, size);
  area.moveCenter(QPointF(100.0, 200.0));
  auto ports = GetShapes<NodePortShape>(scene, area);
  ASSERT_EQ(ports.size(), 1);
  ASSERT_EQ(ports.at(0), port2.get());
}

TEST_F(GraphicsSceneHelperTests, GetSelectedShapes)
{
  toyitems::SequenceItem sequence;
  QGraphicsScene scene;

  auto shape1 = CreateConnectableShape(&sequence);
  auto shape2 = CreateConnectableShape(&sequence);
  auto shape3 = CreateConnectableShape(&sequence);
  auto shape4 = CreateConnectableShape(&sequence);

  scene.addItem(shape1.get());
  scene.addItem(shape2.get());
  scene.addItem(shape3.get());
  scene.addItem(shape4.get());

  auto shapes = GetSelectedShapes<ConnectableShape>(scene);
  ASSERT_EQ(shapes.size(), 0);

  shape4->setSelected(true);
  shape2->setSelected(true);
  shapes = GetSelectedShapes<ConnectableShape>(scene);
  auto expected_selected_shapes = std::vector<ConnectableShape*>({shape2.get(), shape4.get()});

  EXPECT_TRUE(HaveSameElements(shapes, expected_selected_shapes));
}

TEST_F(GraphicsSceneHelperTests, GetUnderlyingItem)
{
  toyitems::SequenceItem sequence;
  auto adapter = std::make_unique<NodeItemConnectableShapeAdapter>(&sequence);
  auto position_strategy = std::make_unique<TopBottomPortPositionStrategy>();
  ConnectableShape shape(std::move(adapter), std::move(position_strategy));
  EXPECT_EQ(GetUnderlyingItem(&shape), &sequence);
}

TEST_F(GraphicsSceneHelperTests, GetInputPort)
{
  toyitems::SequenceItem sequence;
  auto sequence_shape = CreateConnectableShape(&sequence);

  EXPECT_NE(GetInputPort(*sequence_shape), nullptr);
  EXPECT_NE(GetInputPort(*sequence_shape, 0U), nullptr);
  EXPECT_EQ(GetInputPort(*sequence_shape, 1U), nullptr);

  toyitems::WaitItem wait;
  auto wait_shape = CreateConnectableShape(&wait);
  EXPECT_EQ(GetInputPort(*wait_shape), nullptr);
  EXPECT_EQ(GetInputPort(*wait_shape, 0U), nullptr);
  EXPECT_EQ(GetInputPort(*wait_shape, 1U), nullptr);
}

TEST_F(GraphicsSceneHelperTests, GetOutputPort)
{
  toyitems::SequenceItem sequence;
  auto sequence_shape = CreateConnectableShape(&sequence);

  EXPECT_NE(GetOutputPort(*sequence_shape), nullptr);
  EXPECT_NE(GetOutputPort(*sequence_shape, 0U), nullptr);
  EXPECT_EQ(GetOutputPort(*sequence_shape, 1U), nullptr);

  toyitems::WaitItem wait;
  auto wait_shape = CreateConnectableShape(&wait);
  EXPECT_NE(GetOutputPort(*wait_shape), nullptr);
  EXPECT_NE(GetOutputPort(*wait_shape, 0U), nullptr);
  EXPECT_EQ(GetOutputPort(*wait_shape, 1U), nullptr);
}

}  // namespace mvvm::test
