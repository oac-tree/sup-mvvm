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

#include "mvvm/nodeeditor/painting/connectable_shape_painter.h"

#include <mvvm/nodeeditor/connectable_shape.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_connectable_shape_adapter.h>
#include <testutils/mock_port_position_strategy.h>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShapePainter class.
 */
class ConnectableShapePainterTest : public ::testing::Test
{
public:
  /**
   * @brief Returns test object wired with proper connection adapter.
   */
  std::unique_ptr<ConnectableShape> CreateConnectableShape()
  {
    auto adapter = std::make_unique<ConnectableShapeAdapterDecorator>(&m_mock_adapter);
    auto position_strategy =
        std::make_unique<PortPositionStrategyDecorator>(&m_mock_position_strategy);
    return std::make_unique<ConnectableShape>(std::move(adapter), std::move(position_strategy));
  }

  ::testing::NaggyMock<test::MockConnectableShapeAdapter> m_mock_adapter;
  ::testing::NaggyMock<test::MockPortPositionStrategy> m_mock_position_strategy;
};

TEST_F(ConnectableShapePainterTest, GetBoundaryColor)
{
  auto shape = CreateConnectableShape();
  ConnectableShapeStyle style;
  style.boundary_color = Qt::black;
  style.not_finished_boundary_color = Qt::yellow;
  style.running_boundary_color = Qt::blue;
  style.success_boundary_color = Qt::green;
  style.failed_boundary_color = Qt::red;

  const ConnectableShapePainter painter(style, *shape);

  NodeOperationStates states;
  EXPECT_EQ(painter.GetBoundaryColor(states), style.boundary_color);
  states.Set(NodeOperationState::kNotFinished);
  EXPECT_EQ(painter.GetBoundaryColor(states), style.not_finished_boundary_color);
  states.Reset();
  states.Set(NodeOperationState::kRunning);
  EXPECT_EQ(painter.GetBoundaryColor(states), style.running_boundary_color);
  states.Reset();
  states.Set(NodeOperationState::kSuccess);
  EXPECT_EQ(painter.GetBoundaryColor(states), style.success_boundary_color);
  states.Reset();
  states.Set(NodeOperationState::kFailed);
  EXPECT_EQ(painter.GetBoundaryColor(states), style.failed_boundary_color);
}

}  // namespace mvvm::test
