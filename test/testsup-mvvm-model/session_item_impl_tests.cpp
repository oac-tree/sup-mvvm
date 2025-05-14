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

#include "mvvm/model/session_item_impl.h"

#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>

#include <gtest/gtest.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

/**
 * @brief Tests of SessionItemImpl class.
 */
class SessionItemImplTest : public ::testing::Test
{
};

TEST_F(SessionItemImplTest, Constructor)
{
  const std::string item_type("abc");

  auto [data, data_ptr] = test::CreateTestData<SessionItemData>();
  auto [tagged_items, tagged_items_ptr] = test::CreateTestData<TaggedItems>();

  SessionItemImpl item(item_type, std::move(data), std::move(tagged_items));

  EXPECT_EQ(item.GetType(), item_type);
  EXPECT_EQ(item.GetItemData(), data_ptr);
  EXPECT_EQ(item.GetTaggedItems(), tagged_items_ptr);
}

TEST_F(SessionItemImplTest, GetSlot)
{
  SessionItemImpl item("abc", std::make_unique<SessionItemData>(), std::make_unique<TaggedItems>());

  // slot created once at first call, and then do not change
  auto slot = item.GetSlot();
  EXPECT_NE(slot, nullptr);
  EXPECT_EQ(slot, item.GetSlot());
}
