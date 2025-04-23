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

#include "mvvm/serialization/tree_data_itemdata_converter.h"

#include <mvvm/model/session_item_data.h>
#include <mvvm/serialization/tree_data_helper.h>

#include <sup/xml/tree_data_parser.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing TreeDataItemDataConverter.

class TreeDataItemDataConverterTests : public ::testing::Test
{
};

//! Parsing XML data string representing empty document.
TEST_F(TreeDataItemDataConverterTests, TwoRoles)
{
  const std::string body{
      R"(<ItemData><Variant role="0" type="int64">42</Variant><Variant role="1" type="string">abc</Variant></ItemData>)"};

  TreeDataItemDataConverter converter;
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(converter.IsSessionItemDataConvertible(*tree_data));

  auto item_data = converter.ToSessionItemData(*tree_data);
  EXPECT_EQ(item_data->GetRoles(), std::vector<int>({0, 1}));
  EXPECT_TRUE(item_data->Data(0) == variant_t(42L));
  EXPECT_TRUE(item_data->Data(1) == variant_t(std::string("abc")));

  // converting back to tree
  auto new_tree_data = converter.ToTreeData(*item_data);

  EXPECT_EQ(new_tree_data->GetContent(), tree_data->GetContent());
  EXPECT_EQ(new_tree_data->GetNodeName(), std::string("ItemData"));
  EXPECT_EQ(new_tree_data->GetNumberOfChildren(), 2);
  EXPECT_EQ(new_tree_data->GetNumberOfAttributes(), 0);
}
