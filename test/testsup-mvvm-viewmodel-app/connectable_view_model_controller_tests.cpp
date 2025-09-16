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
#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_editor_helper.h>

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
    auto shapes = utils::CastItems<ShapeT>(m_scene.items());
    std::reverse(shapes.begin(), shapes.end());
    return shapes;
  }

  /**
   * @brief Helpers to get underlying item from the shape.
   */
  static SessionItem* GetUnderlyingItem(ConnectableShape* shape)
  {
    return shape->GetConnectableShapeAdapter()->GetItem();
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

TEST_F(ConnectableViewModelControllerTest, SingleNodeItem)
{
  auto node_item = m_app_model.InsertItem<toyitems::SequenceItem>();

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 1);
}

TEST_F(ConnectableViewModelControllerTest, SequenceAndWaitItems)
{
  auto sequence_item = m_app_model.InsertItem<toyitems::SequenceItem>();
  auto wait_item = m_app_model.InsertItem<toyitems::WaitItem>();

  auto controller = CreateController();
  EXPECT_NO_THROW(controller->InitScene(m_app_model.GetRootItem()));

  auto shapes = GetShapes<ConnectableShape>();
  ASSERT_EQ(shapes.size(), 2);
}

TEST_F(ConnectableViewModelControllerTest, NestedItems)
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

}  // namespace mvvm::test
