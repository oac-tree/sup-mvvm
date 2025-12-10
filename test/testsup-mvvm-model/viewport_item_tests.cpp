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

#include "mvvm/standarditems/axis_items.h"
#include "mvvm/standarditems/viewport_item.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

namespace
{

class MockViewport : public ViewportItem
{
public:
  MockViewport() : ViewportItem("MockViewport") { RegisterAxes(); }

  MOCK_METHOD((std::pair<double, double>), GetDataXRangeImpl, (), (const, override));
  MOCK_METHOD((std::pair<double, double>), GetDataYRangeImpl, (), (const, override));
};

}  // namespace

class ViewportItemTests : public ::testing::Test
{
};

TEST_F(ViewportItemTests, GetDataRange)
{
  const MockViewport mock_viewport;
  const std::pair<double, double> expected_xrange{1.0, 2.0};
  const std::pair<double, double> expected_yrange{3.0, 4.0};
  EXPECT_CALL(mock_viewport, GetDataXRangeImpl()).WillOnce(::testing::Return(expected_xrange));
  EXPECT_CALL(mock_viewport, GetDataYRangeImpl()).WillOnce(::testing::Return(expected_yrange));
  EXPECT_EQ(mock_viewport.GetDataXRange(), expected_xrange);
  EXPECT_EQ(mock_viewport.GetDataYRange(), expected_yrange);
}

TEST_F(ViewportItemTests, GetViewportArea)
{
  const MockViewport mock_viewport;
  const double expected_xmin{1.0};
  const double expected_xmax{2.0};
  const double expected_ymin{3.0};
  const double expected_ymax{4.0};

  mock_viewport.GetXAxis()->SetMin(expected_xmin);
  mock_viewport.GetXAxis()->SetMax(expected_xmax);
  mock_viewport.GetYAxis()->SetMin(expected_ymin);
  mock_viewport.GetYAxis()->SetMax(expected_ymax);

  const ViewportArea area = mock_viewport.GetViewportArea();
  EXPECT_DOUBLE_EQ(area.xrange.lower, expected_xmin);
  EXPECT_DOUBLE_EQ(area.xrange.upper, expected_xmax);
  EXPECT_DOUBLE_EQ(area.yrange.lower, expected_ymin);
  EXPECT_DOUBLE_EQ(area.yrange.upper, expected_ymax);
}

TEST_F(ViewportItemTests, SetViewportArea)
{
  const MockViewport mock_viewport;
  const ViewportArea area{{1.0, 2.0}, {3.0, 4.0}};
  mock_viewport.SetViewportArea(area);
  EXPECT_DOUBLE_EQ(mock_viewport.GetXAxis()->GetMin(), area.xrange.lower);
  EXPECT_DOUBLE_EQ(mock_viewport.GetXAxis()->GetMax(), area.xrange.upper);
  EXPECT_DOUBLE_EQ(mock_viewport.GetYAxis()->GetMin(), area.yrange.lower);
  EXPECT_DOUBLE_EQ(mock_viewport.GetYAxis()->GetMax(), area.yrange.upper);
}

}  // namespace mvvm::test
