/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/core/variant.h"

#include <mvvm/utils/string_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing variant_t and utility functions.

class VariantTests : public ::testing::Test
{
};

//! Testing Utils::IsValid function.

TEST_F(VariantTests, IsValid)
{
  using utils::IsValid;
  EXPECT_FALSE(IsValid(variant_t()));
  EXPECT_TRUE(IsValid(variant_t(true)));
  EXPECT_TRUE(IsValid(variant_t(42)));
  EXPECT_TRUE(IsValid(variant_t(42.1)));
  EXPECT_TRUE(IsValid(variant_t(std::string("abc"))));
  EXPECT_TRUE(IsValid(variant_t(std::vector<double>({1.0, 1.1, 1.2}))));
  EXPECT_TRUE(IsValid(variant_t(ComboProperty::CreateFrom({"a1"}))));
  EXPECT_TRUE(IsValid(variant_t(ExternalProperty("text", "color"))));
}

//! Special case of const char
//! Test is failing on old compilers: variant constructed from "const char" is turning into
//! variant<bool>

// TEST_F(VariantTests, ConstChar)
//{
//   variant_t variant1("abc");
//   variant_t variant2(std::string("abc"));
//   EXPECT_TRUE(variant1 == variant2);
//   EXPECT_EQ(std::get<std::string>(variant1), std::string("abc"));
// }

//! Vector<double>

TEST_F(VariantTests, VectorOfDouble)
{
  variant_t variant1(std::vector<double>({1.0, 2.0}));
  variant_t variant2(std::vector<double>({1.0, 2.0}));
  EXPECT_TRUE(variant1 == variant2);

  // to make sure that float precision doesn't pop-up with such kind of comparison
  EXPECT_EQ(std::get<std::vector<double>>(variant1), std::vector<double>({1.0, 2.0}));
}

TEST_F(VariantTests, ComboPropertyVariantEquality)
{
  ComboProperty c1 = ComboProperty() << "a1"
                                     << "a2";
  ComboProperty c2 = ComboProperty() << "a1"
                                     << "a2";

  EXPECT_TRUE(variant_t(c1) == variant_t(c2));

  // make c2 different
  c2 << "a3";
  c2.SetValue("a2");
  EXPECT_TRUE(variant_t(c1) != variant_t(c2));
  EXPECT_FALSE(variant_t(c1) == variant_t(c2));

  // make c1 the same
  c1 << "a3";
  c1.SetValue("a2");
  EXPECT_TRUE(variant_t(c1) == variant_t(c2));
  EXPECT_FALSE(variant_t(c1) != variant_t(c2));

  // change what is selected to make them different again
  c1.SetStringOfSelections("0");
  c2.SetStringOfSelections("1");
  EXPECT_TRUE(variant_t(c1) != variant_t(c2));
  EXPECT_FALSE(variant_t(c1) == variant_t(c2));
}

TEST_F(VariantTests, ExternalPropertyVariantEquality)
{
  ExternalProperty c1("text", "color", "identifier");
  ExternalProperty c2("text", "color", "identifier");
  ExternalProperty c3("text", "color", "");

  EXPECT_TRUE(variant_t(c1) == variant_t(c2));
  EXPECT_FALSE(variant_t(c1) == variant_t(c3));
}

//! Testing Utils::AreCompatible function.

TEST_F(VariantTests, AreCompatible)
{
  std::vector<variant_t> variants = {variant_t(true),
                                     variant_t(42),
                                     variant_t(42.1),
                                     variant_t(std::string("abc")),
                                     variant_t(std::vector<double>({1.1, 2.2, 3.3})),
                                     variant_t(ComboProperty::CreateFrom({"a1"})),
                                     variant_t(ExternalProperty("text", "color"))};
  for (size_t i = 0; i < variants.size(); ++i)
  {
    EXPECT_TRUE(utils::AreCompatible(variant_t(), variants[i]));
    for (size_t j = 0; j < variants.size(); ++j)
    {
      if (i == j)
      {
        EXPECT_TRUE(utils::AreCompatible(variants[i], variants[j]));
      }
      else
      {
        EXPECT_FALSE(utils::AreCompatible(variants[i], variants[j]));
      }
    }
  }
}

