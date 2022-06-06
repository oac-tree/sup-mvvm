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

#include "mvvm/serialization/treedata_item_container_converter.h"

#include "test_utils.h"

#include "mvvm/model/property_item.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionitem_container.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/treedata.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing of TreeDataItemContainerConverter.
//! It uses simplified item constructiion.

class TreeDataItemContainerConverterTest : public ::testing::Test
{
public:
  TreeDataItemContainerConverterTest()
  {
    //! Simplified method to convert SessionItem to TreeData.
    auto to_treedata = [this](const SessionItem& item)
    {
      auto result = std::make_unique<TreeData>("Item");
      result->AddAttribute("model", item.GetType());
      return result;
    };
    m_create_tree_callback = to_treedata;

    //! Simplified method to convert TreeData to SessionItem.
    auto to_item = [this](const TreeData&) { return std::make_unique<PropertyItem>(); };
    m_create_item_callback = to_item;
  }

  create_treedata_t m_create_tree_callback;
  create_item_t m_create_item_callback;
};

//! Container with single items is converted to TreeData.

TEST_F(TreeDataItemContainerConverterTest, ContainerToTreeData)
{
  // creating container
  TagInfo tag = TagInfo::CreatePropertyTag("thickness", PropertyItem::Type);
  SessionItemContainer container(tag);

  // inserting single property item
  auto item = new PropertyItem;
  item->SetData(42);
  EXPECT_TRUE(container.InsertItem(item, 0));

  auto tree_data = ContainerConverter::ToTreeData(container, m_create_tree_callback);

  EXPECT_TRUE(ContainerConverter::IsItemContainerConvertible(*tree_data));
}

//! Container with single items is converted to TreeData and back.

TEST_F(TreeDataItemContainerConverterTest, ContainerToTreeDataAndBack)
{
  // creating container
  TagInfo tag = TagInfo::CreatePropertyTag("thickness", PropertyItem::Type);
  SessionItemContainer container(tag);

  // inserting single property item
  EXPECT_TRUE(container.InsertItem(new PropertyItem, 0));

  auto tree_data = ContainerConverter::ToTreeData(container, m_create_tree_callback);

  auto container2 = ContainerConverter::ToSessionItemContainer(*tree_data, m_create_item_callback);
  EXPECT_EQ(container2->GetItemCount(), 1);
  EXPECT_EQ(container2->GetTagInfo().GetName(), std::string("thickness"));
}
