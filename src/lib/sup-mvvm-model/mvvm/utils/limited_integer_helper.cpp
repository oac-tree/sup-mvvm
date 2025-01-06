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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "limited_integer_helper.h"

#include "limited_integer.h"

#include <mvvm/core/exceptions.h>

#include <functional>
#include <map>

namespace
{

/**
 * @brief Creates specialised limited integer, for given input value and bounds.
 */
template <typename IntegerT>
std::unique_ptr<mvvm::ILimitedInteger> FactoryFunc(const mvvm::variant_t &value,
                                                   const mvvm::variant_t &lower_bound,
                                                   const mvvm::variant_t &upper_bound)
{
  return std::make_unique<mvvm::LimitedInteger<IntegerT>>(value, lower_bound, upper_bound);
}

using factory_funct_t = std::function<std::unique_ptr<mvvm::ILimitedInteger>(
    const mvvm::variant_t &value, const mvvm::variant_t &lower_bound,
    const mvvm::variant_t &upper_bound)>;

const std::map<mvvm::TypeCode, factory_funct_t> factory_map = {
    {mvvm::TypeCode::Char8, FactoryFunc<mvvm::char8>},
    {mvvm::TypeCode::Int8, FactoryFunc<mvvm::int8>},
    {mvvm::TypeCode::UInt8, FactoryFunc<mvvm::uint8>},
    {mvvm::TypeCode::Int16, FactoryFunc<mvvm::int16>},
    {mvvm::TypeCode::UInt16, FactoryFunc<mvvm::uint16>},
    {mvvm::TypeCode::Int32, FactoryFunc<mvvm::int32>},
    {mvvm::TypeCode::UInt32, FactoryFunc<mvvm::uint32>},
    {mvvm::TypeCode::Int64, FactoryFunc<mvvm::int64>},
    {mvvm::TypeCode::UInt64, FactoryFunc<mvvm::uint64>}};

}  // namespace

namespace mvvm
{
std::unique_ptr<ILimitedInteger> CreateLimitedInteger(const variant_t &value,
                                                      const variant_t &lower_bound,
                                                      const variant_t &upper_bound)
{
  auto iter = factory_map.find(GetTypeCode(value));
  if (iter == factory_map.end())
  {
    throw RuntimeException("Can't create limited integer for type=[" + utils::TypeName(value)
                           + "]");
  }
  return iter->second(value, lower_bound, upper_bound);
}

}  // namespace mvvm