TEST_F(VariantTests, TypeName)
{
  using utils::TypeName;

  EXPECT_EQ(TypeName(variant_t()), constants::kEmptyTypeName);

  mvvm::boolean bool_value{true};
  EXPECT_EQ(TypeName(variant_t(bool_value)), constants::kBooleanTypeName);

  mvvm::char8 char_value{'a'};
  EXPECT_EQ(TypeName(variant_t(char_value)), constants::kChar8TypeName);

  mvvm::int8 int8_value{16};
  EXPECT_EQ(TypeName(variant_t(int8_value)), constants::kInt8TypeName);

  mvvm::uint8 uint8_value{16};
  EXPECT_EQ(TypeName(variant_t(uint8_value)), constants::kUInt8TypeName);

  mvvm::int16 int16_value{42};
  EXPECT_EQ(TypeName(variant_t(int16_value)), constants::kInt16TypeName);

  mvvm::uint16 uint16_value{42};
  EXPECT_EQ(TypeName(variant_t(uint16_value)), constants::kUInt16TypeName);

  mvvm::int32 int32_value{42};
  EXPECT_EQ(TypeName(variant_t(int32_value)), constants::kInt32TypeName);

  mvvm::uint32 uint32_value{42};
  EXPECT_EQ(TypeName(variant_t(uint32_value)), constants::kUInt32TypeName);

  mvvm::int64 int64_value{42};
  EXPECT_EQ(TypeName(variant_t(int64_value)), constants::kInt64TypeName);

  mvvm::uint64 uint64_value{42};
  EXPECT_EQ(TypeName(variant_t(uint64_value)), constants::kUInt64TypeName);

  mvvm::float32 float32_value{42.1};
  EXPECT_EQ(TypeName(variant_t(float32_value)), constants::kFloat32TypeName);

  mvvm::float64 float64_value{42.1};
  EXPECT_EQ(TypeName(variant_t(float64_value)), constants::kFloat64TypeName);

  std::string string_value;
  EXPECT_EQ(TypeName(variant_t(string_value)), constants::kStringTypeName);

  EXPECT_EQ(TypeName(variant_t(std::vector<double>({1.0, 1.1, 1.2}))),
            constants::kVectorDoubleTypeName);
  EXPECT_EQ(TypeName(variant_t(ComboProperty::CreateFrom({"a1"}))),
            constants::kComboPropertyTypeName);
  EXPECT_EQ(TypeName(variant_t(ExternalProperty("text", "red"))),
            constants::kExternalPropertyTypeName);
}

TEST_F(VariantTests, DataRoleComparison)
{
  const role_data_t role_data1{0, 42};
  const role_data_t role_data2{0, 42};
  EXPECT_TRUE(role_data1 == role_data2);

  const role_data_t role_data3{42, std::vector<double>{1, 2, 3}};
  const role_data_t role_data4{42, std::vector<double>{1, 2, 3}};
  EXPECT_TRUE(role_data3 == role_data4);

  const role_data_t role_data5{42, std::vector<double>{1, 2}};
  const role_data_t role_data6{42, std::vector<double>{1, 2, 3}};
  EXPECT_FALSE(role_data5 == role_data6);
}

//! Testing methods GetTypeCode, GetString

TEST_F(VariantTests, GetTypeCode)
{
  using utils::ValueToString;

  EXPECT_EQ(GetTypeCode(variant_t{}), TypeCode::Empty);
  EXPECT_EQ(ValueToString(variant_t{}), std::string());

  {
    const mvvm::boolean value{true};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Bool);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("true"));
  }

  {
    const mvvm::char8 value{'a'};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Char8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("97")); // ASCII code
  }

  { // special case, null char
    const mvvm::char8 value{'\0'};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Char8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("0"));  // ASCII code
  }

  {
    const mvvm::int8 value{-8};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-8"));
  }

  {
    const mvvm::uint8 value{16};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("16"));
  }

  {
    const mvvm::int16 value{-42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int16);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-42"));
  }

  {
    const mvvm::uint16 value{42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt16);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("42"));
  }

  {
    const mvvm::int32 value{-42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int32);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-42"));
  }

  {
    const mvvm::uint32 value{42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt32);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("42"));
  }

  {
    const mvvm::int64 value{-42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int64);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-42"));
  }

  {
    const mvvm::uint64 value{42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt64);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("42"));
  }

  {
    const mvvm::float32 value{48.0};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Float32);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("48.0"));
  }

  {
    const mvvm::float64 value{48.0};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Float64);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("48.0"));
  }

  {
    const std::string value{"abc"};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::String);
    EXPECT_EQ(ValueToString(variant_t(value)), value);
  }

  {
    const std::vector<double> value{1.0, 2.0, 3.0};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::VectorOfDouble);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("1.0, 2.0, 3.0"));
  }

  {
    const ComboProperty value = ComboProperty::CreateFrom(std::vector<std::string>{"a1", "a2"});
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::ComboProperty);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("a1;a2"));
  }

  {
    const ExternalProperty value("text", "color", "identifier");
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::ExternalProperty);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("text;color;identifier"));
  }
}
