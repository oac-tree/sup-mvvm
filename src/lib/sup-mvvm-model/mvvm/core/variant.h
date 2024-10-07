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

#ifndef MVVM_CORE_VARIANT_H_
#define MVVM_CORE_VARIANT_H_

//! @file
//! Defines all supported elementary data types.

#include <mvvm/core/basic_scalar_types.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>
#include <mvvm/model_export.h>

#include <string>
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

using role_data_t = std::pair<int, variant_t>;
bool operator==(const role_data_t& lhs, const role_data_t& rhs);

/**
 * @brief Returns type code for given variant.
 */
TypeCode GetTypeCode(const variant_t& variant);

}  // namespace mvvm

//! Defines names of all supported data types.
namespace mvvm::constants
{

const std::string kEmptyTypeName = "empty";
const std::string kBooleanTypeName = "bool";
const std::string kChar8TypeName = "char8";
const std::string kInt8TypeName = "int8";
const std::string kUInt8TypeName = "uint8";
const std::string kInt16TypeName = "int16";
const std::string kUInt16TypeName = "uint16";
const std::string kInt32TypeName = "int32";
const std::string kUInt32TypeName = "uint32";
const std::string kInt64TypeName = "int64";
const std::string kUInt64TypeName = "uint64";
const std::string kFloat32TypeName = "float32";
const std::string kFloat64TypeName = "float64";
const std::string kStringTypeName = "string";

const std::string kVectorDoubleTypeName = "vector_double";
const std::string kComboPropertyTypeName = "ComboProperty";
const std::string kExternalPropertyTypeName = "ExternalProperty";
}  // namespace mvvm::constants

namespace mvvm::utils
{

/**
 * @brief Checks if the given variant is properly initialized with one of the supported data types.
 *
 * The default constructed variant (i.e. which contains a monostate) is invalid.
 */
MVVM_MODEL_EXPORT bool IsValid(const variant_t& value);

/**
 * @brief Checks if two variants are compatible, i.e. we are alowed to replace one variant with
 * another.
 *
 * Two valid variants are considered compatible, when they both have the same underlying type. If
 * one of the variants is invalid, they are considered compatible too.
 */
MVVM_MODEL_EXPORT bool AreCompatible(const variant_t& var1, const variant_t& var2);

/**
 * @brief Returns a string representing the variant type.
 */
MVVM_MODEL_EXPORT std::string TypeName(const variant_t& variant);

/**
 * @brief Returns a string representing the variant value.
 */
MVVM_MODEL_EXPORT std::string ValueToString(const variant_t& variant);

}  // namespace mvvm::utils

#endif  // MVVM_CORE_VARIANT_H_
