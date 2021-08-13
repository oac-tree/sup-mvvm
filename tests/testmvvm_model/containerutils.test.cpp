/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/containerutils.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests of container utils.

class ContainerUtilsTest : public ::testing::Test
{
public:
  class TestItem
  {
  public:
    TestItem() = default;
  };
};

TEST_F(ContainerUtilsTest, isUniquePtr)
{
  EXPECT_FALSE(Utils::is_unique_ptr<int>::value);
  EXPECT_TRUE(Utils::is_unique_ptr<std::unique_ptr<int>>::value);
}

TEST_F(ContainerUtilsTest, IndexOfItem)
{
  // searching in vector of integers
  std::vector<int> vv{1, 7, 5};
  EXPECT_EQ(Utils::IndexOfItem(vv, 1), 0);
  EXPECT_EQ(Utils::IndexOfItem(vv, 10), -1);
  EXPECT_EQ(Utils::IndexOfItem(vv, 5), 2);
  EXPECT_EQ(Utils::IndexOfItem(vv.begin(), vv.end(), 7), 1);

  // searching in vector of SessionItem's
  std::vector<TestItem*> items{new TestItem, new TestItem, new TestItem};
  TestItem other;
  EXPECT_EQ(Utils::IndexOfItem(items, items[0]), 0);
  EXPECT_EQ(Utils::IndexOfItem(items, items[1]), 1);
  EXPECT_EQ(Utils::IndexOfItem(items, items[2]), 2);
  EXPECT_EQ(Utils::IndexOfItem(items, &other), -1);
  for (auto x : items) delete x;

  // searching in vector of unique_ptr
  std::vector<std::unique_ptr<TestItem>> unique_items;
  unique_items.emplace_back(std::make_unique<TestItem>());
  unique_items.emplace_back(std::make_unique<TestItem>());
  EXPECT_EQ(Utils::IndexOfItem(unique_items, unique_items[0].get()), 0);
  EXPECT_EQ(Utils::IndexOfItem(unique_items, unique_items[1].get()), 1);
  EXPECT_EQ(Utils::IndexOfItem(unique_items, &other), -1);
}

TEST_F(ContainerUtilsTest, UniqueWithOrder)
{
  std::vector<int> data = {1, 42, 1, 6, 43, 6};
  EXPECT_EQ(Utils::UniqueWithOrder(data), (std::vector<int>{1, 42, 6, 43}));
}

TEST_F(ContainerUtilsTest, Contains)
{
  std::vector<int> data = {1, 42, 1, 6, 43, 6};
  EXPECT_TRUE(Utils::Contains(data, 42));
  EXPECT_FALSE(Utils::Contains(data, 99));
}
