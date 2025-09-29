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

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_editor_helper.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/node_port_shape.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>
#include <mvvm/test/mock_item_listener.h>
#include <mvvm/utils/container_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTest>

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

TEST_F(ConnectableShapeExtendedTest, CoordinatesOfPortShapes)
{
  auto wait_item = m_app_model.InsertItem<test::toyitems::WaitItem>();
  wait_item->SetX(100.0);
  wait_item->SetY(200.0);

  auto wait_shape = CreateConnectableShape(wait_item);
  auto wait_shape_ptr = wait_shape.get();
  m_scene.addItem(wait_shape.release());
  auto port_shapes = GetShapes<NodePortShape>();
  ASSERT_EQ(port_shapes.size(), 1);

  EXPECT_EQ(wait_shape_ptr->scenePos().x(), 100.0);
  EXPECT_EQ(wait_shape_ptr->scenePos().y(), 200.0);

  // output port is located in the middle of top side of the shape
  auto output_port_shape = port_shapes.at(0);
  EXPECT_DOUBLE_EQ(output_port_shape->scenePos().x(),
                   100.0 + (wait_shape_ptr->GetShapeRect().width() / 2));
  EXPECT_DOUBLE_EQ(output_port_shape->scenePos().y(), 200.0);

  // Validating coordinates of underlying port after NodePortShapes became a child of
  // ConnectableShape which in turn was placed in the scene.
  auto output_port = output_port_shape->GetNodePort();
  auto [port_x, port_y] = output_port->GetScenePosition();
  EXPECT_DOUBLE_EQ(port_x, output_port_shape->scenePos().x());
  EXPECT_DOUBLE_EQ(port_y, output_port_shape->scenePos().y());
}

TEST_F(ConnectableShapeExtendedTest, SetSelected)
{
  auto wait_item = m_app_model.InsertItem<test::toyitems::WaitItem>();

  auto wait_shape = CreateConnectableShape(wait_item);
  auto wait_shape_ptr = wait_shape.get();
  m_scene.addItem(wait_shape.release());

  wait_shape_ptr->setSelected(true);

  auto selected_items = m_scene.selectedItems();
  ASSERT_EQ(selected_items.size(), 1);
  EXPECT_EQ(selected_items.front(), wait_shape_ptr);
}

TEST_F(ConnectableShapeExtendedTest, UndoComplexMove)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  // QTest::mouseEvent seems having problems, see https://bugreports.qt.io/browse/QTBUG-5232
  GTEST_SKIP();
#endif

  QGraphicsView graphics_view(&m_scene);
  graphics_view.setMouseTracking(true);
  graphics_view.setFocus();
  graphics_view.setSceneRect(0, 0, 800, 600);

  m_app_model.SetUndoEnabled(true);

  auto wait_item = m_app_model.InsertItem<test::toyitems::WaitItem>();

  auto wait_shape = CreateConnectableShape(wait_item);
  auto wait_shape_ptr = wait_shape.get();
  m_scene.addItem(wait_shape.release());

  EXPECT_EQ(wait_shape_ptr->x(), 0.0);
  EXPECT_EQ(wait_shape_ptr->y(), 0.0);
  EXPECT_EQ(wait_item->GetX(), 0.0);
  EXPECT_EQ(wait_item->GetY(), 0.0);

  // Out view is a rectangle with top-left-origin at (0,0). Selecting press coordinate so it is
  // inside this rectangle.
  const QPointF scene_press_coordinate{10, 10};
  const QPointF scene_move1_pos{100, 200};
  const QPointF scene_move2_pos{500, 600};

  {  // simulating drag-and-move

    const QPoint viewport_press_coordinate =
        graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_press_coordinate));

    const QPoint viewport_move1_pos =
        graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_move1_pos));

    const QPoint viewport_move2_pos =
        graphics_view.viewport()->mapFromParent(graphics_view.mapFromScene(scene_move2_pos));

    QTest::mousePress(graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      viewport_press_coordinate);

    QTest::mouseEvent(QTest::MouseMove, graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      viewport_move1_pos);
    QTest::mouseEvent(QTest::MouseMove, graphics_view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      viewport_move2_pos);

    QTest::mouseRelease(graphics_view.viewport(), Qt::LeftButton);

    QCoreApplication::processEvents();
  }

  // instruction insert, and single macro with moves
  ASSERT_EQ(m_app_model.GetCommandStack()->GetCommandCount(), 2);

  // new coordinates of item's top-left corner
  const double expected_final_item_x = scene_move2_pos.x() - scene_press_coordinate.x();
  const double expected_final_item_y = scene_move2_pos.y() - scene_press_coordinate.y();

  EXPECT_FLOAT_EQ(wait_item->GetX(), expected_final_item_x);
  EXPECT_FLOAT_EQ(wait_item->GetY(), expected_final_item_y);

  // single undo should bring it back without intermediate moves
  m_app_model.GetCommandStack()->Undo();
  EXPECT_EQ(wait_item->GetX(), 0.0);
  EXPECT_EQ(wait_item->GetY(), 0.0);

  m_app_model.GetCommandStack()->Redo();
  EXPECT_FLOAT_EQ(wait_item->GetX(), expected_final_item_x);
  EXPECT_FLOAT_EQ(wait_item->GetY(), expected_final_item_y);
}

}  // namespace mvvm::test
