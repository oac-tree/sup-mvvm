/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/core/variant.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class VariantTests : public ::testing::Test
{
};

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
  c1.SetSelected(0);
  c1.SetSelected(1);
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
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kEmptyTypeName), TypeCode::Empty);

  mvvm::boolean bool_value{true};
  EXPECT_EQ(TypeName(variant_t(bool_value)), constants::kBooleanTypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kBooleanTypeName), TypeCode::Bool);

  mvvm::char8 char_value{'a'};
  EXPECT_EQ(TypeName(variant_t(char_value)), constants::kChar8TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kChar8TypeName), TypeCode::Char8);

  mvvm::int8 int8_value{16};
  EXPECT_EQ(TypeName(variant_t(int8_value)), constants::kInt8TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kInt8TypeName), TypeCode::Int8);

  mvvm::uint8 uint8_value{16};
  EXPECT_EQ(TypeName(variant_t(uint8_value)), constants::kUInt8TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kUInt8TypeName), TypeCode::UInt8);

  mvvm::int16 int16_value{42};
  EXPECT_EQ(TypeName(variant_t(int16_value)), constants::kInt16TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kInt16TypeName), TypeCode::Int16);

  mvvm::uint16 uint16_value{42};
  EXPECT_EQ(TypeName(variant_t(uint16_value)), constants::kUInt16TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kUInt16TypeName), TypeCode::UInt16);

  mvvm::int32 int32_value{42};
  EXPECT_EQ(TypeName(variant_t(int32_value)), constants::kInt32TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kInt32TypeName), TypeCode::Int32);

  mvvm::uint32 uint32_value{42};
  EXPECT_EQ(TypeName(variant_t(uint32_value)), constants::kUInt32TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kUInt32TypeName), TypeCode::UInt32);

  mvvm::int64 int64_value{42};
  EXPECT_EQ(TypeName(variant_t(int64_value)), constants::kInt64TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kInt64TypeName), TypeCode::Int64);

  mvvm::uint64 uint64_value{42};
  EXPECT_EQ(TypeName(variant_t(uint64_value)), constants::kUInt64TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kUInt64TypeName), TypeCode::UInt64);

  mvvm::float32 float32_value{42.1};
  EXPECT_EQ(TypeName(variant_t(float32_value)), constants::kFloat32TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kFloat32TypeName), TypeCode::Float32);

  mvvm::float64 float64_value{42.1};
  EXPECT_EQ(TypeName(variant_t(float64_value)), constants::kFloat64TypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kFloat64TypeName), TypeCode::Float64);

  std::string string_value;
  EXPECT_EQ(TypeName(variant_t(string_value)), constants::kStringTypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kStringTypeName), TypeCode::String);

  EXPECT_EQ(TypeName(variant_t(std::vector<double>({1.0, 1.1, 1.2}))),
            constants::kVectorDoubleTypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kVectorDoubleTypeName), TypeCode::VectorOfDouble);

  EXPECT_EQ(TypeName(variant_t(ComboProperty::CreateFrom({"a1"}))),
            constants::kComboPropertyTypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kComboPropertyTypeName), TypeCode::ComboProperty);

  EXPECT_EQ(TypeName(variant_t(ExternalProperty("text", "red"))),
            constants::kExternalPropertyTypeName);
  EXPECT_EQ(GetTypeCodeFromTypeName(constants::kExternalPropertyTypeName),
            TypeCode::ExternalProperty);

  EXPECT_THROW(GetTypeCodeFromTypeName("abc"), RuntimeException);
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

