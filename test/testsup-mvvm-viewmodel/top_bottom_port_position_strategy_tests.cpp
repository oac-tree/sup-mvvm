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

#include "mvvm/nodeeditor/top_bottom_port_position_strategy.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for TopBottomPortPositionStrategy class.
 */
class TopBottomPositionStrategyTest : public ::testing::Test
{
};

TEST_F(TopBottomPositionStrategyTest, GetPortPositionFailed)
{
  const TopBottomPortPositionStrategy strategy;

  const double width = 10.0;
  const double height = 20.0;

  const QRectF rect{0.0, 0.0, width, height};

  EXPECT_THROW(strategy.GetPortPosition(PortDirection::kInput, 1, 1, rect), RuntimeException);
  EXPECT_THROW(strategy.GetPortPosition(PortDirection::kInput, 1, 0, rect), RuntimeException);

  EXPECT_EQ(strategy.GetPortPosition(PortDirection::kDefault, 0, 1, rect), QPointF());
}

TEST_F(TopBottomPositionStrategyTest, SingleInputPort)
{
  const TopBottomPortPositionStrategy strategy;

  const double width = 10.0;
  const double height = 20.0;

  const QRectF rect{0.0, 0.0, width, height};

  // port should be located at the bottom center
  const auto pos = strategy.GetPortPosition(PortDirection::kInput, 0, 1, rect);
  EXPECT_DOUBLE_EQ(pos.x(), width / 2.);
  EXPECT_DOUBLE_EQ(pos.y(), height);

  EXPECT_THROW(strategy.GetPortPosition(PortDirection::kInput, 1, 1, rect), RuntimeException);
  EXPECT_THROW(strategy.GetPortPosition(PortDirection::kInput, 1, 0, rect), RuntimeException);
}

TEST_F(TopBottomPositionStrategyTest, TwoInputPorts)
{
  const TopBottomPortPositionStrategy strategy;

  const double width = 30.0;
  const double height = 20.0;

  const QRectF rect{0.0, 0.0, width, height};

  // two ports should be evenly distributed at the bottom
  const auto pos0 = strategy.GetPortPosition(PortDirection::kInput, 0, 2, rect);
  EXPECT_DOUBLE_EQ(pos0.x(), 10.0);
  EXPECT_DOUBLE_EQ(pos0.y(), height);
  const auto pos1 = strategy.GetPortPosition(PortDirection::kInput, 1, 2, rect);
  EXPECT_DOUBLE_EQ(pos1.x(), 20.0);
  EXPECT_DOUBLE_EQ(pos1.y(), height);
}

TEST_F(TopBottomPositionStrategyTest, SingleOutputPort)
{
  const TopBottomPortPositionStrategy strategy;

  const double width = 10.0;
  const double height = 20.0;

  const QRectF rect{0.0, 0.0, width, height};

  // port should be located at the top center
  const auto pos = strategy.GetPortPosition(PortDirection::kOutput, 0, 1, rect);
  EXPECT_DOUBLE_EQ(pos.x(), width / 2.);
  EXPECT_DOUBLE_EQ(pos.y(), 0.0);
}

TEST_F(TopBottomPositionStrategyTest, TwoOutputPorts)
{
  const TopBottomPortPositionStrategy strategy;

  const double width = 30.0;
  const double height = 20.0;

  const QRectF rect{0.0, 0.0, width, height};

  // two ports should be evenly distributed at the top
  const auto pos0 = strategy.GetPortPosition(PortDirection::kOutput, 0, 2, rect);
  EXPECT_DOUBLE_EQ(pos0.x(), 10.0);
  EXPECT_DOUBLE_EQ(pos0.y(), 0.0);
  const auto pos1 = strategy.GetPortPosition(PortDirection::kOutput, 1, 2, rect);
  EXPECT_DOUBLE_EQ(pos1.x(), 20.0);
  EXPECT_DOUBLE_EQ(pos1.y(), 0.0);
}

}  // namespace mvvm::test
