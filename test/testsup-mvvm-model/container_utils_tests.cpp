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

#include "mvvm/utils/container_utils.h"

#include <gtest/gtest.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

/**
 * @brief Tests for functions defined in container_utils.h
 */
class ContainerUtilsTests : public ::testing::Test
{
public:
  class TestItem
  {
  public:
    TestItem() = default;
    virtual ~TestItem() = default;
  };

  class TestItemA : public TestItem
  {
  public:
    TestItemA() = default;
  };

  class TestItemB : public TestItem
  {
  public:
    TestItemB() = default;
  };
};

TEST_F(ContainerUtilsTests, isUniquePtr)
{
  EXPECT_FALSE(utils::is_unique_ptr<int>::value);
  EXPECT_TRUE(utils::is_unique_ptr<std::unique_ptr<int>>::value);
}

TEST_F(ContainerUtilsTests, IndexOfItem)
{
  // searching in vector of integers
  const std::vector<int> vec{1, 7, 5};
  EXPECT_EQ(utils::IndexOfItem(vec, 1), 0);
  EXPECT_EQ(utils::IndexOfItem(vec, 10), -1);
  EXPECT_EQ(utils::IndexOfItem(vec, 5), 2);
  EXPECT_EQ(utils::IndexOfItem(vec.begin(), vec.end(), 7), 1);

  // searching in vector of SessionItem's
  const std::vector<TestItemA*> items{new TestItemA, new TestItemA, new TestItemA};
  const TestItemA other;
  EXPECT_EQ(utils::IndexOfItem(items, items[0]), 0);
  EXPECT_EQ(utils::IndexOfItem(items, items[1]), 1);
  EXPECT_EQ(utils::IndexOfItem(items, items[2]), 2);
  EXPECT_EQ(utils::IndexOfItem(items, &other), -1);
  for (auto item : items)
  {
    delete item;
  }

  // searching in vector of unique_ptr
  std::vector<std::unique_ptr<TestItemA>> unique_items;
  unique_items.emplace_back(std::make_unique<TestItemA>());
  unique_items.emplace_back(std::make_unique<TestItemA>());
  EXPECT_EQ(utils::IndexOfItem(unique_items, unique_items[0].get()), 0);
  EXPECT_EQ(utils::IndexOfItem(unique_items, unique_items[1].get()), 1);
  EXPECT_EQ(utils::IndexOfItem(unique_items, &other), -1);
}

TEST_F(ContainerUtilsTests, UniqueWithOrder)
{
  const std::vector<int> data = {1, 42, 1, 6, 43, 6};
  EXPECT_EQ(utils::UniqueWithOrder(data), (std::vector<int>{1, 42, 6, 43}));
}

TEST_F(ContainerUtilsTests, CastItems)
{
  TestItemA test_item_a1;
  TestItemA test_item_a2;
  TestItemB test_item_b1;

  const std::vector<TestItem*> vec{&test_item_a1, &test_item_b1, &test_item_a2};
  EXPECT_EQ(utils::CastItems<TestItemA>(vec),
            std::vector<TestItemA*>({&test_item_a1, &test_item_a2}));
  EXPECT_EQ(utils::CastItems<TestItem>(vec),
            std::vector<TestItem*>({&test_item_a1, &test_item_b1, &test_item_a2}));
  EXPECT_EQ(utils::CastItems<TestItemB>(vec), std::vector<TestItemB*>({&test_item_b1}));
}

TEST_F(ContainerUtilsTests, StaticCastItems)
{
  TestItemA test_item_a1;
  TestItemA test_item_a2;
  TestItemB test_item_b1;

  const std::vector<TestItem*> vec{&test_item_a1, &test_item_b1, &test_item_a2};
  EXPECT_EQ(utils::StaticCastItems<TestItem>(vec),
            std::vector<TestItem*>({&test_item_a1, &test_item_b1, &test_item_a2}));
}

TEST_F(ContainerUtilsTests, GetVectorOfPtrs)
{
  auto [item1, item1_ptr] = mvvm::test::CreateTestData<TestItem>();
  auto [item2, item2_ptr] = mvvm::test::CreateTestData<TestItem>();

  std::vector<std::unique_ptr<TestItem>> vec;
  vec.push_back(std::move(item1));
  vec.push_back(std::move(item2));

  EXPECT_EQ(utils::GetVectorOfPtrs(vec), std::vector<TestItem*>({item1_ptr, item2_ptr}));
}

TEST_F(ContainerUtilsTests, MakeConstRemoveConst)
{
  TestItemA test_item_a1;
  TestItemB test_item_a2;

  const std::vector<TestItem*> non_const_vect({&test_item_a1, &test_item_a2});
  const std::vector<const TestItem*> const_vect({&test_item_a1, &test_item_a2});
  EXPECT_EQ(utils::MakeConst(non_const_vect), const_vect);
  EXPECT_EQ(utils::RemoveConst(const_vect), non_const_vect);
}
