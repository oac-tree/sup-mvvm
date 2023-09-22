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

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/string_utils.h>

#include <map>

namespace
{

/**
 * @brief Helper structure to visit variant and report string representation of its value.
 */
struct VariantValueVisitor
{
  std::string operator()(std::monostate) { return {}; }
  std::string operator()(mvvm::boolean value) { return mvvm::utils::FromBool(value); }
  std::string operator()(mvvm::char8 value) { return std::string(1, value); }
  std::string operator()(mvvm::int8 value) { return {std::to_string(value)}; }
  std::string operator()(mvvm::uint8 value) { return {std::to_string(value)}; }

  std::string operator()(mvvm::int16 value) { return {std::to_string(value)}; }
  std::string operator()(mvvm::uint16 value) { return {std::to_string(value)}; }

  std::string operator()(mvvm::int32 value) { return {std::to_string(value)}; }
  std::string operator()(mvvm::uint32 value) { return {std::to_string(value)}; }

  std::string operator()(mvvm::int64 value) { return {std::to_string(value)}; }
  std::string operator()(mvvm::uint64 value) { return {std::to_string(value)}; }

  std::string operator()(mvvm::float32 value) { return mvvm::utils::DoubleToString(value); }

  std::string operator()(mvvm::float64 value) { return mvvm::utils::DoubleToString(value); }

  std::string operator()(std::string value) { return value; }

  std::string operator()(std::vector<double> value)
  {
    return {mvvm::utils::ToCommaSeparatedString(value)};
  }

  std::string operator()(mvvm::ComboProperty value) { return {value.GetStringOfValues()}; }

  std::string operator()(mvvm::ExternalProperty value) { return value.ToString(); }

  std::string operator()(mvvm::Limits<int> value)
  {
    throw mvvm::RuntimeException("Visitor for Limits is not implemented");
  }

  std::string operator()(mvvm::Limits<mvvm::int64> value)
  {
    throw mvvm::RuntimeException("Visitor for Limits is not implemented");
  }

  std::string operator()(mvvm::Limits<double> value)
  {
    throw mvvm::RuntimeException("Visitor for Limits is not implemented");
  }
};

}  // namespace

namespace mvvm
{

bool operator==(const datarole_t &lhs, const datarole_t &rhs)
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

std::string ValueToString(const variant_t &variant)
{
  return std::visit(VariantValueVisitor{}, variant);
}

}  // namespace mvvm::utils
