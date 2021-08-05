// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/treedatavariantconverter.h"

#include "test_utils.h"

#include "mvvm/serialization/xmlparseutils.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace ModelView;

//! VectorItem tests.

class TreeDataVariantConverterTest : public ::testing::Test
{
};

//! Parsing XML data string representing datarole_t with undefined data.

TEST_F(TreeDataVariantConverterTest, UndefinedDataRole)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing undefined variant with role=0.
  const std::string body{R"(<Variant role="0" type="undefined"></Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  auto data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(), 0));

  // Converting back
  auto new_tree_data = GetTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing datarole_t with integer data.

TEST_F(TreeDataVariantConverterTest, BoolDataRole)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing undefined variant with role=0.
  const std::string body{R"(<Variant role="42" type="bool">true</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  auto data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(true), 42));

  // Converting back
  auto new_tree_data = GetTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // another variant with the value False
  tree_data = ParseXMLElementString(R"(<Variant role="42" type="bool">False</Variant>)");

  // Converting tree_data to data_role
  data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(false), 42));
}

//! Parsing XML data string representing datarole_t with integer data.

TEST_F(TreeDataVariantConverterTest, IntDataRole)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing int variant with role=0.
  const std::string body{R"(<Variant role="42" type="int">48</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  auto data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(48), 42));

  // Converting back
  auto new_tree_data = GetTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // Another convertion, now spaces are added.
  tree_data = ParseXMLElementString(R"(<Variant role = "42 " type="int"> 48 </Variant>)");
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(48), 42));

  // Attempting to construct invalid tree (integer value is missed)
  tree_data = ParseXMLElementString(R"(<Variant role="42" type="int"></Variant>)");
  EXPECT_TRUE(IsDataRole(*tree_data));
  EXPECT_THROW(GetDataRole(*tree_data), std::invalid_argument);
}

//! Parsing XML data string representing datarole_t with double data.

TEST_F(TreeDataVariantConverterTest, DoubleDataRole)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing double variant with role=0.
  const std::string body{R"(<Variant role="43" type="double">42.3</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  auto data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(42.3), 43));

  // Converting back
  auto new_tree_data = GetTreeData(data_role);
  // Here we can't compare new TreeData with the old one because of internal convertion of
  // double to string and lost of presicion
  EXPECT_DOUBLE_EQ(std::stod(new_tree_data.GetContent()), 42.3);
  EXPECT_EQ(new_tree_data.GetType(), "Variant");
  EXPECT_EQ(new_tree_data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(new_tree_data.GetAttribute("role"), std::string("43"));
}

//! Parsing XML data string representing datarole_t with std::string data.

TEST_F(TreeDataVariantConverterTest, StringDataRole)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing string variant with role=0.
  const std::string body{R"(<Variant role="7" type="string">James Bond</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  auto data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::string("James Bond")), 7));

  // Converting back
  auto new_tree_data = GetTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // Another string with more spaces (they should be preserved)
  tree_data = ParseXMLElementString(R"(<Variant role="7" type="string"> James   Bond </Variant>)");
  data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::string(" James   Bond ")), 7));
}

//! Parsing XML data string representing datarole_t with integer data.

TEST_F(TreeDataVariantConverterTest, VectorOfDoubleRole)
{
  using ModelView::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="vector_double_t">48.0</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRole(*tree_data));

  // Converting tree_data to data_role
  auto data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::vector<double>{48.0}), 42));

  // Converting back
  auto new_tree_data = GetTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // vector of two elements
  tree_data =
      ParseXMLElementString(R"(<Variant role="42" type="vector_double_t">1.0, 2.0</Variant>)");

  // Converting tree_data to data_role
  data_role = GetDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::vector<double>{1.0, 2.0}), 42));

  // Converting back
  new_tree_data = GetTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);
}
