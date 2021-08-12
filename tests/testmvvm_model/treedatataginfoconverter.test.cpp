// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

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
