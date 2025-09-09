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

  EXPECT_CALL(m_mock_adapter, GetX()).Times(1);
  EXPECT_CALL(m_mock_adapter, GetY()).Times(1);

  EXPECT_NO_THROW(CreateConnectableShape());
}

TEST_F(ConnectableShapeTest, SettingPosition)
{
  EXPECT_CALL(m_mock_adapter, GetX()).WillOnce(::testing::Return(1.0));
  EXPECT_CALL(m_mock_adapter, GetY()).WillOnce(::testing::Return(2.0));

  auto shape = CreateConnectableShape();
  EXPECT_EQ(shape->x(), 1.0);
  EXPECT_EQ(shape->y(), 2.0);
}

}  // namespace mvvm::test
