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

#include "mvvm/nodeeditor/connectable_view_model_controller.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/default_node_shape_factory.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_editor_helper.h>
#include <mvvm/nodeeditor/node_port_shape.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableViewModelController class.
 */
class ConnectableViewModelControllerTest : public ::testing::Test
{
public:
  /**
   * @brief Creates wired controller instance for testing.
   */
  std::unique_ptr<ConnectableViewModelController> CreateController()
  {
    auto factory = std::make_unique<DefaultNodeShapeFactory>();
    auto controller =
        std::make_unique<ConnectableViewModelController>(std::move(factory), &m_scene);
    return controller;
  }

  /**
   * @brief Helper to get shapes of specific type from the scene.
   *
   * The result will be sorted according the order of the addition.
   */
  template <typename ShapeT>
  std::vector<ShapeT*> GetShapes() const
  {
    return ::mvvm::GetShapes<ShapeT>(m_scene);
  }

  /**
   * @brief Validates if connection goes from child to parent.
   */
  static bool IsConnected(NodeConnectionShape* connection_shape, ConnectableShape* parent,
                          ConnectableShape* child)
  {
    auto child_output_port = GetPortsByDirection(child->GetPorts(), PortDirection::kOutput);
    auto parent_input_ports = GetPortsByDirection(parent->GetPorts(), PortDirection::kInput);
    const bool correct_ports = child_output_port.size() == 1 && parent_input_ports.size() == 1;
    auto connection = connection_shape->GetNodeConnection();
    return connection->GetStartPort() == child_output_port.front()
           && connection->GetEndPort() == parent_input_ports.front();
  }

  ApplicationModel m_app_model;
  QGraphicsScene m_scene;
};

TEST_F(ConnectableViewModelControllerTest, InitialState)
{
  std::unique_ptr<IConnectableShapeFactory> uninitialized_factory;
  EXPECT_THROW(ConnectableViewModelController(std::move(uninitialized_factory), nullptr),
               RuntimeException);
  EXPECT_THROW(ConnectableViewModelController(std::make_unique<DefaultNodeShapeFactory>(), nullptr),
               RuntimeException);

  EXPECT_NO_THROW(CreateController());
  auto controller = CreateController();
  EXPECT_THROW(controller->InitScene(nullptr), RuntimeException);
}

TEST_F(ConnectableViewModelControllerTest, InitSceneForEmptyModel)
{
  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));
  EXPECT_EQ(controller->GetRootItem(), m_app_model.GetRootItem());
  EXPECT_TRUE(m_scene.items().empty());
}

TEST_F(ConnectableViewModelControllerTest, InitSceneSingleNodeItem)
{
  auto node_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  node_item->SetX(10.0);
  node_item->SetY(20.0);

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  EXPECT_EQ(shapes.at(0)->scenePos().x(), 10.0);
  EXPECT_EQ(shapes.at(0)->scenePos().y(), 20.0);
}

TEST_F(ConnectableViewModelControllerTest, InitSceneSequenceAndWaitItems)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>();

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
}

TEST_F(ConnectableViewModelControllerTest, InitSceneNestedItems)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto wait_item1 = m_app_model.InsertItem<toyitems::WaitItem>(sequence_item);
  auto wait_item2 = m_app_model.InsertItem<toyitems::WaitItem>(sequence_item);

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();

  ASSERT_EQ(shapes.size(), 3);
  ASSERT_EQ(GetUnderlyingItem(shapes.at(0)), sequence_item);
  ASSERT_EQ(GetUnderlyingItem(shapes.at(1)), wait_item1);
  ASSERT_EQ(GetUnderlyingItem(shapes.at(2)), wait_item2);

  auto connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 2);

  // connects sequence_item and wait_item1
  ASSERT_TRUE(IsConnected(connections.at(0), shapes.at(0), shapes.at(1)));
  ASSERT_FALSE(IsConnected(connections.at(0), shapes.at(0), shapes.at(2)));

  // connects sequence_item and wait_item2
  ASSERT_TRUE(IsConnected(connections.at(1), shapes.at(0), shapes.at(2)));
  ASSERT_FALSE(IsConnected(connections.at(1), shapes.at(0), shapes.at(1)));
}

TEST_F(ConnectableViewModelControllerTest, InsertItemInEmptyModel)
{
  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto shapes = GetShapes<ConnectableShape>();

  ASSERT_EQ(shapes.size(), 1);
}

