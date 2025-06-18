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

#include "mvvm/serialization/session_item_composer_components.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/serialization/tree_data.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for classes from session_item_composer_components.h
 */
class SessionItemComposerComponentsTests : public ::testing::Test
{
};

TEST_F(SessionItemComposerComponentsTests, ItemComposerComponent)
{
  ItemComposerComponent component;

  EXPECT_FALSE(component.HasNextComponent());
  EXPECT_EQ(component.GetComponentCount(), 0U);

  auto child0 = std::make_unique<ItemComposerComponent>();
  auto child0_ptr = child0.get();
  auto child1 = std::make_unique<ItemComposerComponent>();
  auto child1_ptr = child1.get();

  component.AddNextComponent(std::move(child0));
  component.AddNextComponent(std::move(child1));
  EXPECT_TRUE(component.HasNextComponent());
  EXPECT_EQ(component.GetComponentCount(), 2U);

  auto taken0 = component.TakeNextComponent();
  auto taken1 = component.TakeNextComponent();
  EXPECT_EQ(taken0.get(), child0_ptr);
  EXPECT_EQ(taken1.get(), child1_ptr);
}

TEST_F(SessionItemComposerComponentsTests, ItemContainerFromTreeData)
{
  const tree_data_t tree_data("abc");
  const ItemComposerContext context{};

  SessionItem item;
  EXPECT_THROW(ItemContainerFromTreeData(context, tree_data, item, nullptr), RuntimeException);

  // more tests in session_item_composer_tests.cpp
}

TEST_F(SessionItemComposerComponentsTests, TaggedItemsFromTreeData)
{
  const tree_data_t tree_data("abc");
  const ItemComposerContext context{};

  SessionItem item;
  EXPECT_THROW(TaggedItemsFromTreeData(context, tree_data, item, nullptr), RuntimeException);

  // more tests in session_item_composer_tests.cpp
}

TEST_F(SessionItemComposerComponentsTests, SessionItemFromTreeData)
{
  const tree_data_t tree_data("abc");
  const ItemComposerContext context{};

  SessionItem item;
  EXPECT_THROW(SessionItemFromTreeData(context, tree_data, item, nullptr), RuntimeException);

  // more tests in session_item_composer_tests.cpp
}

}  // namespace mvvm::test
