/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#include "mvvm/serialization/treedataitemdataconverter.h"

#include "test_utils.h"

#include "mvvm/model/sessionitemdata.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/xmlparseutils.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing TreeDataItemDataConverter.

class TreeDataItemDataConverterTest : public ::testing::Test
{
};

//! Parsing XML data string representing empty document.

TEST_F(TreeDataItemDataConverterTest, TwoRoles)
{
  const std::string body{
      R"(<ItemData><Variant role="0" type="int">42</Variant><Variant role="1" type="string">abc</Variant></ItemData>)"};

  TreeDataItemDataConverter converter;
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(converter.IsSessionItemDataConvertible(*tree_data));

  auto item_data = converter.ToSessionItemData(*tree_data);
  EXPECT_EQ(item_data->GetRoles(), std::vector<int>({0, 1}));
  EXPECT_TRUE(item_data->Data(0) == variant_t(42));
  EXPECT_TRUE(item_data->Data(1) == variant_t(std::string("abc")));

  // converting back to tree
  auto new_tree_data = converter.ToTreeData(*item_data);

  EXPECT_EQ(new_tree_data->GetContent(), tree_data->GetContent());
  EXPECT_EQ(new_tree_data->GetType(), std::string("ItemData"));
  EXPECT_EQ(new_tree_data->GetNumberOfChildren(), 2);
  EXPECT_EQ(new_tree_data->GetNumberOfAttributes(), 0);
}
