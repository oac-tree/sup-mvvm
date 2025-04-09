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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/tree_data_variant_converter.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_helper.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing methods from treedata_variant_converter.h

class TreeDataVariantConverterTests : public ::testing::Test
{
public:
  void PrintTreeData(const tree_data_t& tree_data)
  {
    std::cout << "Type:'" << tree_data.GetNodeName() << "' "
              << "Content:'" << tree_data.GetContent() << "' "
              << "Children:" << tree_data.GetNumberOfChildren() << " "
              << " attribute_count: " << tree_data.Attributes().size() << std::endl;
    for (const auto& it : tree_data.Attributes())
    {
      std::cout << it.first << " " << it.second << "\n";
    }
  }
};

//! Parsing XML data string representing role_data_t with undefined data.

TEST_F(TreeDataVariantConverterTests, UndefinedDataRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing undefined variant
  const std::string body{R"(<Variant role="0" type="empty"></Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(0, variant_t()));

  // converting back
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing role_data_t with integer data.

TEST_F(TreeDataVariantConverterTests, BoolDataRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing bool variant
  const std::string body{R"(<Variant role="42" type="bool">true</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(true)));

  // converting back
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);

  // another variant with the value False
  tree_data = ParseXMLElementString(R"(<Variant role="42" type="bool">False</Variant>)");

  // converting tree_data to role_data
  role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(false)));
}

//! Parsing XML data string representing role_data_t with char8 data.

TEST_F(TreeDataVariantConverterTests, Char8DataRole)
{
  using mvvm::ParseXMLElementString;

  {  // char8, 'A', ASCII code 65
    const std::string body{R"(<Variant role="42" type="char8">65</Variant>)"};
    auto tree_data = ParseXMLElementString(body);
    EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

    // converting tree_data to role_data
    auto role_data = ToRoleData(*tree_data);
    EXPECT_EQ(role_data, role_data_t(42, variant_t(mvvm::char8{'A'})));

    // converting back
    auto new_tree_data = ToTreeData(role_data);
    EXPECT_EQ(new_tree_data, *tree_data);
  }

  {  // char8 '\0', ASCII code 0
    const std::string body{R"(<Variant role="42" type="char8">0</Variant>)"};
    auto tree_data = ParseXMLElementString(body);
    EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

    // converting tree_data to role_data
    auto role_data = ToRoleData(*tree_data);
    EXPECT_EQ(role_data, role_data_t(42, variant_t(mvvm::char8{'\0'})));

    // converting back
    auto new_tree_data = ToTreeData(role_data);
    EXPECT_EQ(new_tree_data, *tree_data);
  }
}

//! Parsing XML data string representing role_data_t with integer data.

TEST_F(TreeDataVariantConverterTests, Int32DataRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing int32 variant
  const std::string body{R"(<Variant role="42" type="int32">48</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(48)));

  // converting back
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);

  // another conversion, now spaces are added.
  tree_data = ParseXMLElementString(R"(<Variant role = "42 " type="int32"> 48 </Variant>)");
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // attempting to construct invalid tree (integer value is missed)
  tree_data = ParseXMLElementString(R"(<Variant role="42" type="int32"></Variant>)");
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));
  EXPECT_THROW(ToRoleData(*tree_data), mvvm::RuntimeException);

  // more tests for other inetgers in treedata_variant_converter_tests.cpp
}

//! Parsing XML data string representing role_data_t with double data.

