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

#include "mvvm/standarditems/viewport_helper.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

namespace
{

/**
 * @brief The TestItem class provides a method to return values.
 *
 * Used to test templated GetMinMax function.
 */
class TestItem
{
public:
  TestItem(std::vector<double> values) : m_values(std::move(values)) {}
  std::vector<double> GetValues() { return m_values; }
  std::vector<double> m_values;
};

}  // namespace

/**
 * @brief Tests for helper methods from viewport_helper.h
 */
class ViewportHelperTests : public ::testing::Test
{
public:
};

TEST_F(ViewportHelperTests, GetMinMax)
{
  {  // empty values
    const std::vector<TestItem*> items;
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, constants::kFallBackMin);
    EXPECT_EQ(max, constants::kFallBackMax);
  }

  {  // single item
    TestItem item1({1.0, 2.0, 3.0});
    const std::vector<TestItem*> items = {&item1};
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, 1.0);
    EXPECT_EQ(max, 3.0);
  }

  {  // multiple items
    TestItem item1({1.0, 2.0, 3.0});
    TestItem item2({-1.0, 0.0, 4.0});
    TestItem item3({5.0, 6.0});
    const std::vector<TestItem*> items = {&item1, &item2, &item3};
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, -1.0);
    EXPECT_EQ(max, 6.0);
  }

  {  // multiple items, same values
    TestItem item1({1.0, 1.0, 1.0});
    TestItem item2({1.0, 1.0, 1.0});
    const std::vector<TestItem*> items = {&item1, &item2};
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, 1.0);
    EXPECT_EQ(max, 1.0);
  }
}

TEST_F(ViewportHelperTests, ExpandRelativeRange)
{
  {  // positive margins
    const AxisRange range{10.0, 20.0};
    const double lower_margin = 0.1;
    const double upper_margin = 0.2;
    const AxisRange expanded_range = ExpandRelativeRange(range, lower_margin, upper_margin);
    EXPECT_DOUBLE_EQ(expanded_range.lower, 9.0);
    EXPECT_DOUBLE_EQ(expanded_range.upper, 22.0);
  }

  {  // negative margins
    const AxisRange range{10.0, 20.0};
    const double lower_margin = -0.1;
    const double upper_margin = -0.2;
    const AxisRange expanded_range = ExpandRelativeRange(range, lower_margin, upper_margin);
    EXPECT_DOUBLE_EQ(expanded_range.lower, 11.0);
    EXPECT_DOUBLE_EQ(expanded_range.upper, 18.0);
  }
}

TEST_F(ViewportHelperTests, ApplyRelativeMargins)
{
  {  // apply margins
    const ViewportArea area{{10.0, 20.0}, {40.0, 50.0}};
    const ViewportMargins margins{0.1, 0.2, 0.2, 0.1};  // left, top, right, bottom
    const ViewportArea expanded_area = ApplyRelativeMargins(area, margins);
    EXPECT_DOUBLE_EQ(expanded_area.xrange.lower, 9.0);
    EXPECT_DOUBLE_EQ(expanded_area.xrange.upper, 22.0);
    EXPECT_DOUBLE_EQ(expanded_area.yrange.lower, 39.0);
    EXPECT_DOUBLE_EQ(expanded_area.yrange.upper, 52.0);
  }

  {  // apply zero margins
    const ViewportArea area{{10.0, 20.0}, {40.0, 50.0}};
    const ViewportMargins margins{0.0, 0.0, 0.0, 0.0};  // left, top, right, bottom
    const ViewportArea expanded_area = ApplyRelativeMargins(area, margins);
    EXPECT_DOUBLE_EQ(expanded_area.xrange.lower, area.xrange.lower);
    EXPECT_DOUBLE_EQ(expanded_area.xrange.upper, area.xrange.upper);
    EXPECT_DOUBLE_EQ(expanded_area.yrange.lower, area.yrange.lower);
    EXPECT_DOUBLE_EQ(expanded_area.yrange.upper, area.yrange.upper);
  }
}

TEST_F(ViewportHelperTests, ApplySafetyMargins)
{
  {  // apply safety margins
    const ViewportArea area{{10.0, 10.00001}, {20.0, 20.00001}};
    const double min_delta = 1e-5;
    const double safety_margin = 0.5;
    const ViewportArea safe_area = ApplySafetyMargins(area, min_delta, safety_margin);
    EXPECT_DOUBLE_EQ(safe_area.xrange.lower, 9.5);
    EXPECT_DOUBLE_EQ(safe_area.xrange.upper, 10.50001);
    EXPECT_DOUBLE_EQ(safe_area.yrange.lower, 19.5);
    EXPECT_DOUBLE_EQ(safe_area.yrange.upper, 20.50001);
  }

  {  // no need to apply safety margins
    const ViewportArea area{{10.0, 11.0}, {20.0, 21.0}};
    const double min_delta = 1e-5;
    const double safety_margin = 0.5;
    const ViewportArea safe_area = ApplySafetyMargins(area, min_delta, safety_margin);
    EXPECT_DOUBLE_EQ(safe_area.xrange.lower, area.xrange.lower);
    EXPECT_DOUBLE_EQ(safe_area.xrange.upper, area.xrange.upper);
    EXPECT_DOUBLE_EQ(safe_area.yrange.lower, area.yrange.lower);
    EXPECT_DOUBLE_EQ(safe_area.yrange.upper, area.yrange.upper);
  }
}

}  // namespace mvvm::test
