/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/nodeeditor/painting/node_connection_painter.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for ConnectableShapePainter class.
 */
class NodeConnectionPainterTest : public ::testing::Test
{
};

TEST_F(NodeConnectionPainterTest, GetBaseColor)
{
  NodeConnectionStyle style;
  style.base_color = Qt::black;
  style.not_finished_color = Qt::yellow;
  style.running_color = Qt::blue;
  style.success_color = Qt::green;
  style.failed_color = Qt::red;

  const NodeConnectionPainter painter(style);

  NodeOperationStates states;
  EXPECT_EQ(painter.GetBaseColor(states), style.base_color);
  states.Set(NodeOperationState::kNotFinished);
  EXPECT_EQ(painter.GetBaseColor(states), style.not_finished_color);
  states.Reset();
  states.Set(NodeOperationState::kRunning);
  EXPECT_EQ(painter.GetBaseColor(states), style.running_color);
  states.Reset();
  states.Set(NodeOperationState::kSuccess);
  EXPECT_EQ(painter.GetBaseColor(states), style.success_color);
  states.Reset();
  states.Set(NodeOperationState::kFailure);
  EXPECT_EQ(painter.GetBaseColor(states), style.failed_color);
}

}  // namespace mvvm::test
