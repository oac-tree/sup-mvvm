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

#include "variant_limits_helper.h"

#include <limits>

namespace
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

  if constexpr (std::is_same_v<ActualT, mvvm::float64>)
  {
    return {static_cast<OutputT>(std::numeric_limits<ActualT>::lowest()), max};
  }

  if constexpr (std::is_same_v<ActualT, mvvm::float32>)
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
std::pair<OutputLimitsT, OutputLimitsT> GetMinMaxLimitsPair(const mvvm::variant_t& lower_bound,
                                                            const mvvm::variant_t& upper_bound)
{
  auto result = GetNumericMinMaxPair<OutputLimitsT, ActualLimitsT>();

  if (mvvm::utils::IsValid(lower_bound))
  {
    result.first = static_cast<OutputLimitsT>(std::get<ActualLimitsT>(lower_bound));
  }

  if (mvvm::utils::IsValid(upper_bound))
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
 * @details Reported limits will obtained by the combination of user defined limits and numeric
 * limits. If one of user limits is not set, std::numeric_limit::min() and std::numeric_limit::max()
 * will be used for lower and upper bounds.
 *
 * @details Deduced limits will be casted to LimitsT parameter.
 */

template <typename LimitsT>
struct VariantLimits32Visitor
{
  std::pair<LimitsT, LimitsT> operator()(std::monostate value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for monostate is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::boolean value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for boolean is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::char8 value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for char8 is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::int8 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::int8>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::uint8 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::uint8>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::int16 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::int16>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::uint16 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::uint16>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::int32 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::int32>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::uint32 value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for uint32 is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::int64 value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for int64 is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::uint64 value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for uint64 is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::float32 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::float32>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(mvvm::float64 value)
  {
    (void)value;
    return GetMinMaxLimitsPair<LimitsT, mvvm::float64>(m_lower_bound, m_upper_bound);
  }

  std::pair<LimitsT, LimitsT> operator()(const std::string& value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for string is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(const std::vector<double>& value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for vector<double> is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(const mvvm::ComboProperty& value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for ComboProperty is not implemented");
  }

  std::pair<LimitsT, LimitsT> operator()(const mvvm::ExternalProperty& value)
  {
    (void)value;
    throw mvvm::RuntimeException("Visitor for ExternalProperty is not implemented");
  }

  mvvm::variant_t m_lower_bound;
  mvvm::variant_t m_upper_bound;
};

}  // namespace

namespace mvvm
{

std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value, const variant_t& lower_bound,
                                          const variant_t& upper_bound)
{
  VariantLimits32Visitor<int> visitor;
  visitor.m_lower_bound = lower_bound;
  visitor.m_upper_bound = upper_bound;

  return std::visit(visitor, value);
}

std::pair<double, double> GetFloat64MinMaxNumeric(const variant_t& value,
                                                  const variant_t& lower_bound,
                                                  const variant_t& upper_bound)
{
  VariantLimits32Visitor<double> visitor;
  visitor.m_lower_bound = lower_bound;
  visitor.m_upper_bound = upper_bound;

  return std::visit(visitor, value);
}

}  // namespace mvvm
