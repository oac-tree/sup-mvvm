/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/treedata_variant_converter.h"

#include <mvvm/serialization/xml_parse_utils.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <iostream>
#include <stdexcept>

using namespace mvvm;

//! VectorItem tests.

class TreeDataVariantConverterTests : public ::testing::Test
{
public:
  void PrintTreeData(const TreeData& tree_data)
  {
    std::cout << "Type:" << tree_data.GetType() << " "
              << "Name:" << tree_data.GetName() << " "
              << "Children:" << tree_data.GetNumberOfChildren() << std::endl;
    for (const auto& it : tree_data.Attributes())
    {
      std::cout << it.first << " " << it.second << "\n";
    }
  }
};

//! Parsing XML data string representing datarole_t with undefined data.

TEST_F(TreeDataVariantConverterTests, UndefinedDataRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing undefined variant with role=0.
  const std::string body{R"(<Variant role="0" type="empty"></Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(), 0));

  // Converting back
  auto new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing datarole_t with integer data.

TEST_F(TreeDataVariantConverterTests, BoolDataRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing undefined variant with role=0.
  const std::string body{R"(<Variant role="42" type="bool">true</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(true), 42));

  // Converting back
  auto new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // another variant with the value False
  tree_data = ParseXMLElementString(R"(<Variant role="42" type="bool">False</Variant>)");

  // Converting tree_data to data_role
  data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(false), 42));
}

//! Parsing XML data string representing datarole_t with integer data.

TEST_F(TreeDataVariantConverterTests, IntDataRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing int variant with role=0.
  const std::string body{R"(<Variant role="42" type="int64">48</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(48LL), 42));

  // Converting back
  auto new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // Another convertion, now spaces are added.
  tree_data = ParseXMLElementString(R"(<Variant role = "42 " type="int64"> 48 </Variant>)");
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(48LL), 42));

  // Attempting to construct invalid tree (integer value is missed)
  tree_data = ParseXMLElementString(R"(<Variant role="42" type="int64"></Variant>)");
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));
  EXPECT_THROW(ToDataRole(*tree_data), std::invalid_argument);
}

//! Parsing XML data string representing datarole_t with double data.

TEST_F(TreeDataVariantConverterTests, DoubleDataRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing double variant with role=0.
  const std::string body{R"(<Variant role="43" type="float64">42.3</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(42.3), 43));

  // Converting back
  auto new_tree_data = ToTreeData(data_role);
  // Here we can't compare new TreeData with the old one because of the internal conversion of
  // double to string and lost of precision
  EXPECT_DOUBLE_EQ(std::stod(new_tree_data.GetContent()), 42.3);
  EXPECT_EQ(new_tree_data.GetType(), "Variant");
  EXPECT_EQ(new_tree_data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(new_tree_data.GetAttribute("role"), std::string("43"));
}

//! Parsing XML data string representing datarole_t with std::string data.

TEST_F(TreeDataVariantConverterTests, StringDataRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing string variant with role=0.
  const std::string body{R"(<Variant role="7" type="string">James Bond</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::string("James Bond")), 7));

  // Converting back
  auto new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // Another string with more spaces (they should be preserved)
  tree_data = ParseXMLElementString(R"(<Variant role="7" type="string"> James   Bond </Variant>)");
  data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::string(" James   Bond ")), 7));
}

//! Parsing XML data string representing datarole_t with vector.

TEST_F(TreeDataVariantConverterTests, VectorOfDoubleRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="vector_double_t">48.0</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::vector<double>{48.0}), 42));

  // Converting back
  auto new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);

  // vector of two elements
  tree_data =
      ParseXMLElementString(R"(<Variant role="42" type="vector_double_t">1.0, 2.0</Variant>)");

  // Converting tree_data to data_role
  data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(std::vector<double>{1.0, 2.0}), 42));

  // Converting back
  new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing datarole_t with ComboProperty.

TEST_F(TreeDataVariantConverterTests, ComboPropertyRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="ComboProperty">a1</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  ComboProperty expected_combo = ComboProperty::CreateFrom({"a1"});

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);

  EXPECT_EQ(data_role, datarole_t(variant_t(expected_combo), 42));

  // Converting back. New tree_dsata will have selections field defined (this is default behavior
  // of ComboProperty).
  const std::string body2{R"(<Variant role="42" type="ComboProperty" selections="0">a1</Variant>)"};
  auto expected_new_tree_data = ParseXMLElementString(body2);

  EXPECT_EQ(ToTreeData(data_role), *expected_new_tree_data);
}

//! Parsing XML data string representing datarole_t with ComboProperty.

TEST_F(TreeDataVariantConverterTests, ComboPropertyRoleWithSelections)
{
  using mvvm::ParseXMLElementString;

  ComboProperty expected_combo =
      ComboProperty::CreateFrom(std::vector<std::string>({"a1", "a2", "a3"}));
  expected_combo.SetSelected("a1", false);
  expected_combo.SetSelected("a2", true);
  expected_combo.SetSelected("a3", true);

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{
      R"(<Variant role="42" type="ComboProperty" selections="1,2">a1;a2;a3</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);

  EXPECT_EQ(data_role, datarole_t(variant_t(expected_combo), 42));

  // Converting back.
  auto new_tree_data = ToTreeData(data_role);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing datarole_t with ExternalProperty.

TEST_F(TreeDataVariantConverterTests, ExternalPropertyRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{
      R"(<Variant role="42" type="ExternalProperty">text;color;identifier</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  ExternalProperty expected_property("text", "color", "identifier");

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(expected_property), 42));

  // converting back
  EXPECT_EQ(ToTreeData(data_role), *tree_data);
}

//! Parsing XML data string representing datarole_t with empty ExternalProperty.

TEST_F(TreeDataVariantConverterTests, EmptyExternalPropertyRole)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="ExternalProperty">;;</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  ExternalProperty expected_property;

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(expected_property), 42));

  // converting back
  EXPECT_EQ(ToTreeData(data_role), *tree_data);
}

//! Parsing XML data string representing datarole_t with Limits<int>::CreateLimited.

TEST_F(TreeDataVariantConverterTests, IntLimitsLimited)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="IntLimits">limited;1;2</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  auto expected_limits = Limits<int>::CreateLimited(1, 2);

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(expected_limits), 42));

  // converting back
  EXPECT_EQ(ToTreeData(data_role), *tree_data);
}

//! Parsing XML data string representing datarole_t with Limits<int>::CreateNonnegative().

TEST_F(TreeDataVariantConverterTests, IntLimitsNonnegative)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="IntLimits">nonnegative</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  auto expected_limits = Limits<int>::CreateNonnegative();

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(expected_limits), 42));

  // converting back
  EXPECT_EQ(ToTreeData(data_role), *tree_data);
}

//! Parsing XML data string representing datarole_t with Limits<int>::CreateLimited.

TEST_F(TreeDataVariantConverterTests, RealLimitsLimited)
{
  using mvvm::ParseXMLElementString;

  // Constructing TreeData representing a vector with single element with role=0.
  const std::string body{R"(<Variant role="42" type="RealLimits">limited;1.1;2.2</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  auto expected_limits = Limits<double>::CreateLimited(1.1, 2.2);

  // Converting tree_data to data_role
  auto data_role = ToDataRole(*tree_data);
  EXPECT_EQ(data_role, datarole_t(variant_t(expected_limits), 42));

  // converting back
  EXPECT_EQ(ToTreeData(data_role), *tree_data);
}
