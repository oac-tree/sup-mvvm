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

#include "mvvm/serialization/tree_data_item_container_converter.h"

#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/serialization/tree_data.h>

#include <gtest/gtest.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

//! Testing of TreeDataItemContainerConverter.
//! It uses simplified item constructiion.

class TreeDataItemContainerConverterTests : public ::testing::Test
{
public:
  TreeDataItemContainerConverterTests()
  {
    //! Simplified method to convert SessionItem to TreeData.
    auto to_treedata = [this](const SessionItem& item)
    {
      auto result = std::make_unique<tree_data_t>("Item");
      result->AddAttribute("model", item.GetType());
      return result;
    };
    m_create_tree_callback = to_treedata;

    //! Simplified method to convert TreeData to SessionItem.
    auto to_item = [this](const tree_data_t&) { return std::make_unique<PropertyItem>(); };
    m_create_item_callback = to_item;
  }

  create_treedata_t m_create_tree_callback;
  create_item_t m_create_item_callback;
};

//! Container with single items is converted to TreeData.

TEST_F(TreeDataItemContainerConverterTests, ContainerToTreeData)
{
  // creating container
  const TagInfo tag = TagInfo::CreatePropertyTag("thickness", PropertyItem::GetStaticType());
  SessionItemContainer container(tag);

  // inserting single property item
  auto item = std::make_unique<PropertyItem>();
  item->SetData(42);
  EXPECT_TRUE(container.InsertItem(std::move(item), 0));

  auto tree_data = ContainerConverter::ToTreeData(container, m_create_tree_callback);

  EXPECT_TRUE(ContainerConverter::IsItemContainerConvertible(*tree_data));
}

//! Container with single items is converted to TreeData and back.

TEST_F(TreeDataItemContainerConverterTests, ContainerToTreeDataAndBack)
{
  // creating container
  const TagInfo tag = TagInfo::CreatePropertyTag("thickness", PropertyItem::GetStaticType());
  SessionItemContainer container(tag);

  // inserting single property item
  EXPECT_TRUE(container.InsertItem(std::make_unique<PropertyItem>(), 0));

  auto tree_data = ContainerConverter::ToTreeData(container, m_create_tree_callback);

  auto container2 = ContainerConverter::ToSessionItemContainer(*tree_data, m_create_item_callback);
  EXPECT_EQ(container2->GetItemCount(), 1);
  EXPECT_EQ(container2->GetTagInfo().GetName(), std::string("thickness"));
}
