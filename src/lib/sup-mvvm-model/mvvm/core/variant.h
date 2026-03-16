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

#ifndef MVVM_CORE_VARIANT_H_
#define MVVM_CORE_VARIANT_H_

//! @file
//! Defines all supported elementary data types.

#include <mvvm/core/basic_scalar_types.h>
#include <mvvm/core/mvvm_types.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>

#include <string_view>
#include <variant>
#include <vector>

namespace mvvm
{

enum class TypeCode : uint32
{
  Empty = 0,
  Bool,
  Char8,
  Int8,
  UInt8,
  Int16,
  UInt16,
  Int32,
  UInt32,
  Int64,
  UInt64,
  Float32,
  Float64,
  String,
  VectorOfDouble,
  ComboProperty,
  ExternalProperty
};

using variant_t = std::variant<std::monostate, boolean, char8, int8, uint8, int16, uint16, int32,
                               uint32, int64, uint64, float32, float64, std::string,
                               std::vector<float64>, ComboProperty, ExternalProperty>;

using role_data_t = std::pair<role_t, variant_t>;
bool operator==(const role_data_t& lhs, const role_data_t& rhs);

/**
 * @brief Returns type code for given variant.
 */
TypeCode GetTypeCode(const variant_t& variant);

/**
 * @brief Returns type code from type name.
 */
TypeCode GetTypeCodeFromTypeName(const std::string& str);

/**
 * @brief Returns type name from type code.
 */
std::string GetTypeNameFromTypeCode(TypeCode type_code);

}  // namespace mvvm

//! Defines names of all supported data types.
namespace mvvm::constants
{

constexpr auto kEmptyTypeName = "empty";
constexpr auto kBooleanTypeName = "bool";
constexpr auto kChar8TypeName = "char8";
constexpr auto kInt8TypeName = "int8";
constexpr auto kUInt8TypeName = "uint8";
constexpr auto kInt16TypeName = "int16";
constexpr auto kUInt16TypeName = "uint16";
constexpr auto kInt32TypeName = "int32";
constexpr auto kUInt32TypeName = "uint32";
constexpr auto kInt64TypeName = "int64";
constexpr auto kUInt64TypeName = "uint64";
constexpr auto kFloat32TypeName = "float32";
constexpr auto kFloat64TypeName = "float64";
constexpr auto kStringTypeName = "string";

constexpr auto kVectorDoubleTypeName = "vector_double";
constexpr auto kComboPropertyTypeName = "ComboProperty";
constexpr auto kExternalPropertyTypeName = "ExternalProperty";


// clang-format off
constexpr std::string_view EmptyTypeName() noexcept { return "empty"; }
constexpr std::string_view BooleanTypeName() noexcept { return "bool"; }
constexpr std::string_view Char8TypeName() noexcept { return "char8"; }
constexpr std::string_view Int8TypeName() noexcept { return "int8"; }
constexpr std::string_view UInt8TypeName() noexcept { return "uint8"; }
constexpr std::string_view Int16TypeName() noexcept { return "int16"; }
constexpr std::string_view UInt16TypeName() noexcept { return "uint16"; }
constexpr std::string_view Int32TypeName() noexcept { return "int32"; }
constexpr std::string_view UInt32TypeName() noexcept { return "uint32"; }
constexpr std::string_view Int64TypeName() noexcept { return "int64"; }
constexpr std::string_view UInt64TypeName() noexcept { return "uint64"; }
constexpr std::string_view Float32TypeName() noexcept { return "float32"; }
constexpr std::string_view Float64TypeName() noexcept { return "float64"; }
constexpr std::string_view StringTypeName() noexcept { return "string"; }
constexpr std::string_view VectorDoubleTypeName() noexcept { return "vector_double"; }
constexpr std::string_view ComboPropertyTypeName() noexcept { return "ComboProperty"; }
constexpr std::string_view ExternalPropertyTypeName() noexcept { return "ExternalProperty"; }
// clang-format on
}  // namespace mvvm::constants

namespace mvvm::utils
{

/**
 * @brief Checks if the given variant is properly initialized with one of the supported data types.
 *
 * The default constructed variant (i.e. which contains a monostate) is invalid.
 */
bool IsValid(const variant_t& value);

/**
 * @brief Checks if two variants are compatible, i.e. we are alowed to replace one variant with
 * another.
 *
 * Two valid variants are considered compatible, when they both have the same underlying type. If
 * one of the variants is invalid, they are considered compatible too.
 */
bool AreCompatible(const variant_t& var1, const variant_t& var2);

/**
 * @brief Returns a string representing the variant type.
 */
std::string TypeName(const variant_t& variant);

/**
 * @brief Returns a string representing the variant value.
 */
std::string ValueToString(const variant_t& variant);

/**
 * @brief Constructs a variant from its string representation.
 */
variant_t ValueFromString(TypeCode type_code, const std::string& str);

}  // namespace mvvm::utils

#endif  // MVVM_CORE_VARIANT_H_
