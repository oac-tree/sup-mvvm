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
using string_to_variant_converter_t = std::function<variant_t(const std::string& str)>;

/**
 * @brief Converts a string to undefined variant.
 */
variant_t to_undefined([[maybe_unused]] const std::string& str)
{
  return {};
}

/**
 * @brief Converts a string to bool variant.
 */
variant_t to_bool(const std::string& str)
{
  return {mvvm::utils::StringToBool(str)};
}

/**
 * @brief Converts a string to integer based variant.
 */
template <typename T>
variant_t to_int(const std::string& str)
{
  T parsed_value;
  const std::from_chars_result result =
      std::from_chars(str.data(), str.data() + str.size(), parsed_value);

  if ((result.ec == std::errc::invalid_argument) || (result.ec == std::errc::result_out_of_range))
  {
    throw RuntimeException("Can't parse int stored in TreeData");
  }

  return {parsed_value};
}

/**
 * @brief Converts a string to variant based on a string.
 */
variant_t to_string(const std::string& str)
{
  return {str};
}

/**
 * @brief Converts a string to variant based on a double.
 */
template <typename T>
variant_t to_double(const std::string& str)
{
  // we use own conversion to double instead of std::stod to not to depend on user's locale
  if (auto value = mvvm::utils::StringToDouble(str); value)
  {
    const T concrete_value(static_cast<T>(value.value()));
    return variant_t(concrete_value);
  }
  throw RuntimeException("Error in variant converter: malformed double number");
}

/**
 * @brief Converts a string to variant based on a vector of doubles.
 */
variant_t to_vector_double(const std::string& str)
{
  return utils::StringToVector<double>(str);
}

/**
 * @brief Converts a string to variant based on a ComboProperty.
 */
variant_t to_combo_property(const std::string& str)
{
  return ComboProperty::CreateFromString(str);
}

/**
 * @brief Converts a string to variant based on external property.
 */
variant_t to_external_property(const std::string& str)
{
  return ExternalProperty::CreateFromString(str);
}

/**
 * @brief Creates a map to convert TypeCode to variant converter.
 */
std::map<TypeCode, string_to_variant_converter_t> CreateStringToVariantConverterMap()
{
  const std::map<TypeCode, string_to_variant_converter_t> result = {
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

/**
 * @brief Creates a map to convert TypeCode to string.
 */
std::map<TypeCode, std::string> CreateTypeCodeToStringMap()
{
  const std::map<TypeCode, std::string> result = {
      {TypeCode::Empty, std::string(constants::EmptyTypeName())},
      {TypeCode::Bool, std::string(constants::BooleanTypeName())},
      {TypeCode::Char8, std::string(constants::Char8TypeName())},
      {TypeCode::Int8, std::string(constants::Int8TypeName())},
      {TypeCode::UInt8, std::string(constants::UInt8TypeName())},
      {TypeCode::Int16, std::string(constants::Int16TypeName())},
      {TypeCode::UInt16, std::string(constants::UInt16TypeName())},
      {TypeCode::Int32, std::string(constants::Int32TypeName())},
      {TypeCode::UInt32, std::string(constants::UInt32TypeName())},
      {TypeCode::Int64, std::string(constants::Int64TypeName())},
      {TypeCode::UInt64, std::string(constants::UInt64TypeName())},
      {TypeCode::Float32, std::string(constants::Float32TypeName())},
      {TypeCode::Float64, std::string(constants::Float64TypeName())},
      {TypeCode::String, std::string(constants::StringTypeName())},
      {TypeCode::VectorOfDouble, std::string(constants::VectorDoubleTypeName())},
      {TypeCode::ComboProperty, std::string(constants::ComboPropertyTypeName())},
      {TypeCode::ExternalProperty, std::string(constants::ExternalPropertyTypeName())}};
  return result;
}

/**
 * @brief Creates a map to convert string to TypeCode.
 *
 * We reuse already existing map and invert it.
 */
std::map<std::string, TypeCode> CreateStringToTypeCodeMap()
{
  std::map<std::string, TypeCode> result;

  for (const auto& [key, value] : CreateTypeCodeToStringMap())
  {
    (void)result.insert({value, key});
  }

  return result;
}

}  // namespace

bool operator==(const role_data_t& lhs, const role_data_t& rhs)
{
  return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

TypeCode GetTypeCode(const variant_t& variant)
{
  return static_cast<TypeCode>(variant.index());
}

TypeCode GetTypeCodeFromTypeName(const std::string& str)
{
  static const std::map<std::string, TypeCode> kStringToTypeCodeMap = CreateStringToTypeCodeMap();
  auto iter = kStringToTypeCodeMap.find(str);
  if (iter == kStringToTypeCodeMap.end())
  {
    throw RuntimeException("Can't find type name");
  }

  return iter->second;
}

std::string GetTypeNameFromTypeCode(TypeCode type_code)
{
  static const std::map<TypeCode, std::string> kTypeCodeToStringMap = CreateTypeCodeToStringMap();
  auto iter = kTypeCodeToStringMap.find(type_code);
  if (iter == kTypeCodeToStringMap.end())
  {
    throw RuntimeException("Can't find type name");
  }

  return iter->second;
}

}  // namespace mvvm

namespace mvvm::utils
{

bool IsValid(const variant_t& value)
{
  return value.index() != 0;  // index==0 corresponds to `monostate`
}

bool AreCompatible(const variant_t& var1, const variant_t& var2)
{
  // If one of them is invalid, they are considered to be compatible.
  if (!IsValid(var1) || !IsValid(var2))
  {
    return true;
  }

  // However, properly initialized variants of different types are considered to be incompatible.
  return var1.index() == var2.index();
}

std::string TypeName(const variant_t& variant)
{
  return GetTypeNameFromTypeCode(GetTypeCode(variant));
}

std::string ValueToString(const variant_t& variant)
{
  return std::visit(VariantValueVisitor{}, variant);
}

variant_t ValueFromString(TypeCode type_code, const std::string& str)
{
  const static std::map<TypeCode, string_to_variant_converter_t> converter_map =
      CreateStringToVariantConverterMap();

  auto iter = converter_map.find(type_code);
  if (iter == converter_map.end())
  {
    throw RuntimeException("Unsupported Qt variant");
  }
  return iter->second(str);
}

}  // namespace mvvm::utils
