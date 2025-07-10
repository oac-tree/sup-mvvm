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

#include "variant.h"

#include "variant_value_visitor.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/string_utils.h>

#include <charconv>
#include <functional>
#include <map>

namespace mvvm
{

namespace
{

//! converter function for string to mvvm::variant_t
using string_to_variant_converter = std::function<variant_t(const std::string &str)>;

/**
 * @brief Converts a string to undefined variant.
 */
variant_t to_undefined(const std::string &str)
{
  (void)str;
  return mvvm::variant_t();
}

/**
 * @brief Converts a string to bool variant.
 */
variant_t to_bool(const std::string &str)
{
  return variant_t(mvvm::utils::StringToBool(str));
}

/**
 * @brief Converts a string to integer based variant.
 */
template <typename T>
variant_t to_int(const std::string &str)
{
  T parsed_value;
  const std::from_chars_result result =
      std::from_chars(str.data(), str.data() + str.size(), parsed_value);

  if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
  {
    throw RuntimeException("Can't parse int stored in TreeData");
  }

  return variant_t(parsed_value);
}

/**
 * @brief Converts a string to variant based on a string.
 */
variant_t to_string(const std::string &str)
{
  return variant_t(str);
}

/**
 * @brief Converts a string to variant based on a double.
 */
template <typename T>
variant_t to_double(const std::string &str)
{
  // we use own conversion to double instead of std::stod to not to depend on user's locale
  if (auto value = mvvm::utils::StringToDouble(str); value)
  {
    const T concrete_value(value.value());
    return variant_t(concrete_value);
  }
  throw RuntimeException("Error in variant converter: malformed double number");
}

/**
 * @brief Converts a string to variant based on a vector of doubles.
 */
variant_t to_vector_double(const std::string &str)
{
  const auto values = utils::ParseCommaSeparatedDoubles(str);
  return variant_t(values);
}

/**
 * @brief Converts a string to variant based on a ComboProperty.
 */
variant_t to_combo_property(const std::string &str)
{
  (void)str;
  // FIXME implement after COmboProperty -> string serialization refactoring
  throw RuntimeException("Not implemented");
}

/**
 * @brief Converts a string to variant based on external property.
 */
variant_t to_external_property(const std::string &str)
{
  const auto property = ExternalProperty::CreateFromString(str);
  return variant_t(property);
}

std::map<TypeCode, string_to_variant_converter> CreateStringToVariantConverterMap()
{
  const std::map<TypeCode, string_to_variant_converter> result = {
      {TypeCode::Empty, to_undefined},
      {TypeCode::Bool, to_bool},
      {TypeCode::Char8, to_int<mvvm::char8>},
      {TypeCode::Int8, to_int<mvvm::int8>},
      {TypeCode::UInt8, to_int<mvvm::uint8>},

      {TypeCode::Int16, to_int<mvvm::int16>},
      {TypeCode::UInt16, to_int<mvvm::uint16>},

      {TypeCode::Int32, to_int<mvvm::int32>},
      {TypeCode::UInt32, to_int<mvvm::uint32>},

      {TypeCode::Int64, to_int<mvvm::int64>},
      {TypeCode::UInt64, to_int<mvvm::uint64>},

      {TypeCode::Float32, to_double<mvvm::float32>},
      {TypeCode::Float64, to_double<mvvm::float64>},
      {TypeCode::String, to_string},
      {TypeCode::VectorOfDouble, to_vector_double},
      {TypeCode::ComboProperty, to_combo_property},
      {TypeCode::ExternalProperty, to_external_property}};
  return result;
}

}  // namespace

bool operator==(const role_data_t &lhs, const role_data_t &rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

TypeCode GetTypeCode(const variant_t &variant)
{
  return static_cast<TypeCode>(variant.index());
}

}  // namespace mvvm

namespace mvvm::utils
{

bool IsValid(const variant_t &value)
{
  return value.index() != 0;  // index==0 corresponds to `monostate`
}

bool AreCompatible(const variant_t &var1, const variant_t &var2)
{
  // If one of them is invalid, they are considered to be compatible.
  if (!IsValid(var1) || !IsValid(var2))
  {
    return true;
  }

  // However, properly initialized variants of different types are considered to be incompatible.
  return var1.index() == var2.index();
}

std::string TypeName(const variant_t &variant)
{
  static std::map<TypeCode, std::string> kTypeNameMap = {
      {TypeCode::Empty, constants::kEmptyTypeName},
      {TypeCode::Bool, constants::kBooleanTypeName},
      {TypeCode::Char8, constants::kChar8TypeName},
      {TypeCode::Int8, constants::kInt8TypeName},
      {TypeCode::UInt8, constants::kUInt8TypeName},
      {TypeCode::Int16, constants::kInt16TypeName},
      {TypeCode::UInt16, constants::kUInt16TypeName},
      {TypeCode::Int32, constants::kInt32TypeName},
      {TypeCode::UInt32, constants::kUInt32TypeName},
      {TypeCode::Int64, constants::kInt64TypeName},
      {TypeCode::UInt64, constants::kUInt64TypeName},
      {TypeCode::Float32, constants::kFloat32TypeName},
      {TypeCode::Float64, constants::kFloat64TypeName},
      {TypeCode::String, constants::kStringTypeName},
      {TypeCode::VectorOfDouble, constants::kVectorDoubleTypeName},
      {TypeCode::ComboProperty, constants::kComboPropertyTypeName},
      {TypeCode::ExternalProperty, constants::kExternalPropertyTypeName}};
  return kTypeNameMap[static_cast<TypeCode>(variant.index())];
}

std::string ValueToString(const variant_t &variant)
{
  return std::visit(VariantValueVisitor{}, variant);
}

variant_t ValueFromString(TypeCode type_code, const std::string &str)
{
  const static std::map<TypeCode, string_to_variant_converter> converter_map =
      CreateStringToVariantConverterMap();

  auto iter = converter_map.find(type_code);
  if (iter == converter_map.end())
  {
    throw RuntimeException("Unsupported Qt variant");
  }
  return iter->second(str);
}

}  // namespace mvvm::utils
