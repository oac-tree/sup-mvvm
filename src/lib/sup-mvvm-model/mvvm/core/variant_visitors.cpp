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

#include "variant_visitors.h"

#include <limits>

namespace
{

/**
 * @brief Returns pair of integer representing numeric limits of given type.
 */
template <typename T>
std::pair<int, int> GetNumericMinMaxInt32Pair()
{
  return {std::numeric_limits<T>::min(), std::numeric_limits<T>::max()};
}

/**
 * @brief Returns pair of integer representing min/max limits of given type.
 *
 * @details It combines user provided limits with numeric limits.
 */
template <typename T>
std::pair<int, int> GetMinMaxInt32Pair(const mvvm::variant_t& lower_bound,
                                       const mvvm::variant_t& upper_bound)
{
  auto result = GetNumericMinMaxInt32Pair<T>();

  if (mvvm::utils::IsValid(lower_bound))
  {
    result.first = static_cast<int>(std::get<T>(lower_bound));
  }

  if (mvvm::utils::IsValid(upper_bound))
  {
    result.second = static_cast<int>(std::get<T>(upper_bound));
  }

  return result;
}

}  // namespace

namespace mvvm
{

std::pair<int, int> VariantLimits32Visitor::operator()(std::monostate)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(boolean value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(char8 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(int8 value)
{
  return GetMinMaxInt32Pair<int8>(m_lower_bound, m_upper_bound);
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint8 value)
{
  return GetMinMaxInt32Pair<uint8>(m_lower_bound, m_upper_bound);
}

std::pair<int, int> VariantLimits32Visitor::operator()(int16 value)
{
  return GetMinMaxInt32Pair<int16>(m_lower_bound, m_upper_bound);
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint16 value)
{
  return GetMinMaxInt32Pair<uint16>(m_lower_bound, m_upper_bound);
}

std::pair<int, int> VariantLimits32Visitor::operator()(int32 value)
{
  return GetMinMaxInt32Pair<int32>(m_lower_bound, m_upper_bound);
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint32 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(int64 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(uint64 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(float32 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(float64 value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(std::string value)
{
  throw mvvm::RuntimeException("Visitor for string is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(std::vector<double> value)
{
  throw mvvm::RuntimeException("Visitor for vector<double> is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(ComboProperty value)
{
  throw mvvm::RuntimeException("Visitor for ComboProperty is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(ExternalProperty value)
{
  throw mvvm::RuntimeException("Visitor for ExternalProperty is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(mvvm::Limits<int> value)
{
  throw mvvm::RuntimeException("Visitor for Limits is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(mvvm::Limits<int64> value)
{
  throw mvvm::RuntimeException("Visitor for Limits is not implemented");
}

std::pair<int, int> VariantLimits32Visitor::operator()(mvvm::Limits<double> value)
{
  throw mvvm::RuntimeException("Visitor for Limits is not implemented");
}

std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value, const variant_t& lower_bound,
                                          const variant_t& upper_bound)
{
  VariantLimits32Visitor visitor;
  visitor.m_lower_bound = lower_bound;
  visitor.m_upper_bound = upper_bound;

  return std::visit(visitor, value);
}

}  // namespace mvvm