TEST_F(VariantTests, TypeCodeFromString)
{
  using utils::ValueFromString;
  using utils::ValueToString;

  EXPECT_EQ(GetTypeCode(variant_t{}), TypeCode::Empty);
  EXPECT_EQ(ValueToString(variant_t{}), std::string());

  {
    const mvvm::boolean value{true};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Bool);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("true"));
    EXPECT_EQ(ValueFromString(TypeCode::Bool, std::string("true")), variant_t(value));
  }

  {
    const mvvm::char8 value{'a'};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Char8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("97"));  // ASCII code
    EXPECT_EQ(ValueFromString(TypeCode::Char8, std::string("97")), variant_t(value));
  }

  {  // special case, null char
    const mvvm::char8 value{'\0'};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Char8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("0"));  // ASCII code
    EXPECT_EQ(ValueFromString(TypeCode::Char8, std::string("0")), variant_t(value));
  }

  {
    const mvvm::int8 value{-8};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-8"));
    EXPECT_EQ(ValueFromString(TypeCode::Int8, std::string("-8")), variant_t(value));
  }

  {
    const mvvm::uint8 value{16};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt8);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("16"));
    EXPECT_EQ(ValueFromString(TypeCode::UInt8, std::string("16")), variant_t(value));
  }

  {
    const mvvm::int16 value{-42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int16);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-42"));
    EXPECT_EQ(ValueFromString(TypeCode::Int16, std::string("-42")), variant_t(value));
  }

  {
    const mvvm::uint16 value{42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt16);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("42"));
    EXPECT_EQ(ValueFromString(TypeCode::UInt16, std::string("42")), variant_t(value));
  }

  {
    const mvvm::int32 value{-42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int32);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-42"));
    EXPECT_EQ(ValueFromString(TypeCode::Int32, std::string("-42")), variant_t(value));
  }

  {
    const mvvm::uint32 value{42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt32);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("42"));
    EXPECT_EQ(ValueFromString(TypeCode::UInt32, std::string("42")), variant_t(value));
  }

  {
    const mvvm::int64 value{-42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Int64);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("-42"));
    EXPECT_EQ(ValueFromString(TypeCode::Int64, std::string("-42")), variant_t(value));
  }

  {
    const mvvm::uint64 value{42};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::UInt64);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("42"));
    EXPECT_EQ(ValueFromString(TypeCode::UInt64, std::string("42")), variant_t(value));
  }

  {
    const mvvm::float32 value{48.0};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Float32);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("48.0"));
    EXPECT_EQ(ValueFromString(TypeCode::Float32, std::string("48.0")), variant_t(value));
  }

  {
    const mvvm::float64 value{48.0};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::Float64);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("48.0"));
    EXPECT_EQ(ValueFromString(TypeCode::Float64, std::string("48.0")), variant_t(value));
    EXPECT_THROW(ValueFromString(TypeCode::Float64, std::string("a48.0")), RuntimeException);
  }

  {
    const std::string value{"abc"};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::String);
    EXPECT_EQ(ValueToString(variant_t(value)), value);
    EXPECT_EQ(ValueFromString(TypeCode::String, std::string("abc")), variant_t(value));
  }

  {
    const std::vector<double> value{1.0, 2.0, 3.0};
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::VectorOfDouble);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("1.0,2.0,3.0"));
    EXPECT_EQ(ValueFromString(TypeCode::VectorOfDouble, std::string("1.0,2.0,3.0")),
              variant_t(value));
  }

  {
    const ComboProperty value = ComboProperty::CreateFrom(std::vector<std::string>{"a1", "a2"});
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::ComboProperty);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("values[a1;a2] selection[0]"));
    EXPECT_EQ(ValueFromString(TypeCode::ComboProperty, std::string("values[a1;a2] selection[0]")),
              variant_t(value));
  }

  {
    const ExternalProperty value("text", "color", "identifier");
    EXPECT_EQ(GetTypeCode(variant_t(value)), TypeCode::ExternalProperty);
    EXPECT_EQ(ValueToString(variant_t(value)), std::string("text;color;identifier"));
    EXPECT_EQ(ValueFromString(TypeCode::ExternalProperty, std::string("text;color;identifier")),
              variant_t(value));
  }
}

}  // namespace mvvm::test
