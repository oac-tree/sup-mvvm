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
    std::vector<TestItem*> items;
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, constants::kFallBackMin);
    EXPECT_EQ(max, constants::kFallBackMax);
  }

  {  // single item
    TestItem item1({1.0, 2.0, 3.0});
    std::vector<TestItem*> items = {&item1};
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, 1.0);
    EXPECT_EQ(max, 3.0);
  }

  {  // multiple items
    TestItem item1({1.0, 2.0, 3.0});
    TestItem item2({-1.0, 0.0, 4.0});
    TestItem item3({5.0, 6.0});
    std::vector<TestItem*> items = {&item1, &item2, &item3};
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, -1.0);
    EXPECT_EQ(max, 6.0);
  }

  {  // multiple items, same values
    TestItem item1({1.0, 1.0, 1.0});
    TestItem item2({1.0, 1.0, 1.0});
    std::vector<TestItem*> items = {&item1, &item2};
    auto [min, max] = GetMinMax(items, &TestItem::GetValues);
    EXPECT_EQ(min, 1.0);
    EXPECT_EQ(max, 1.0);
  }
}

}  // namespace mvvm::test
