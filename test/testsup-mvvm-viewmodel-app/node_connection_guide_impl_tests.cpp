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

#include "mvvm/nodeeditor/node_connection_guide_impl.h"

#include <mvvm/nodeeditor/node_connection.h>
#include <mvvm/nodeeditor/node_connection_shape.h>
#include <mvvm/nodeeditor/node_port.h>
#include <mvvm/nodeeditor/node_port_shape.h>

#include <gtest/gtest.h>
#include <testutils/mock_node_connection_controller_context.h>
#include <testutils/test_node_editor_helper.h>

#include <QGraphicsScene>

namespace mvvm::test
{

/**
 * @brief Tests for NodeConnectionGuideImpl class.
 */
class NodeConnectionGuideImplTest : public ::testing::Test
{
public:
  std::unique_ptr<NodeConnectionGuideImpl> CreateController()
  {
    auto context = m_mock_controller_context.CreateContext();
    return std::make_unique<NodeConnectionGuideImpl>(context, &m_scene);
  }

  QGraphicsScene m_scene;
  ::testing::StrictMock<MockNodeConnectionGuideContext> m_mock_controller_context;
};

TEST_F(NodeConnectionGuideImplTest, InitialState)
{
  auto controller = CreateController();
  EXPECT_EQ(controller->GetCurrentConnection(), nullptr);
  EXPECT_EQ(controller->GetEndPortProxy(), nullptr);
}

TEST_F(NodeConnectionGuideImplTest, FindPort)
{
  auto controller = CreateController();
  const QPointF scene_pos(10.0, 20.0);
  auto port_shape = CreateNodePortShape();
  m_scene.addItem(port_shape.get());
  port_shape->setPos(scene_pos);

  EXPECT_EQ(port_shape->GetNodePort()->GetScenePosition(), std::make_pair(10.0, 20.0));

  auto found_port = controller->FindNodePortShape(scene_pos);
  EXPECT_EQ(found_port, port_shape.get());
}

TEST_F(NodeConnectionGuideImplTest, FindTwoPorts)
{
  auto controller = CreateController();

  const QPointF start_scene_pos(10.0, 20.0);
  auto start_port_shape = CreateNodePortShape(PortDirection::kInput);
  m_scene.addItem(start_port_shape.get());
  start_port_shape->setPos(start_scene_pos);

  const QPointF end_scene_pos(300.0, 400.0);
  auto end_port_shape = CreateNodePortShape(PortDirection::kOutput);
  m_scene.addItem(end_port_shape.get());
  end_port_shape->setPos(end_scene_pos);

  EXPECT_EQ(controller->FindNodePortShape(start_scene_pos), start_port_shape.get());
  EXPECT_EQ(controller->FindNodePortShape(end_scene_pos), end_port_shape.get());
}

TEST_F(NodeConnectionGuideImplTest, ProcessMousePressNotOnPort)
{
  auto controller = CreateController();
  const QPointF scene_pos(10.0, 20.0);
  EXPECT_EQ(controller->ProcessMousePress(scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasIgnored);
  EXPECT_EQ(controller->GetCurrentConnection(), nullptr);
  EXPECT_EQ(controller->GetEndPortProxy(), nullptr);
}

TEST_F(NodeConnectionGuideImplTest, ProcessMousePressOnPort)
{
  auto controller = CreateController();
  const QPointF scene_pos(10.0, 20.0);
  auto port_shape = CreateNodePortShape();
  m_scene.addItem(port_shape.get());
  port_shape->setPos(scene_pos);

  EXPECT_CALL(m_mock_controller_context, OnConnectionStarted());
  EXPECT_EQ(controller->ProcessMousePress(scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasHandled);
  EXPECT_NE(controller->GetCurrentConnection(), nullptr);
  EXPECT_NE(controller->GetEndPortProxy(), nullptr);

  auto node_connection = controller->GetCurrentConnection()->GetNodeConnection();
  EXPECT_EQ(node_connection->GetStartPort(), port_shape->GetNodePort());
  EXPECT_EQ(node_connection->GetEndPort(), controller->GetEndPortProxy());

  EXPECT_EQ(node_connection->GetStartPort()->GetScenePosition(), std::make_pair(10.0, 20.0));
  EXPECT_EQ(node_connection->GetEndPort()->GetScenePosition(), std::make_pair(10.0, 20.0));
}

TEST_F(NodeConnectionGuideImplTest, ProcessUnrelatedMouseMove)
{
  auto controller = CreateController();
  const QPointF scene_pos(10.0, 20.0);
  EXPECT_EQ(controller->ProcessMouseMove(scene_pos), EventProcessStatus::kEventWasIgnored);
  EXPECT_EQ(controller->GetCurrentConnection(), nullptr);
  EXPECT_EQ(controller->GetEndPortProxy(), nullptr);
}

TEST_F(NodeConnectionGuideImplTest, ProcessMouseMoveWithActiveConnection)
{
  auto controller = CreateController();
  const QPointF scene_pos(10.0, 20.0);
  auto port_shape = CreateNodePortShape();
  m_scene.addItem(port_shape.get());
  port_shape->setPos(scene_pos);

  EXPECT_CALL(m_mock_controller_context, OnConnectionStarted());
  EXPECT_EQ(controller->ProcessMousePress(scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasHandled);
  EXPECT_NE(controller->GetCurrentConnection(), nullptr);
  EXPECT_NE(controller->GetEndPortProxy(), nullptr);

  const QPointF new_pos(30.0, 40.0);
  EXPECT_EQ(controller->ProcessMouseMove(new_pos), EventProcessStatus::kEventWasHandled);
  EXPECT_EQ(
      controller->GetCurrentConnection()->GetNodeConnection()->GetEndPort()->GetScenePosition(),
      std::make_pair(30.0, 40.0));
}

TEST_F(NodeConnectionGuideImplTest, ProcessUnrelatedMouseRelease)
{
  auto controller = CreateController();
  const QPointF scene_pos(10.0, 20.0);
  EXPECT_EQ(controller->ProcessMouseRelease(scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasIgnored);
  EXPECT_EQ(controller->GetCurrentConnection(), nullptr);
  EXPECT_EQ(controller->GetEndPortProxy(), nullptr);
}

TEST_F(NodeConnectionGuideImplTest, ProcessMouseReleaseWithActiveConnection)
{
  auto controller = CreateController();

  const QPointF start_scene_pos(10.0, 20.0);
  auto start_port_shape = CreateNodePortShape(PortDirection::kInput);
  m_scene.addItem(start_port_shape.get());
  start_port_shape->setPos(start_scene_pos);

  const QPointF end_scene_pos(30.0, 40.0);
  auto end_port_shape = CreateNodePortShape(PortDirection::kOutput);
  m_scene.addItem(end_port_shape.get());
  end_port_shape->setPos(end_scene_pos);

  EXPECT_CALL(m_mock_controller_context, OnConnectionStarted());
  EXPECT_EQ(controller->ProcessMousePress(start_scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasHandled);

  auto node_connection = controller->GetCurrentConnection()->GetNodeConnection();
  EXPECT_EQ(node_connection->GetStartPort()->GetScenePosition(), std::make_pair(10.0, 20.0));
  EXPECT_EQ(node_connection->GetEndPort()->GetScenePosition(), std::make_pair(10.0, 20.0));

  EXPECT_CALL(m_mock_controller_context, OnConnectionFinished());
  EXPECT_CALL(m_mock_controller_context,
              OnConnectionRequest(start_port_shape->GetNodePort(), end_port_shape->GetNodePort()));

  EXPECT_EQ(controller->ProcessMouseRelease(end_scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasHandled);

  EXPECT_EQ(controller->GetCurrentConnection(), nullptr);
  EXPECT_EQ(controller->GetEndPortProxy(), nullptr);
}

TEST_F(NodeConnectionGuideImplTest, ProcessMouseReleaseWhenPortsIncompatible)
{
  auto controller = CreateController();

  const QPointF start_scene_pos(10.0, 20.0);
  auto start_port_shape = CreateNodePortShape(PortDirection::kInput);
  m_scene.addItem(start_port_shape.get());
  start_port_shape->setPos(start_scene_pos);

  const QPointF end_scene_pos(30.0, 40.0);
  auto end_port_shape = CreateNodePortShape(PortDirection::kInput);  // same direction
  m_scene.addItem(end_port_shape.get());
  end_port_shape->setPos(end_scene_pos);

  EXPECT_CALL(m_mock_controller_context, OnConnectionStarted());
  EXPECT_EQ(controller->ProcessMousePress(start_scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasHandled);

  auto node_connection = controller->GetCurrentConnection()->GetNodeConnection();
  EXPECT_EQ(node_connection->GetStartPort()->GetScenePosition(), std::make_pair(10.0, 20.0));
  EXPECT_EQ(node_connection->GetEndPort()->GetScenePosition(), std::make_pair(10.0, 20.0));

  EXPECT_CALL(m_mock_controller_context, OnConnectionFinished());
  EXPECT_CALL(m_mock_controller_context,
              OnConnectionRequest(start_port_shape->GetNodePort(), end_port_shape->GetNodePort()))
      .Times(0);

  EXPECT_EQ(controller->ProcessMouseRelease(end_scene_pos, Qt::LeftButton),
            EventProcessStatus::kEventWasHandled);

  EXPECT_EQ(controller->GetCurrentConnection(), nullptr);
  EXPECT_EQ(controller->GetEndPortProxy(), nullptr);
}

}  // namespace mvvm::test
