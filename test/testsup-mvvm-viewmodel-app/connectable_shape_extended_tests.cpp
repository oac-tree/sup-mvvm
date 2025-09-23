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

#include "mvvm/nodeeditor/connectable_shape.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_editor_helper.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>
#include <mvvm/test/mock_item_listener.h>
#include <mvvm/utils/container_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShape class, when view is fully constructed and belongs to scene.
 */
class ConnectableShapeExtendedTest : public ::testing::Test
{
public:
  ConnectableShapeExtendedTest()
      : m_node_item(m_app_model.InsertItem<NodeItem>()), m_mock_item_listener(m_node_item)
  {
  }

  /**
   * @brief Returns test object wired with proper connection adapter.
   */
  std::unique_ptr<ConnectableShape> CreateConnectableShape(NodeItem* node_item)
  {
    auto adapter = std::make_unique<NodeItemConnectableShapeAdapter>(node_item);
    auto position_strategy = std::make_unique<TopBottomPortPositionStrategy>();
    return std::make_unique<ConnectableShape>(std::move(adapter), std::move(position_strategy));
  }

  template <typename ShapeT>
  std::vector<ShapeT*> GetShapes()
  {
    return utils::CastItems<ShapeT>(m_scene.items());
  }

  ApplicationModel m_app_model;
  NodeItem* m_node_item{m_app_model.InsertItem<NodeItem>()};
  QGraphicsScene m_scene;

  ::testing::StrictMock<MockItemListener> m_mock_item_listener;
};

TEST_F(ConnectableShapeExtendedTest, DefaultCoordinatesAfterInsert)
{
  auto shape = CreateConnectableShape(m_node_item);
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  EXPECT_EQ(shape_ptr->x(), 0.0);
  EXPECT_EQ(shape_ptr->y(), 0.0);
  EXPECT_EQ(m_node_item->GetX(), 0.0);
  EXPECT_EQ(m_node_item->GetY(), 0.0);
}

TEST_F(ConnectableShapeExtendedTest, NonDefaultCoordinatesAfterInsert)
{
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(::testing::_)).Times(2);
  m_node_item->SetX(10.0);
  m_node_item->SetY(20.0);

  auto shape = CreateConnectableShape(m_node_item);
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  EXPECT_EQ(shape_ptr->x(), 10.0);
  EXPECT_EQ(shape_ptr->y(), 20.0);
  EXPECT_EQ(m_node_item->GetX(), 10.0);
  EXPECT_EQ(m_node_item->GetY(), 20.0);
}

TEST_F(ConnectableShapeExtendedTest, ChangeItemCoordinatesAfterInsert)
{
  auto shape = CreateConnectableShape(m_node_item);
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  const PropertyChangedEvent expected_event1{m_node_item, std::string{NodeItem::kXpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event1)).Times(1);
  const PropertyChangedEvent expected_event2{m_node_item, std::string{NodeItem::kYpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event2)).Times(1);

  EXPECT_EQ(shape_ptr->x(), 0.0);
  EXPECT_EQ(shape_ptr->y(), 0.0);

  m_node_item->SetX(10.0);
  m_node_item->SetY(20.0);

  EXPECT_EQ(shape_ptr->x(), 10.0);
  EXPECT_EQ(shape_ptr->y(), 20.0);
  EXPECT_EQ(m_node_item->GetX(), 10.0);
  EXPECT_EQ(m_node_item->GetY(), 20.0);
}

TEST_F(ConnectableShapeExtendedTest, ChangeSceneCoordinatesAfterInsert)
{
  auto shape = CreateConnectableShape(m_node_item);
  auto shape_ptr = shape.get();

  m_scene.addItem(shape.release());

  const PropertyChangedEvent expected_event1{m_node_item, std::string{NodeItem::kXpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event1)).Times(1);
  const PropertyChangedEvent expected_event2{m_node_item, std::string{NodeItem::kYpos}};
  EXPECT_CALL(m_mock_item_listener, OnPropertyChanged(expected_event2)).Times(1);

  EXPECT_EQ(shape_ptr->x(), 0.0);
  EXPECT_EQ(shape_ptr->y(), 0.0);

  shape_ptr->setX(10.0);
  shape_ptr->setY(20.0);

  EXPECT_EQ(shape_ptr->x(), 10.0);
  EXPECT_EQ(shape_ptr->y(), 20.0);
  EXPECT_EQ(m_node_item->GetX(), 10.0);
  EXPECT_EQ(m_node_item->GetY(), 20.0);
}

TEST_F(ConnectableShapeExtendedTest, MakeChildConnectedFailure)
{
  auto shape = CreateConnectableShape(m_node_item);
  EXPECT_THROW(shape->MakeChildConnected(nullptr), RuntimeException);
  EXPECT_THROW(shape->MakeChildConnected(shape.get()), RuntimeException);

  // doesn't have necessary ports
  auto another_shape = CreateConnectableShape(m_node_item);
  EXPECT_THROW(shape->MakeChildConnected(another_shape.get()), RuntimeException);
}

TEST_F(ConnectableShapeExtendedTest, MakeSequenceAndWaitConnected)
{
  auto sequence_item = m_app_model.InsertItem<test::toyitems::SequenceItem>();
  auto wait_item = m_app_model.InsertItem<test::toyitems::WaitItem>();

  auto sequence_shape = CreateConnectableShape(sequence_item);
  auto wait_shape = CreateConnectableShape(wait_item);

  EXPECT_THROW(sequence_shape->MakeChildConnected(wait_shape.get()), RuntimeException)
      << "Should be part of the scene";

  m_scene.addItem(sequence_shape.get());
  m_scene.addItem(wait_shape.get());

  EXPECT_THROW(wait_shape->MakeChildConnected(sequence_shape.get()), RuntimeException)
      << "Cannot connect Wait to Sequence";

  auto connection = sequence_shape->MakeChildConnected(wait_shape.get());

  auto connections = GetShapes<NodeConnectionShape>();
  ASSERT_EQ(connections.size(), 1);
  EXPECT_EQ(connection, connections.at(0));

  auto node_connection = connections.front()->GetNodeConnection();
  ASSERT_EQ(node_connection->GetStartPort(),
            GetPortsByDirection(wait_shape->GetPorts(), PortDirection::kOutput).front());
  ASSERT_EQ(node_connection->GetEndPort(),
            GetPortsByDirection(sequence_shape->GetPorts(), PortDirection::kInput).front());
}

TEST_F(ConnectableShapeExtendedTest, AttemptToConnectTwoWaits)
{
  auto wait_item1 = m_app_model.InsertItem<test::toyitems::WaitItem>();
  auto wait_item2 = m_app_model.InsertItem<test::toyitems::WaitItem>();

  auto wait_shape1 = CreateConnectableShape(wait_item1);
  auto wait_shape2 = CreateConnectableShape(wait_item2);

  m_scene.addItem(wait_shape1.get());
  m_scene.addItem(wait_shape2.get());

  EXPECT_THROW(wait_shape1->MakeChildConnected(wait_shape2.get()), RuntimeException)
      << "Shape doesn't have input port";
}

}  // namespace mvvm::test
