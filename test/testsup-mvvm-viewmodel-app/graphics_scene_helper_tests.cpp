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

#include "mvvm/nodeeditor/graphics_scene_helper.h"

#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_editor_constants.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>
#include <mvvm/test/test_container_helper.h>

#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>
#include <testutils/toy_items.h>

#include <QGraphicsScene>
#include <QLinearGradient>

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

  EXPECT_TRUE(utils::HaveSameElements(shapes, expected_selected_shapes));
}

TEST_F(GraphicsSceneHelperTests, GetUnderlyingItem)
{
  toyitems::SequenceItem sequence;

  auto shape = CreateConnectableShape(&sequence);

  EXPECT_EQ(GetUnderlyingItem(shape.get()), &sequence);
  EXPECT_EQ(GetUnderlyingItem<toyitems::SequenceItem>(shape.get()), &sequence);
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

TEST_F(GraphicsSceneHelperTests, IsConnected)
{
  QGraphicsScene scene;

  ApplicationModel model;
  EXPECT_FALSE(IsParentChildConnection(nullptr, nullptr, nullptr));

  auto sequence = model.InsertItem<toyitems::SequenceItem>();
  auto sequence_shape = CreateConnectableShape(sequence);
  scene.addItem(sequence_shape.get());

  auto wait = model.InsertItem<toyitems::WaitItem>();
  auto wait_shape = CreateConnectableShape(wait);
  scene.addItem(wait_shape.get());

  auto connection_shape = sequence_shape->MakeChildConnected(wait_shape.get());
  EXPECT_TRUE(IsParentChildConnection(sequence_shape.get(), wait_shape.get(), connection_shape));

  EXPECT_FALSE(IsParentChildConnection(wait_shape.get(), sequence_shape.get(), connection_shape));
}

TEST_F(GraphicsSceneHelperTests, ConnectableShapeGradient)
{
  auto gradient = ConnectableShapeGradient(Qt::red, QRectF(0, 0, 100, 200));
  ASSERT_EQ(gradient.type(), QGradient::Type::LinearGradient);
}

TEST_F(GraphicsSceneHelperTests, FindSceneItems)
{
  toyitems::SequenceItem sequence0;
  toyitems::SequenceItem sequence1;
  QGraphicsScene scene;

  auto shape0 = CreateConnectableShape(&sequence0);
  auto shape1 = CreateConnectableShape(&sequence1);

  scene.addItem(shape0.release());
  scene.addItem(shape1.release());

  EXPECT_EQ(FindSceneItems(scene), std::vector<mvvm::SessionItem*>({&sequence0, &sequence1}));
}

}  // namespace mvvm::test
