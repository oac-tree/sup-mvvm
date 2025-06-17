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

#include "mvvm/serialization/tree_data_composer.h"

#include <mvvm/model/session_item.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_constants.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for TreeDataComposer class.
 */
class TreeDataComposerTests : public ::testing::Test
{
};

TEST_F(TreeDataComposerTests, SingleItem)
{
  const SessionItem item;

  const TreeDataComposer composer({});

  const auto item_tree_data = composer.CreateTreeData(item);

  EXPECT_EQ(item_tree_data->GetNodeName(), constants::kItemElementType);
  EXPECT_TRUE(item_tree_data->GetContent().empty());
  ASSERT_EQ(item_tree_data->GetNumberOfAttributes(), 1U);
  EXPECT_EQ(item_tree_data->GetAttribute(constants::kTypelAttributeKey), item.GetType());
  ASSERT_EQ(item_tree_data->GetNumberOfChildren(), 2U);

  const auto data_tree = item_tree_data->Children().at(0);
  EXPECT_EQ(data_tree.GetNodeName(), constants::kItemDataElementType);

  const auto tagged_items_data_tree = item_tree_data->Children().at(1);
  EXPECT_EQ(tagged_items_data_tree.GetNodeName(), constants::kTaggedItemsElementType);
}

}  // namespace mvvm::test
