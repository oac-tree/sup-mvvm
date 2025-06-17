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

#include "mvvm/serialization/standard_tree_data_composer_components.h"
#include "mvvm/serialization/tree_data_constants.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for classes in standard_tree_data_composer_components.h header.
 */
class StandardTreeDataComposerComponentsTests : public ::testing::Test
{
};

TEST_F(StandardTreeDataComposerComponentsTests, ContainerComposerComponentWhenContainerIsEmpty)
{
  const TagInfo tag_info("tag_info", 0, 1, std::vector<std::string>());
  const SessionItemContainer container(tag_info);

  const ItemContainerComposerComponent component(container);

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kItemContainerElementType);

  ASSERT_EQ(tree_data.GetNumberOfChildren(), 1U);
  EXPECT_TRUE(tree_data.GetContent().empty());
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 0U);
  EXPECT_EQ(tree_data.Children().at(0).GetNodeName(), constants::kTagInfoElementType);

  EXPECT_FALSE(component.HasNextComponent());
}

TEST_F(StandardTreeDataComposerComponentsTests, TaggedItemsComposerComponentWithoutContainers)
{
  const std::string default_tag_name("default_tag_name");

  TaggedItems tagged_items;
  tagged_items.SetDefaultTag(default_tag_name);

  const TaggedItemsComposerComponent component(tagged_items);

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kTaggedItemsElementType);
  ASSERT_EQ(tree_data.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data.GetAttribute(constants::kDefaultTagKey), default_tag_name);
  ASSERT_EQ(tree_data.GetNumberOfChildren(), 0U);
  EXPECT_TRUE(tree_data.GetContent().empty());

  EXPECT_FALSE(component.HasNextComponent());
}

TEST_F(StandardTreeDataComposerComponentsTests, ItemComposerComponentForEmptyItem)
{
  const SessionItem item;

  const ItemComposerComponent component(item);

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kItemElementType);
  EXPECT_TRUE(tree_data.GetContent().empty());
  ASSERT_EQ(tree_data.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data.GetAttribute(constants::kTypelAttributeKey), item.GetType());

  ASSERT_EQ(tree_data.GetNumberOfChildren(), 1U);
  EXPECT_EQ(tree_data.Children().at(0).GetNodeName(), constants::kItemDataElementType);

  EXPECT_TRUE(component.HasNextComponent());
  EXPECT_EQ(component.GetQueuedComponentsCount(), 1U);
}

}  // namespace mvvm::test