TEST_F(ConnectableViewModelControllerTest, InsertChildInParent)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();

  auto controller = CreateController();
  controller->InitScene(m_app_model.GetRootItem());

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);

  // inserting child
  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>(sequence_item);
  shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);
  ASSERT_TRUE(IsConnected(connections.at(0), shapes.at(0), shapes.at(1)));
  EXPECT_EQ(connections.at(0)->scene(), &m_scene);
}

TEST_F(ConnectableViewModelControllerTest, RemoveItemFromModel)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>();

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);

  m_app_model.RemoveItem(wait_item);
  shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);

  m_app_model.RemoveItem(sequence_item);
  shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 0);

  auto connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);
}

TEST_F(ConnectableViewModelControllerTest, RemoveParentAndChild)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>(sequence_item);

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  m_app_model.RemoveItem(sequence_item);

  // removing sequence removes all views and connections
  shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 0);
  connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);
}

TEST_F(ConnectableViewModelControllerTest, RemoveChildFromParent)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>(sequence_item);

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
  auto connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);

  m_app_model.RemoveItem(wait_item);

  // removing wait removes its; view and hanging connection
  shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
  connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 0);
}

TEST_F(ConnectableViewModelControllerTest, ValidateCoordinatesAfterMoveItemToParent)
{
  // Sequence and wait
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  sequence_item->SetX(10.0);
  sequence_item->SetY(20.0);

  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>();
  wait_item->SetX(100.0);
  wait_item->SetY(200.0);

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  // move wait so it become a child of sequence
  m_app_model.MoveItem(wait_item, sequence_item, TagIndex::Append());

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);

  auto sequence_shape = shapes.at(0);
  auto wait_shape = shapes.at(1);

  // coordinates are the same as before
  EXPECT_EQ(sequence_shape->scenePos().x(), 10.0);
  EXPECT_EQ(sequence_shape->scenePos().y(), 20.0);
  EXPECT_EQ(wait_shape->scenePos().x(), 100.0);
  EXPECT_EQ(wait_shape->scenePos().y(), 200.0);

  // there is one new connections
  auto connection_shapes = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connection_shapes.size(), 1);
  ASSERT_TRUE(IsConnected(connection_shapes.at(0), sequence_shape, wait_shape));

  // coordinates of node ports coincide with connection begin/end coordinates
  auto sequence_input_ports =
      GetPortsByDirection(sequence_shape->GetPorts(), PortDirection::kInput);
  ASSERT_EQ(sequence_input_ports.size(), 1);
  auto wait_output_ports = GetPortsByDirection(wait_shape->GetPorts(), PortDirection::kOutput);
  ASSERT_EQ(wait_output_ports.size(), 1);
  auto connection = connection_shapes.at(0)->GetNodeConnection();
  ASSERT_EQ(connection->GetEndPort(), sequence_input_ports.front());
  ASSERT_EQ(connection->GetStartPort(), wait_output_ports.front());
  auto [seq_port_x, seq_port_y] = sequence_input_ports.front()->GetScenePosition();
  auto [wait_port_x, wait_port_y] = wait_output_ports.front()->GetScenePosition();
  EXPECT_DOUBLE_EQ(seq_port_x, connection_shapes.at(0)->path().pointAtPercent(1.0).x());
  EXPECT_DOUBLE_EQ(seq_port_y, connection_shapes.at(0)->path().pointAtPercent(1.0).y());
  EXPECT_DOUBLE_EQ(wait_port_x, connection_shapes.at(0)->path().pointAtPercent(0.0).x());
  EXPECT_DOUBLE_EQ(wait_port_y, connection_shapes.at(0)->path().pointAtPercent(0.0).y());

  // actual coordinates of node port shapes coincide with the port coordinates
  auto port_shapes = GetShapes<NodePortShape>();
  ASSERT_EQ(port_shapes.size(), 3);  // two input/output for sequence and one for wait
  for (const auto& port_shape : port_shapes)
  {
    auto node_port = port_shape->GetNodePort();
    auto [port_x, port_y] = node_port->GetScenePosition();
    EXPECT_DOUBLE_EQ(port_x, port_shape->scenePos().x());
    EXPECT_DOUBLE_EQ(port_y, port_shape->scenePos().y());
  }
}

// TODO: test for automatic connection removal

}  // namespace mvvm::test
