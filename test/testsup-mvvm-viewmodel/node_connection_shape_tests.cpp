/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/nodeeditor/node_connection_shape.h"

#include <gtest/gtest.h>
#include <testutils/mock_node_connection.h>
#include <testutils/mock_node_port.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodeConnectionShape class.
 */
class NodeConnectionShapeTest : public ::testing::Test
{
public:
  std::unique_ptr<NodeConnectionShape> CreateNodePortShape()
  {
    auto node_connection = std::make_unique<NodeConnectionDecorator>(&m_mock_node_connection);
    return std::make_unique<NodeConnectionShape>(NodeTreeLayout::kTopToBottom,
                                                 std::move(node_connection));
  }

  ::testing::StrictMock<MockNodeConnection> m_mock_node_connection;
  ::testing::StrictMock<MockNodePort> m_mock_start_port;
  ::testing::StrictMock<MockNodePort> m_mock_end_port;
};

TEST_F(NodeConnectionShapeTest, InitialState)
{
  EXPECT_CALL(m_mock_node_connection, SetNotifyShapeCallback(::testing::_)).Times(1);

  auto node_connection_shape = CreateNodePortShape();
  EXPECT_NE(node_connection_shape->GetNodeConnection(), nullptr);
}

TEST_F(NodeConnectionShapeTest, UpdatePathWithoutPorts)
{
  EXPECT_CALL(m_mock_node_connection, SetNotifyShapeCallback(::testing::_)).Times(1);
  EXPECT_CALL(m_mock_node_connection, GetStartPort()).Times(1).WillOnce(::testing::Return(nullptr));
  EXPECT_CALL(m_mock_node_connection, GetEndPort()).Times(1).WillOnce(::testing::Return(nullptr));

  auto node_connection_shape = CreateNodePortShape();
  node_connection_shape->UpdatePath();
}

TEST_F(NodeConnectionShapeTest, UpdatePathWithPorts)
{
  EXPECT_CALL(m_mock_node_connection, SetNotifyShapeCallback(::testing::_)).Times(1);
  EXPECT_CALL(m_mock_node_connection, GetStartPort())
      .Times(1)
      .WillOnce(::testing::Return(&m_mock_start_port));
  EXPECT_CALL(m_mock_node_connection, GetEndPort())
      .Times(1)
      .WillOnce(::testing::Return(&m_mock_end_port));

  EXPECT_CALL(m_mock_start_port, GetScenePosition())
      .Times(1)
      .WillOnce(::testing::Return(std::make_pair(100.0, 100.0)));
  EXPECT_CALL(m_mock_start_port, GetPortDirection())
      .Times(1)
      .WillOnce(::testing::Return(PortDirection::kOutput));
  EXPECT_CALL(m_mock_end_port, GetScenePosition())
      .Times(1)
      .WillOnce(::testing::Return(std::make_pair(300.0, 300.0)));

  auto node_connection_shape = CreateNodePortShape();
  node_connection_shape->UpdatePath();

  EXPECT_EQ(node_connection_shape->path().elementCount(), 4);  // MoveTo + 3x CurveTo;
  EXPECT_EQ(node_connection_shape->path().currentPosition(), QPointF(300.0, 300.0));
}

}  // namespace mvvm::test
