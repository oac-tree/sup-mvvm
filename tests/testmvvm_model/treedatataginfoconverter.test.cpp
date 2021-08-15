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

#include "mvvm/serialization/treedatataginfoconverter.h"

#include "test_utils.h"

#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/xmlparseutils.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! VectorItem tests.

class TreeDataTagInfoConverterTest : public ::testing::Test
{
};

//! Parsing XML data string representing TagInfo without model_types defined.

TEST_F(TreeDataTagInfoConverterTest, ConvertUniversalTag)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing TagInfo without model types defined.
  const std::string body{R"(<TagInfo min="0" max="1" name="Width"></TagInfo>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsTagInfoConvertible(*tree_data));

  // Converting tree_data to data_role
  auto tag_info = ToTagInfo(*tree_data);
  EXPECT_EQ(tag_info.GetMin(), 0);
  EXPECT_EQ(tag_info.GetMax(), 1);
  EXPECT_EQ(tag_info.GetName(), "Width");
  EXPECT_EQ(tag_info.GetModelTypes(), std::vector<std::string>());

  // Converting back
  auto new_tree_data = ToTreeData(tag_info);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing TagInfo without model_types defined.

TEST_F(TreeDataTagInfoConverterTest, ConvertUniversalTagWithModelTypes)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing TagInfo without model types defined.
  const std::string body{
      R"(<TagInfo min="0" max="-1" name="Width">SegmentItem, PulseItem</TagInfo>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsTagInfoConvertible(*tree_data));

  // Converting tree_data to data_role
  auto tag_info = ToTagInfo(*tree_data);
  EXPECT_EQ(tag_info.GetMin(), 0);
  EXPECT_EQ(tag_info.GetMax(), -1);
  EXPECT_EQ(tag_info.GetName(), "Width");
  EXPECT_EQ(tag_info.GetModelTypes(), std::vector<std::string>({"SegmentItem", "PulseItem"}));

  // Converting back
  auto new_tree_data = ToTreeData(tag_info);
  EXPECT_EQ(new_tree_data, *tree_data);
}