TEST_F(TreeDataVariantConverterTests, DoubleDataRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing float64 variant
  const std::string body{R"(<Variant role="43" type="float64">42.3</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(43, variant_t(42.3)));

  // converting back
  auto new_tree_data = ToTreeData(role_data);
  // Here we can't compare new TreeData with the old one because of the internal conversion of
  // double to string and lost of precision.
  EXPECT_DOUBLE_EQ(std::stod(new_tree_data.GetContent()), 42.3);
  EXPECT_EQ(new_tree_data.GetNodeName(), "Variant");
  EXPECT_EQ(new_tree_data.GetNumberOfAttributes(), 2);
  EXPECT_EQ(new_tree_data.GetAttribute("role"), std::string("43"));
}

//! Parsing XML data string representing role_data_t with std::string data.

TEST_F(TreeDataVariantConverterTests, StringDataRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing string variant
  const std::string body{R"(<Variant role="7" type="string">James Bond</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(7, variant_t(std::string("James Bond"))));

  // converting back
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);

  // another string with more spaces (they should be preserved)
  tree_data = ParseXMLElementString(R"(<Variant role="7" type="string"> James   Bond </Variant>)");
  role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(7, variant_t(std::string(" James   Bond "))));
}

//! Parsing XML data string representing role_data_t with vector.

TEST_F(TreeDataVariantConverterTests, VectorOfDoubleRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing vector with a single element
  const std::string body{R"(<Variant role="42" type="vector_double">48.0</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(std::vector<double>{48.0})));

  // converting back
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);

  // vector of two elements
  tree_data =
      ParseXMLElementString(R"(<Variant role="42" type="vector_double">1.0, 2.0</Variant>)");

  // converting tree_data to role_data
  role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(std::vector<double>{1.0, 2.0})));

  // converting back
  new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing role_data_t with ComboProperty.

TEST_F(TreeDataVariantConverterTests, ComboPropertyRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing a vector with single element
  const std::string body{R"(<Variant role="42" type="ComboProperty">a1</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  ComboProperty expected_combo = ComboProperty::CreateFrom({"a1"});

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);

  EXPECT_EQ(role_data, role_data_t(42, variant_t(expected_combo)));

  // Converting back. New tree_dsata will have selections field defined (this is default behavior
  // of ComboProperty).
  const std::string body2{R"(<Variant role="42" type="ComboProperty" selections="0">a1</Variant>)"};
  auto expected_new_tree_data = ParseXMLElementString(body2);

  EXPECT_EQ(ToTreeData(role_data), *expected_new_tree_data);
}

//! Parsing XML data string representing role_data_t with ComboProperty.

TEST_F(TreeDataVariantConverterTests, ComboPropertyRoleWithSelections)
{
  using mvvm::ParseXMLElementString;

  ComboProperty expected_combo =
      ComboProperty::CreateFrom(std::vector<std::string>({"a1", "a2", "a3"}));
  expected_combo.SetSelected("a1", false);
  expected_combo.SetSelected("a2", true);
  expected_combo.SetSelected("a3", true);

  // constructing TreeData representing ComboProperty with 3 elements, two of which selected
  const std::string body{
      R"(<Variant role="42" type="ComboProperty" selections="1,2">a1;a2;a3</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);

  EXPECT_EQ(role_data, role_data_t(42, variant_t(expected_combo)));

  // converting back.
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);
}

//! Parsing XML data string representing role_data_t with ExternalProperty.

TEST_F(TreeDataVariantConverterTests, ExternalPropertyRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing an ExternalProperty
  const std::string body{
      R"(<Variant role="42" type="ExternalProperty">text;color;identifier</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  ExternalProperty expected_property("text", "color", "identifier");

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(expected_property)));

  // converting back
  EXPECT_EQ(ToTreeData(role_data), *tree_data);
}

//! Parsing XML data string representing role_data_t with empty ExternalProperty.

TEST_F(TreeDataVariantConverterTests, EmptyExternalPropertyRole)
{
  using mvvm::ParseXMLElementString;

  // constructing TreeData representing an ExternalProperty
  const std::string body{R"(<Variant role="42" type="ExternalProperty">;;</Variant>)"};
  auto tree_data = ParseXMLElementString(body);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  ExternalProperty expected_property;

  // converting tree_data to role_data
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(42, variant_t(expected_property)));

  // converting back
  EXPECT_EQ(ToTreeData(role_data), *tree_data);
}
