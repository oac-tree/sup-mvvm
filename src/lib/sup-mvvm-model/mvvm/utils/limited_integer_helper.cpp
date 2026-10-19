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

#include "limited_integer_helper.h"

#include "limited_integer.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <functional>
#include <map>
#include <optional>
#include <string>

namespace mvvm
{

namespace
{

/**
 * @brief Returns the integer stored in the variant, empty if the variant is invalid.
 *
 * @details Throws if the variant holds another type.
 */
template <typename IntegerT>
std::optional<IntegerT> GetOptionalInteger(const variant_t& variant, const std::string& name)
{
  if (!utils::IsValid(variant))
  {
    return std::nullopt;
  }

  if (!std::holds_alternative<IntegerT>(variant))
  {
    throw RuntimeException("The " + name + " type=[" + utils::TypeName(variant)
                           + "] doesn't coincide with integer type=["
                           + utils::TypeName(variant_t{IntegerT{}}) + "]");
  }

  return std::get<IntegerT>(variant);
}

/**
 * @brief Creates specialised limited integer, for given input value and bounds.
 *
 * @details The default_value of a nullable integer, or the value of a non-nullable one, holds
 * IntegerT.
 */
template <typename IntegerT>
std::unique_ptr<ILimitedInteger> FactoryFunc(const variant_t& value, const variant_t& default_value,
                                             const variant_t& lower_bound,
                                             const variant_t& upper_bound, bool is_nullable)
{
  const auto lower = GetOptionalInteger<IntegerT>(lower_bound, "lower bound");
  const auto upper = GetOptionalInteger<IntegerT>(upper_bound, "upper bound");

  if (is_nullable)
  {
    return std::make_unique<LimitedInteger<IntegerT>>(GetOptionalInteger<IntegerT>(value, "value"),
                                                      std::get<IntegerT>(default_value), lower,
                                                      upper);
  }
  return std::make_unique<LimitedInteger<IntegerT>>(std::get<IntegerT>(value), lower, upper);
}

using factory_funct_t = std::function<std::unique_ptr<ILimitedInteger>(
    const variant_t& value, const variant_t& default_value, const variant_t& lower_bound,
    const variant_t& upper_bound, bool is_nullable)>;

std::map<TypeCode, factory_funct_t> CreateFactoryMap()
{
  const std::map<TypeCode, factory_funct_t> result = {
      {TypeCode::Char8, FactoryFunc<char8>},   {TypeCode::Int8, FactoryFunc<int8>},
      {TypeCode::UInt8, FactoryFunc<uint8>},   {TypeCode::Int16, FactoryFunc<int16>},
      {TypeCode::UInt16, FactoryFunc<uint16>}, {TypeCode::Int32, FactoryFunc<int32>},
      {TypeCode::UInt32, FactoryFunc<uint32>}, {TypeCode::Int64, FactoryFunc<int64>},
      {TypeCode::UInt64, FactoryFunc<uint64>}};
  return result;
}

/**
 * @brief Creates limited integer of the type of default_value.
 */
std::unique_ptr<ILimitedInteger> CreateLimitedIntegerImpl(const variant_t& value,
                                                          const variant_t& default_value,
                                                          const variant_t& lower_bound,
                                                          const variant_t& upper_bound,
                                                          bool is_nullable)
{
  static const auto kFactoryMap = CreateFactoryMap();

  const auto iter = kFactoryMap.find(GetTypeCode(default_value));
  if (iter == kFactoryMap.end())
  {
    throw RuntimeException("Can't create limited integer for type=["
                           + utils::TypeName(default_value) + "]");
  }
  return iter->second(value, default_value, lower_bound, upper_bound, is_nullable);
}

}  // namespace

std::unique_ptr<ILimitedInteger> CreateLimitedInteger(const variant_t& value,
                                                      const variant_t& lower_bound,
                                                      const variant_t& upper_bound)
{
  return CreateLimitedIntegerImpl(value, value, lower_bound, upper_bound, false);
}

std::unique_ptr<ILimitedInteger> CreateNullableLimitedInteger(const variant_t& value,
                                                              const variant_t& default_value,
                                                              const variant_t& lower_bound,
                                                              const variant_t& upper_bound)
{
  return CreateLimitedIntegerImpl(value, default_value, lower_bound, upper_bound, true);
}

}  // namespace mvvm
