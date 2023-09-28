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

#ifndef MVVM_CORE_VARIANT_VISITORS_H_
#define MVVM_CORE_VARIANT_VISITORS_H_

//! @file
//! Defines visitors for variant.

#include <mvvm/core/exceptions.h>
#include <mvvm/core/variant.h>
#include <limits>

namespace mvvm
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

/**
 * @brief Helper structure to visit variant and report numeric limits of stored type.
 *
 * @details Used in the context of integer editors. It is implemented only for types which we can
 * fit into int32: int8, uint8, int16, uint16, int32.
 */

struct VariantLimits32Visitor
{
  std::pair<int, int> operator()(std::monostate)
  {
    throw mvvm::RuntimeException("Visitor for monostate is not implemented");
  }

  std::pair<int, int> operator()(boolean value)
  {
    throw mvvm::RuntimeException("Visitor for boolean is not implemented");
  }

  std::pair<int, int> operator()(char8 value)
  {
    throw mvvm::RuntimeException("Visitor for char8 is not implemented");
  }

  std::pair<int, int> operator()(int8 value)
  {
    return GetMinMaxInt32Pair<int8>(m_lower_bound, m_upper_bound);
  }

  std::pair<int, int> operator()(uint8 value)
  {
    return GetMinMaxInt32Pair<uint8>(m_lower_bound, m_upper_bound);
  }

  std::pair<int, int> operator()(int16 value)
  {
    return GetMinMaxInt32Pair<int16>(m_lower_bound, m_upper_bound);
  }

  std::pair<int, int> operator()(uint16 value)
  {
    return GetMinMaxInt32Pair<uint16>(m_lower_bound, m_upper_bound);
  }

  std::pair<int, int> operator()(int32 value)
  {
    return GetMinMaxInt32Pair<int32>(m_lower_bound, m_upper_bound);
  }

  std::pair<int, int> operator()(uint32 value)
  {
    throw mvvm::RuntimeException("Visitor for uint32 is not implemented");
  }

  std::pair<int, int> operator()(int64 value)
  {
    throw mvvm::RuntimeException("Visitor for int64 is not implemented");
  }

  std::pair<int, int> operator()(uint64 value)
  {
    throw mvvm::RuntimeException("Visitor for uint64 is not implemented");
  }

  std::pair<int, int> operator()(float32 value)
  {
    throw mvvm::RuntimeException("Visitor for float32 is not implemented");
  }

  std::pair<int, int> operator()(float64 value)
  {
    throw mvvm::RuntimeException("Visitor for float64 is not implemented");
  }

  std::pair<int, int> operator()(std::string value)
  {
    throw mvvm::RuntimeException("Visitor for string is not implemented");
  }

  std::pair<int, int> operator()(std::vector<double> value)
  {
    throw mvvm::RuntimeException("Visitor for vector<double> is not implemented");
  }

  std::pair<int, int> operator()(ComboProperty value)
  {
    throw mvvm::RuntimeException("Visitor for ComboProperty is not implemented");
  }

  std::pair<int, int> operator()(ExternalProperty value)
  {
    throw mvvm::RuntimeException("Visitor for ExternalProperty is not implemented");
  }

  std::pair<int, int> operator()(mvvm::Limits<int> value)
  {
    throw mvvm::RuntimeException("Visitor for Limits is not implemented");
  }

  std::pair<int, int> operator()(mvvm::Limits<int64> value)
  {
    throw mvvm::RuntimeException("Visitor for Limits is not implemented");
  }

  std::pair<int, int> operator()(mvvm::Limits<double> value)
  {
    throw mvvm::RuntimeException("Visitor for Limits is not implemented");
  }


  variant_t m_lower_bound;
  variant_t m_upper_bound;
};

/**
 * @brief Returns pairs of integers representing limits of stored type.
 *
 * @param value The value for which we have to find min and max values.
 * @param lower_bound User defined additional lower bound.
 * @param upper_bound User defined additional upper bound.
 *
 * @details Used in the context of integer editors. It is implemented only for types which we can
 * fit into int32: int8, uint8, int16, uint16, int32. When no user defined limits are provided, will
 * return limits deduced from std::numeric.
 */
std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value, const variant_t& lower_bound = {},
                                          const variant_t& upper_bound = {});

}  // namespace mvvm

#endif  // MVVM_CORE_VARIANT_VISITORS_H_
