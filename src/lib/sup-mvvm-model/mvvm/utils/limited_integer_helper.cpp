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

namespace mvvm
{

namespace
{

/**
 * @brief Creates specialised limited integer, for given input value and bounds.
 */
template <typename IntegerT>
std::unique_ptr<ILimitedInteger> FactoryFunc(const variant_t& value, const variant_t& lower_bound,
                                             const variant_t& upper_bound)
{
  return std::make_unique<LimitedInteger<IntegerT>>(value, lower_bound, upper_bound);
}

using factory_funct_t = std::function<std::unique_ptr<ILimitedInteger>(
    const variant_t& value, const variant_t& lower_bound, const variant_t& upper_bound)>;

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

}  // namespace

std::unique_ptr<ILimitedInteger> CreateLimitedInteger(const variant_t& value,
                                                      const variant_t& lower_bound,
                                                      const variant_t& upper_bound)
{
  static const auto kFactoryMap = CreateFactoryMap();

  auto iter = kFactoryMap.find(GetTypeCode(value));
  if (iter == kFactoryMap.end())
  {
    throw RuntimeException("Can't create limited integer for type=[" + utils::TypeName(value)
                           + "]");
  }
  return iter->second(value, lower_bound, upper_bound);
}

}  // namespace mvvm
