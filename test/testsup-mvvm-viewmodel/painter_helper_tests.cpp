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

#include <QDebug>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QPen>

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

TEST_F(PainterHelperTests, GetShapeFromPath)
{
  const double path_length = 100.0;
  const double pen_width = 10.0;
  const QPen pen(Qt::black, 10.0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);

  QPainterPath original_path;
  EXPECT_EQ(GetShapeFromPath(original_path, pen).length(), 0.0);

  original_path.moveTo(0.0, 0.0);
  original_path.lineTo(0.0, path_length);
  const QPainterPath shape_path = GetShapeFromPath(original_path, pen);

  const double expected_perimeter = (path_length * 2) + (pen_width * 2);
  EXPECT_DOUBLE_EQ(shape_path.length(), expected_perimeter);
}

TEST_F(PainterHelperTests, GetConnectionShape)
{
  const double path_length = 100.0;
  const double pen_width = 10.0;

  QPainterPath original_path;
  original_path.moveTo(0.0, 0.0);
  original_path.lineTo(0.0, path_length);
  const QPainterPath shape_path = GetConnectionShape(original_path, pen_width);

  const double expected_perimeter = (path_length * 2) + (pen_width * 2);
  EXPECT_DOUBLE_EQ(shape_path.length(), expected_perimeter);
}

}  // namespace mvvm::test
