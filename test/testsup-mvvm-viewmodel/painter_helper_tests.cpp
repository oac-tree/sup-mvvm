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

#include "mvvm/nodeeditor/painter_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/nodeeditor/node_connection.h>
#include <mvvm/style/mvvm_style_helper.h>
#include <mvvm/utils/numeric_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for helper methods from painter_helper.h
 */
class PainterHelperTests : public ::testing::Test
{
};

TEST_F(PainterHelperTests, ScalePenWidth)
{
  // In unit tests in the absence of QApplication, this should always succeed
  ASSERT_DOUBLE_EQ(style::ScaleToSystem(1.0), 1.0);

  EXPECT_DOUBLE_EQ(ScalePenWidth(-1.0), 1.0);
  EXPECT_DOUBLE_EQ(ScalePenWidth(0.0), 1.0);
  EXPECT_DOUBLE_EQ(ScalePenWidth(0.5), 1.0);
  EXPECT_DOUBLE_EQ(ScalePenWidth(1.0), 1.0);
  EXPECT_DOUBLE_EQ(ScalePenWidth(1.25), 1.25);
  EXPECT_DOUBLE_EQ(ScalePenWidth(1.5), 1.5);
  EXPECT_DOUBLE_EQ(ScalePenWidth(1.75), 1.75);
  EXPECT_DOUBLE_EQ(ScalePenWidth(2.0), 2.0);
}

TEST_F(PainterHelperTests, ScaleAndRoundPenWidth)
{
  // In unit tests in the absence of QApplication, this should always succeed
  ASSERT_DOUBLE_EQ(style::ScaleToSystem(1.0), 1.0);

  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(-1.0), 1.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(0.0), 1.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(0.5), 1.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(1.0), 1.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(1.25), 1.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(1.5), 2.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(1.75), 2.0);
  EXPECT_DOUBLE_EQ(ScaleAndRoundPenWidth(2.0), 2.0);
}

}  // namespace mvvm::test
