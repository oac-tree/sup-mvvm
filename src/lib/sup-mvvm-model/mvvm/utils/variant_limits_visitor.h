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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_UTILS_VARIANT_INTEGER_LIMITS_VISITOR_H
#define MVVM_UTILS_VARIANT_INTEGER_LIMITS_VISITOR_H

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/core/variant.h>

#include <limits>
#include <vector>

namespace mvvm
{

/**
 * @brief Returns pair representing numeric limits of given type.
 *
 * @tparam ActualT Type which limits we want to know.
 * @tparam OutputT Typw to cast the result.
 */
template <typename OutputT, typename ActualT>
std::pair<OutputT, OutputT> GetNumericMinMaxPair()
{
  auto max = static_cast<OutputT>(std::numeric_limits<ActualT>::max());

  if constexpr (std::is_same_v<ActualT, float64>)
  {
    return {static_cast<OutputT>(std::numeric_limits<ActualT>::lowest()), max};
  }

  if constexpr (std::is_same_v<ActualT, float32>)
  {
    return {static_cast<OutputT>(std::numeric_limits<ActualT>::lowest()), max};
  }

  return {static_cast<OutputT>(std::numeric_limits<ActualT>::min()), max};
}

/**
 * @brief Returns pair representing min/max limits of given type.
 *
 * @tparam OutputLimitsT Limits will be casted to this type.
 * @tparam ActualLimitsT Type of user provided limits.
 *
 * @details It combines user provided limits with numeric limits.
 */
template <typename OutputLimitsT, typename ActualLimitsT>
std::pair<OutputLimitsT, OutputLimitsT> GetMinMaxLimitsPair(const variant_t& lower_bound,
                                                            const variant_t& upper_bound)
{
  auto result = GetNumericMinMaxPair<OutputLimitsT, ActualLimitsT>();

  if (utils::IsValid(lower_bound))
  {
    result.first = static_cast<OutputLimitsT>(std::get<ActualLimitsT>(lower_bound));
  }

  if (utils::IsValid(upper_bound))
  {
    result.second = static_cast<OutputLimitsT>(std::get<ActualLimitsT>(upper_bound));
  }

  return result;
}

/**
 * @brief Helper structure to visit variant and report numeric limits of stored type.
 *
 * @tparam Type of limits to report.
 *
 * @details Reported limits will be obtained by the combination of user defined limits and numeric
 * limits. If one of user limits is not set, std::numeric_limit::min() and std::numeric_limit::max()
 * will be used for lower and upper bounds.
 *
 * @details Deduced limits will be casted to LimitsT parameter.
 */

template <typename LimitsT>
struct VariantLimitsVisitor
{
  std::pair<LimitsT, LimitsT> operator()(std::monostate value);

  std::pair<LimitsT, LimitsT> operator()(boolean value);

  std::pair<LimitsT, LimitsT> operator()(char8 value);

  std::pair<LimitsT, LimitsT> operator()(int8 value);

  std::pair<LimitsT, LimitsT> operator()(uint8 value);

  std::pair<LimitsT, LimitsT> operator()(int16 value);

  std::pair<LimitsT, LimitsT> operator()(uint16 value);

  std::pair<LimitsT, LimitsT> operator()(int32 value);

  std::pair<LimitsT, LimitsT> operator()(uint32 value);

  std::pair<LimitsT, LimitsT> operator()(int64 value);

  std::pair<LimitsT, LimitsT> operator()(uint64 value);

  std::pair<LimitsT, LimitsT> operator()(float32 value);

  std::pair<LimitsT, LimitsT> operator()(float64 value);

  std::pair<LimitsT, LimitsT> operator()(const std::string& value);

  std::pair<LimitsT, LimitsT> operator()(const std::vector<double>& value);

  std::pair<LimitsT, LimitsT> operator()(const ComboProperty& value);

  std::pair<LimitsT, LimitsT> operator()(const ExternalProperty& value);

  variant_t m_lower_bound;
  variant_t m_upper_bound;
};

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(std::monostate value)
{
  (void)value;
  throw RuntimeException("Visitor for monostate is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(boolean value)
{
  (void)value;
  throw RuntimeException("Visitor for boolean is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(char8 value)
{
  (void)value;
  throw RuntimeException("Visitor for char8 is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(int8 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, int8>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(uint8 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, uint8>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(int16 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, int16>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(uint16 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, uint16>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(int32 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, int32>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(uint32 value)
{
  (void)value;
  throw RuntimeException("Visitor for uint32 is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(int64 value)
{
  (void)value;
  throw RuntimeException("Visitor for int64 is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(uint64 value)
{
  (void)value;
  throw RuntimeException("Visitor for uint64 is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(float32 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, float32>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(float64 value)
{
  (void)value;
  return GetMinMaxLimitsPair<LimitsT, float64>(m_lower_bound, m_upper_bound);
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(
    const std::string& value)
{
  (void)value;
  throw RuntimeException("Visitor for string is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(
    const std::vector<double>& value)
{
  (void)value;
  throw RuntimeException("Visitor for vector<double> is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(
    const ComboProperty& value)
{
  (void)value;
  throw RuntimeException("Visitor for ComboProperty is not implemented");
}

template <typename LimitsT>
inline std::pair<LimitsT, LimitsT> VariantLimitsVisitor<LimitsT>::operator()(
    const ExternalProperty& value)
{
  (void)value;
  throw RuntimeException("Visitor for ExternalProperty is not implemented");
}

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_INTEGER_HELPER_H_
