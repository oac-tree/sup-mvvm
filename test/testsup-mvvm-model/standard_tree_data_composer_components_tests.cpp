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

#include <mvvm/model/compound_item.h>
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

  const ItemContainerComposerComponent component(container, {});

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kItemContainerElementType);

  ASSERT_EQ(tree_data.GetNumberOfChildren(), 1U);
  EXPECT_TRUE(tree_data.GetContent().empty());
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 0U);
  EXPECT_EQ(tree_data.Children().at(0).GetNodeName(), constants::kTagInfoElementType);

  EXPECT_FALSE(component.HasNextComponent());
}

TEST_F(StandardTreeDataComposerComponentsTests, ContainerComposerComponentWithTwoItems)
{
  const TagInfo tag_info("tag_info", 0, 2, std::vector<std::string>());
  SessionItemContainer container(tag_info);

  auto item0 = std::make_unique<CompoundItem>();
  auto item0_ptr = item0.get();
  container.InsertItem(std::move(item0), 0);
  auto item1 = std::make_unique<SessionItem>();
  auto item1_ptr = item1.get();
  container.InsertItem(std::move(item1), 1);

  ItemContainerComposerComponent component(container, {});

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kItemContainerElementType);

  ASSERT_EQ(tree_data.GetNumberOfChildren(), 1U);
  EXPECT_TRUE(tree_data.GetContent().empty());
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 0U);
  EXPECT_EQ(tree_data.Children().at(0).GetNodeName(), constants::kTagInfoElementType);

  EXPECT_TRUE(component.HasNextComponent());
  ASSERT_EQ(component.GetQueuedComponentsCount(), 2U);

  auto taken_component0 = component.TakeNextComponent();
  const auto tree_data0 = taken_component0->GetTreeData();
  EXPECT_EQ(tree_data0.GetNodeName(), constants::kItemElementType);
  EXPECT_TRUE(tree_data0.GetContent().empty());
  ASSERT_EQ(tree_data0.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data0.GetAttribute(constants::kTypelAttributeKey), item0_ptr->GetType());

  auto taken_component1 = component.TakeNextComponent();
  const auto tree_data1 = taken_component1->GetTreeData();
  EXPECT_EQ(tree_data1.GetNodeName(), constants::kItemElementType);
  EXPECT_TRUE(tree_data1.GetContent().empty());
  ASSERT_EQ(tree_data1.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data1.GetAttribute(constants::kTypelAttributeKey), item1_ptr->GetType());
}

TEST_F(StandardTreeDataComposerComponentsTests, TaggedItemsComposerComponentWithoutContainers)
{
  const std::string default_tag_name("default_tag_name");

  TaggedItems tagged_items;
  tagged_items.SetDefaultTag(default_tag_name);

  const TaggedItemsComposerComponent component(tagged_items, {});

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kTaggedItemsElementType);
  ASSERT_EQ(tree_data.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data.GetAttribute(constants::kDefaultTagKey), default_tag_name);
  ASSERT_EQ(tree_data.GetNumberOfChildren(), 0U);
  EXPECT_TRUE(tree_data.GetContent().empty());

  EXPECT_FALSE(component.HasNextComponent());
}

TEST_F(StandardTreeDataComposerComponentsTests, TaggedItemsComposerComponentWithContainers)
{
  const std::string default_tag_name("default_tag_name");
  const TagInfo tag_info("tag_info", 0, 1, std::vector<std::string>());

  TaggedItems tagged_items;
  tagged_items.SetDefaultTag(default_tag_name);
  tagged_items.RegisterTag(tag_info);

  TaggedItemsComposerComponent component(tagged_items, {});

  const auto tree_data = component.GetTreeData();
  EXPECT_EQ(tree_data.GetNodeName(), constants::kTaggedItemsElementType);
  ASSERT_EQ(tree_data.GetNumberOfAttributes(), 1U);
  EXPECT_EQ(tree_data.GetAttribute(constants::kDefaultTagKey), default_tag_name);
  ASSERT_EQ(tree_data.GetNumberOfChildren(), 0U);
  EXPECT_TRUE(tree_data.GetContent().empty());

  EXPECT_TRUE(component.HasNextComponent());
  EXPECT_EQ(component.GetQueuedComponentsCount(), 1U);

  auto container_component = component.TakeNextComponent();
  const auto container_tree_data = container_component->GetTreeData();
  EXPECT_EQ(container_tree_data.GetNodeName(), constants::kItemContainerElementType);

  ASSERT_EQ(container_tree_data.GetNumberOfChildren(), 1U);
  EXPECT_TRUE(container_tree_data.GetContent().empty());
  EXPECT_EQ(container_tree_data.GetNumberOfAttributes(), 0U);
  EXPECT_EQ(container_tree_data.Children().at(0).GetNodeName(), constants::kTagInfoElementType);

  EXPECT_FALSE(container_component->HasNextComponent());
}

TEST_F(StandardTreeDataComposerComponentsTests, ItemComposerComponentForEmptyItem)
{
  const SessionItem item;

  const ItemComposerComponent component(item, {});

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
