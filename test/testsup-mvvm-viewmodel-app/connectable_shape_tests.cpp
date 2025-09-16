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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_connectable_shape_adapter.h>
#include <testutils/mock_port_position_strategy.h>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShape class.
 */
class ConnectableShapeTest : public ::testing::Test
{
public:
  /**
   * @brief Returns test object wired with proper connection adapter.
   */
  std::unique_ptr<ConnectableShape> CreateConnectableShape()
  {
    auto adapter = std::make_unique<ConnectableShapeDecorator>(&m_mock_adapter);
    auto position_strategy =
        std::make_unique<PortPositionStrategyDecorator>(&m_mock_position_strategy);
    return std::make_unique<ConnectableShape>(std::move(adapter), std::move(position_strategy));
  }

  ::testing::StrictMock<test::MockConnectableShapeAdapter> m_mock_adapter;
  ::testing::StrictMock<test::MockPortPositionStrategy> m_mock_position_strategy;
};

TEST_F(ConnectableShapeTest, InvalidInitialization)
{
  EXPECT_THROW(ConnectableShape({}, {}), RuntimeException);
  EXPECT_THROW(ConnectableShape(std::make_unique<ConnectableShapeDecorator>(&m_mock_adapter), {}),
               RuntimeException);

  EXPECT_CALL(m_mock_adapter, GetItem()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetX()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetY()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetPortInfos()).Times(1);

  EXPECT_NO_THROW(CreateConnectableShape());
}

TEST_F(ConnectableShapeTest, GetPorts)
{
  EXPECT_CALL(m_mock_adapter, GetItem()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetX()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetY()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetPortInfos()).Times(1);

  auto shape = CreateConnectableShape();
  EXPECT_TRUE(shape->GetPorts().empty());
}

TEST_F(ConnectableShapeTest, SetupPorts)
{
  const std::vector<PortInfo> input_ports = {
      PortInfo(PortDirection::kInput, "port_type", "label1")};
  const std::vector<PortInfo> output_ports = {
      PortInfo(PortDirection::kOutput, "port_type", "label2")};

  std::vector<PortInfo> all_ports = input_ports;
  all_ports.insert(all_ports.end(), output_ports.begin(), output_ports.end());

  ON_CALL(m_mock_position_strategy, GetNodeTreeLayout())
      .WillByDefault(::testing::Return(NodeTreeLayout::kTopToBottom));

  EXPECT_CALL(m_mock_position_strategy, GetNodeTreeLayout()).Times(::testing::AtLeast(1));
  EXPECT_CALL(m_mock_adapter, GetItem()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetX()).WillOnce(::testing::Return(0.0));
  EXPECT_CALL(m_mock_adapter, GetY()).WillOnce(::testing::Return(0.0));
  EXPECT_CALL(m_mock_adapter, GetPortInfos()).WillOnce(::testing::Return(all_ports));

  // Expect creating input ports
  for (std::size_t i = 0; i < input_ports.size(); ++i)
  {
    EXPECT_CALL(m_mock_position_strategy,
                GetPortPosition(PortDirection::kInput, i, input_ports.size(), ::testing::_))
        .WillOnce(::testing::Return(QPointF{static_cast<double>(i), 1.0}));
  }

  // Expect creating output ports
  for (std::size_t i = 0; i < output_ports.size(); ++i)
  {
    EXPECT_CALL(m_mock_position_strategy,
                GetPortPosition(PortDirection::kOutput, i, output_ports.size(), ::testing::_))
        .WillOnce(::testing::Return(QPointF{static_cast<double>(i), -1.0}));
  }

  auto view = CreateConnectableShape();
  auto child_items = view->childItems();
  ASSERT_EQ(child_items.size(), all_ports.size());

  EXPECT_DOUBLE_EQ(child_items[0]->pos().x(), 0.0);
  EXPECT_DOUBLE_EQ(child_items[0]->pos().y(), 1.0);
  EXPECT_DOUBLE_EQ(child_items[1]->pos().x(), 0.0);
  EXPECT_DOUBLE_EQ(child_items[1]->pos().y(), -1.0);
}

TEST_F(ConnectableShapeTest, SetViewInitialCoordinatesFromItem)
{
  EXPECT_CALL(m_mock_adapter, GetItem()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetX()).WillOnce(::testing::Return(1.0));
  EXPECT_CALL(m_mock_adapter, GetY()).WillOnce(::testing::Return(2.0));
  EXPECT_CALL(m_mock_adapter, GetPortInfos()).Times(1);

  auto view = CreateConnectableShape();
  EXPECT_EQ(view->x(), 1.0);
  EXPECT_EQ(view->y(), 2.0);
}

}  // namespace mvvm::test
