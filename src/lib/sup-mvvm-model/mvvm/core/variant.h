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

#ifndef MVVM_CORE_VARIANT_H_
#define MVVM_CORE_VARIANT_H_

//! @file
//! Defines all supported elementary data types.

#include <mvvm/core/basic_scalar_types.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/external_property.h>
#include <mvvm/model/limits.h>
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
  ExternalProperty,
  LimitsInt,
  LimitsInt64,
  LimitsDouble
};

 using variant_t = std::variant<std::monostate, boolean, int64, double, std::string,
                                std::vector<double>, mvvm::ComboProperty, mvvm::ExternalProperty,
                                mvvm::Limits<int>, mvvm::Limits<int64>, mvvm::Limits<double>>;

//using variant_t =
//    std::variant<std::monostate, boolean, char8, int8, uint8, int16, uint16, int32, uint32, int64,
//                 uint64, float32, float64, std::string, std::vector<double>, ComboProperty,
//                 ExternalProperty, Limits<int>, Limits<int64>, Limits<double>>;

using datarole_t = std::pair<variant_t, int>;
bool operator==(const datarole_t& lhs, const datarole_t& rhs);

}  // namespace mvvm

//! Defines names of all supported (serializable) data types.
namespace mvvm::constants
{
const std::string kEmptyTypeName = "empty";
const std::string kBooleanTypeName = "bool";
const std::string kInt64TypeName = "int64";
const std::string kFloat64TypeName = "float64";
const std::string kStringTypeName = "string";

const std::string kVectorDoubleVariantName = "vector_double_t";
const std::string kComboPropertyVariantName = "ComboProperty";
const std::string kExternalPropertyVariantName = "ExternalProperty";
const std::string kIntLimitsVariantName = "IntLimits";
const std::string kLongIntLimitsVariantName = "LongIntLimits";
const std::string kRealLimitsVariantName = "RealLimits";
}  // namespace mvvm::constants

namespace mvvm::utils
{
//! Returns true if given variant is properly initialized (i.e. is not default-constructed).
MVVM_MODEL_EXPORT bool IsValid(const variant_t& value);

//! Returns true if two variants are compatible, i.e. we are alowed to replace one variant with
//! another.
MVVM_MODEL_EXPORT bool AreCompatible(const variant_t& var1, const variant_t& var2);

//! Returns string representing type name.
MVVM_MODEL_EXPORT std::string TypeName(const variant_t& variant);

}  // namespace mvvm::utils

#endif  // MVVM_CORE_VARIANT_H_
