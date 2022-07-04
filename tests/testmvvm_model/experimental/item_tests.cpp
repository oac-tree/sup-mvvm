/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/experimental/item.h"

#include <gtest/gtest.h>

using namespace mvvm::experimental;

//! Testing Item.

class ItemTests : public ::testing::Test
{
public:
  std::pair<std::unique_ptr<Item>, Item*> CreateTestData()
  {
    auto item = std::make_unique<Item>();
    auto item_ptr = item.get();
    return {std::move(item), item_ptr};
  }
};

TEST_F(ItemTests, InitialState)
{
  Item item;
  EXPECT_EQ(item.GetItemCount(), 0);
  EXPECT_EQ(item.GetItem(0), nullptr);
  EXPECT_EQ(item.GetModel(), nullptr);
  EXPECT_EQ(item.GetParent(), nullptr);
}

TEST_F(ItemTests, SetData)
{
  Item item;

  EXPECT_TRUE(item.SetData(42));
  EXPECT_FALSE(item.SetData(42));
  EXPECT_EQ(item.GetData(), variant_t(42));
}

TEST_F(ItemTests, InsertItem)
{
  auto [child, child_ptr] = CreateTestData();

  Item item;
  auto inserted = item.InsertItem(std::move(child), 0);

  EXPECT_EQ(inserted, child_ptr);
  EXPECT_EQ(item.GetItemCount(), 1);
  EXPECT_EQ(item.GetItem(0), child_ptr);
}
