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

#include "mvvm/core/variant.h"

#include <map>

namespace mvvm
{

bool operator==(const datarole_t &lhs, const datarole_t &rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
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

// std::string TypeName(const variant_t &variant)
//{
//   static std::map<int, std::string> type_name_map = {
//       {0, constants::kEmptyTypeName},
//       {1, constants::kBooleanTypeName},
//       {2, constants::kInt64TypeName},
//       {3, constants::kFloat64TypeName},
//       {4, constants::kStringTypeName},
//       {5, constants::kVectorDoubleTypeName},
//       {6, constants::kComboPropertyTypeName},
//       {7, constants::kExternalPropertyTypeName},
//       {8, constants::kIntLimitsTypeName},
//       {9, constants::kLongIntLimitsTypeName},
//       {10, constants::kRealLimitsTypeName},
//   };
//   return type_name_map[static_cast<int>(variant.index())];
// }

std::string TypeName(const variant_t &variant)
{
  static std::map<TypeCode, std::string> type_name_map = {
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
      {TypeCode::ExternalProperty, constants::kExternalPropertyTypeName},
      {TypeCode::LimitsInt, constants::kIntLimitsTypeName},
      {TypeCode::LimitsInt64, constants::kLongIntLimitsTypeName},
      {TypeCode::LimitsDouble, constants::kRealLimitsTypeName},
  };
  return type_name_map[static_cast<TypeCode>(variant.index())];
}

}  // namespace mvvm::utils
