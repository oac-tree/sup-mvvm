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

#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_helper.h>
#include <mvvm/utils/string_format.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief The TestData class represents a data agregate for testing purposes to build XML string.
 *
 * @details A structure is used to configure parameterized test below.
 */
struct TestData
{
  mvvm::variant_t value;  //! the data value
  std::string type_name;  //! it's type name
  std::string value_str;  //! it's string representation
};

//! Testing methods from treedata_variant_converter.h with focus on integer-based numbers

class TreeDataVariantConverterIntegerTest : public ::testing::TestWithParam<TestData>
{
public:
  static inline const int kRole = 42;

  /**
   * @brief Returns a string representing XML of a variant.
   */
  static std::string GetVariantXMLString(const std::string& type_name, const std::string& value_str)
  {
    const std::string body{R"(<Variant role="%1" type="%2">%3</Variant>)"};
    std::string result =
        utils::StringFormat(body).arg(std::to_string(kRole)).arg(type_name).arg(value_str);
    return result;
  }
};

TEST_P(TreeDataVariantConverterIntegerTest, IntegerDataRole)
{
  auto param = GetParam();

  // constructing XML string representing variant with given parameters
  auto xml_str = GetVariantXMLString(param.type_name, param.value_str);

  // building tree data from XML string
  auto tree_data = ParseXMLElementString(xml_str);
  EXPECT_TRUE(IsDataRoleConvertible(*tree_data));

  // converting tree_data to role_data and validating result
  auto role_data = ToRoleData(*tree_data);
  EXPECT_EQ(role_data, role_data_t(kRole, param.value));

  // converting data role to a new tree data and checking against the original
  auto new_tree_data = ToTreeData(role_data);
  EXPECT_EQ(new_tree_data, *tree_data);
}

INSTANTIATE_TEST_SUITE_P(FunctionalTests, TreeDataVariantConverterIntegerTest,
                         ::testing::Values(TestData{mvvm::int8{-42}, "int8", "-42"},
                                           TestData{mvvm::uint8{42}, "uint8", "42"},
                                           TestData{mvvm::int16{-42}, "int16", "-42"},
                                           TestData{mvvm::uint16{42}, "uint16", "42"},
                                           TestData{mvvm::int32{-42}, "int32", "-42"},
                                           TestData{mvvm::uint32{42}, "uint32", "42"},
                                           TestData{mvvm::int64{-42}, "int64", "-42"},
                                           TestData{mvvm::uint64{42}, "uint64", "42"}));
