/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <mvvm/model/compound_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_constants.h>
#include <mvvm/serialization/tree_data_helper.h>

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
  EXPECT_EQ(item_tree_data->GetAttribute(constants::kTypeAttributeKey), item.GetType());
  ASSERT_EQ(item_tree_data->GetNumberOfChildren(), 2U);

  const auto data_tree = item_tree_data->Children().at(0);
  EXPECT_EQ(data_tree.GetNodeName(), constants::kItemDataElementType);
  ASSERT_EQ(data_tree.GetNumberOfChildren(), 1U);  // identifier

  const auto tagged_items_data_tree = item_tree_data->Children().at(1);
  EXPECT_TRUE(IsTaggedItemsConvertible(tagged_items_data_tree));
  EXPECT_EQ(tagged_items_data_tree.GetNodeName(), constants::kTaggedItemsElementType);
  ASSERT_EQ(tagged_items_data_tree.GetNumberOfChildren(), 0U);
}

TEST_F(TreeDataComposerTests, ParentWithChild)
{
  CompoundItem item;
  auto& property = item.AddProperty("width", 42);

  const TreeDataComposer composer({});

  const auto item_tree_data = composer.CreateTreeData(item);

  EXPECT_EQ(item_tree_data->GetNodeName(), constants::kItemElementType);
  EXPECT_TRUE(item_tree_data->GetContent().empty());
  ASSERT_EQ(item_tree_data->GetNumberOfAttributes(), 1U);
  EXPECT_EQ(item_tree_data->GetAttribute(constants::kTypeAttributeKey), item.GetType());
  ASSERT_EQ(item_tree_data->GetNumberOfChildren(), 2U);

  const auto data_tree = item_tree_data->Children().at(0);
  EXPECT_EQ(data_tree.GetNodeName(), constants::kItemDataElementType);
  ASSERT_EQ(data_tree.GetNumberOfChildren(), 1U);  // identifier

  const auto tagged_items_data_tree = item_tree_data->Children().at(1);
  EXPECT_TRUE(IsTaggedItemsConvertible(tagged_items_data_tree));
  EXPECT_EQ(tagged_items_data_tree.GetNodeName(), constants::kTaggedItemsElementType);
  EXPECT_EQ(tagged_items_data_tree.GetAttribute(constants::kDefaultTagKey), std::string());
  ASSERT_EQ(tagged_items_data_tree.GetNumberOfChildren(), 1U);

  const auto container_data_tree = tagged_items_data_tree.Children().at(0);
  EXPECT_TRUE(IsItemContainerConvertible(container_data_tree));
  EXPECT_EQ(container_data_tree.GetNodeName(), constants::kItemContainerElementType);
  ASSERT_EQ(container_data_tree.GetNumberOfChildren(), 2U);

  const auto taginfo_data_tree = container_data_tree.Children().at(0);
  EXPECT_EQ(taginfo_data_tree.GetNodeName(), constants::kTagInfoElementType);

  const auto child_item_data_tree = container_data_tree.Children().at(1);
  EXPECT_EQ(child_item_data_tree.GetNodeName(), constants::kItemElementType);
}

TEST_F(TreeDataComposerTests, ParentWithChildFiltered)
{
  CompoundItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = item.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());

  // going to filter out child item
  auto filter = [child](auto& item) { return &item != child; };

  const TreeDataComposer composer(filter);

  const auto item_tree_data = composer.CreateTreeData(item);

  EXPECT_EQ(item_tree_data->GetNodeName(), constants::kItemElementType);
  EXPECT_TRUE(item_tree_data->GetContent().empty());
  ASSERT_EQ(item_tree_data->GetNumberOfAttributes(), 1U);
  EXPECT_EQ(item_tree_data->GetAttribute(constants::kTypeAttributeKey), item.GetType());
  ASSERT_EQ(item_tree_data->GetNumberOfChildren(), 2U);

  const auto data_tree = item_tree_data->Children().at(0);
  EXPECT_EQ(data_tree.GetNodeName(), constants::kItemDataElementType);
  ASSERT_EQ(data_tree.GetNumberOfChildren(), 1U);  // identifier

  const auto tagged_items_data_tree = item_tree_data->Children().at(1);
  EXPECT_TRUE(IsTaggedItemsConvertible(tagged_items_data_tree));
  EXPECT_EQ(tagged_items_data_tree.GetNodeName(), constants::kTaggedItemsElementType);
  EXPECT_EQ(tagged_items_data_tree.GetAttribute(constants::kDefaultTagKey), std::string("tag"));
  ASSERT_EQ(tagged_items_data_tree.GetNumberOfChildren(), 1U);

  const auto container_data_tree = tagged_items_data_tree.Children().at(0);
  EXPECT_TRUE(IsItemContainerConvertible(container_data_tree));
  EXPECT_EQ(container_data_tree.GetNodeName(), constants::kItemContainerElementType);
  ASSERT_EQ(container_data_tree.GetNumberOfChildren(), 1U);  // no child was copied

  const auto taginfo_data_tree = container_data_tree.Children().at(0);
  EXPECT_EQ(taginfo_data_tree.GetNodeName(), constants::kTagInfoElementType);
}

}  // namespace mvvm::test
