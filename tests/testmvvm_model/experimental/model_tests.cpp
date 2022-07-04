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
#include "mvvm/experimental/model.h"

#include <gtest/gtest.h>

using namespace mvvm::experimental;

//! Testing Item.

class ModelTests : public ::testing::Test
{
public:
  std::pair<std::unique_ptr<Item>, Item*> CreateTestData()
  {
    auto item = std::make_unique<Item>();
    auto item_ptr = item.get();
    return {std::move(item), item_ptr};
  }
};

TEST_F(ModelTests, InitialState)
{
  Model model;
  EXPECT_NE(model.GetRootItem(), nullptr);
}

TEST_F(ModelTests, InsertItem)
{
  auto [child, child_ptr] = CreateTestData();

  Model model;
  auto inserted = model.InsertItem(std::move(child), model.GetRootItem(), 0);

  EXPECT_EQ(inserted, child_ptr);
  EXPECT_EQ(model.GetRootItem()->GetItemCount(), 1);
  EXPECT_EQ(model.GetRootItem()->GetItem(0), child_ptr);
}
