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

#include "mvvm/serialization/treedatataggeditemsconverter.h"

#include "test_utils.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitemcontainer.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/TreeData.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing TreeDataTaggedItemsConverter.

class TreeDataTaggedItemsConverterTest : public ::testing::Test
{
public:
  TreeDataTaggedItemsConverterTest() = default;

  std::unique_ptr<TreeDataTaggedItemsConverter> createConverter() const
  {
    //! Simplified method to convert SessionItem to TreeData.
    auto to_treedata = [this](const SessionItem& item)
    {
      auto result = std::make_unique<TreeData>("Item");
      result->AddAttribute("model", item.modelType());
      return result;
    };

    //! Simplified method to convert TreeData to SessionItem.
    auto to_item = [this](const TreeData&) { return std::make_unique<PropertyItem>(); };

    ConverterCallbacks callbacks{to_treedata, to_item};
    return std::make_unique<TreeDataTaggedItemsConverter>(callbacks);
  }
};

//! TaggedItems converted to TreeData.

TEST_F(TreeDataTaggedItemsConverterTest, TaggedItemsToTreeData)
{
  TaggedItems tagged_items;
  tagged_items.RegisterTag(TagInfo::CreatePropertyTag("thickness", PropertyItem::Type), true);
  tagged_items.RegisterTag(TagInfo::CreateUniversalTag("Items"));

  tagged_items.at(0).InsertItem(new PropertyItem, 0);
  tagged_items.at(1).InsertItem(new PropertyItem, 0);
  tagged_items.at(1).InsertItem(new PropertyItem, 1);

  auto converter = createConverter();
  auto tree_data = converter->ToTreeData(tagged_items);

  EXPECT_TRUE(converter->IsTaggedItemsConvertible(*tree_data));
}

//! TaggedItems converted to TreeData.

TEST_F(TreeDataTaggedItemsConverterTest, TaggedItemsToTreeDataAndBack)
{
  TaggedItems tagged_items;
  tagged_items.RegisterTag(TagInfo::CreatePropertyTag("thickness", PropertyItem::Type), true);
  tagged_items.RegisterTag(TagInfo::CreateUniversalTag("items"));

  tagged_items.at(0).InsertItem(new PropertyItem, 0);
  tagged_items.at(1).InsertItem(new PropertyItem, 0);
  tagged_items.at(1).InsertItem(new PropertyItem, 1);

  auto converter = createConverter();
  auto tree_data = converter->ToTreeData(tagged_items);

  auto tagged_items2 = converter->ToTaggedItems(*tree_data);
  EXPECT_EQ(tagged_items2->GetDefaultTag(), "thickness");
  EXPECT_EQ(tagged_items2->at(0).GetName(), "thickness");
  EXPECT_EQ(tagged_items2->at(0).GetItemCount(), 1);
  EXPECT_EQ(tagged_items2->at(1).GetName(), "items");
  EXPECT_EQ(tagged_items2->at(1).GetItemCount(), 2);
}
