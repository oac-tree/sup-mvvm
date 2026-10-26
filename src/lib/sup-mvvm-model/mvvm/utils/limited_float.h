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

#ifndef MVVM_UTILS_LIMITED_FLOAT_H_
#define MVVM_UTILS_LIMITED_FLOAT_H_

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/i_limited_float.h>
#include <mvvm/utils/string_utils.h>

#include <algorithm>
#include <limits>
#include <optional>

namespace mvvm
{

/**
 * @brief The LimitedFloat class represents a floating point number with limits.
 *
 * @details It can be float32 or float64, as supported by our mvvm::variant_t type. The class
 * provides conversion to and from text and steps within bounds. It is used in the context of spin
 * box editors.
 *
 * @details A nullable number can have no value. The first step from no value sets the default
 * value.
 */
template <typename T>
class LimitedFloat : public ILimitedFloat
{
public:
  /**
   * @brief Constructor of a non-nullable number.
   *
   * @param value The value.
   * @param lower_bound Lower bound, the lowest value of the type if absent.
   * @param upper_bound Upper bound, the maximum of the type if absent.
   *
   * @details The value is forced into bounds. Throws if the upper bound is smaller than the lower
   * bound.
   */
  LimitedFloat(T value, std::optional<T> lower_bound, std::optional<T> upper_bound);

  /**
   * @brief Constructor of a nullable number.
   *
   * @param value The value, can be absent.
   * @param default_value The value set by the first step from no value.
   * @param lower_bound Lower bound, the lowest value of the type if absent.
   * @param upper_bound Upper bound, the maximum of the type if absent.
   *
   * @details The value and the default value are forced into bounds. Throws if the upper bound is
   * smaller than the lower bound.
   */
  LimitedFloat(std::optional<T> value, T default_value, std::optional<T> lower_bound,
               std::optional<T> upper_bound);

  /**
   * @brief Returns current value, empty if there is no value.
   */
  std::optional<T> GetValue() const;

  /**
   * @brief Returns the default value.
   */
  T GetDefaultValue() const;

  /**
   * @brief Returns lower bound.
   */
  T GetLowerBound() const;

  /**
   * @brief Returns upper bound.
   */
  T GetUpperBound() const;

  /**
   * @brief Sets the value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Fails if the value is outside of bounds.
   */
  bool SetValue(T value);

  bool IsNullable() const override;

  bool HasValue() const override;

  bool ResetValue() override;

  variant_t GetValueAsVariant() const override;

  variant_t GetLowerBoundAsVariant() const override;

  variant_t GetUpperBoundAsVariant() const override;

  bool SetValueFromVariant(const variant_t& value) override;

  std::string GetValueAsText(std::int32_t precision, bool scientific) const override;

  bool SetValueFromText(const std::string& text) override;

  std::optional<variant_t> ParseText(const std::string& text) const override;

  double GetSingleStep() const override;

  void SetSingleStep(double step) override;

  bool StepBy(std::int32_t steps) override;

  bool IsAtMinimum() const override;

  bool IsAtMaximum() const override;

private:
  LimitedFloat(std::optional<T> value, T default_value, std::optional<T> lower_bound,
               std::optional<T> upper_bound, bool is_nullable);

  /**
   * @brief Returns true if the given value is within bounds.
   *
   * @details Takes double, so a parsed value is checked before the cast to T.
   */
  bool IsInBounds(double value) const;

  std::optional<T> m_value;
  T m_default_value{};
  T m_lower_bound{};
  T m_upper_bound{};
  double m_step{1.0};
  bool m_is_nullable{false};
};

template <typename T>
inline LimitedFloat<T>::LimitedFloat(T value, std::optional<T> lower_bound,
                                     std::optional<T> upper_bound)
    : LimitedFloat(value, value, lower_bound, upper_bound, false)
{
}

template <typename T>
inline LimitedFloat<T>::LimitedFloat(std::optional<T> value, T default_value,
                                     std::optional<T> lower_bound, std::optional<T> upper_bound)
    : LimitedFloat(value, default_value, lower_bound, upper_bound, true)
{
}

template <typename T>
inline LimitedFloat<T>::LimitedFloat(std::optional<T> value, T default_value,
                                     std::optional<T> lower_bound, std::optional<T> upper_bound,
                                     bool is_nullable)
    : ILimitedFloat()
    , m_lower_bound(lower_bound.value_or(std::numeric_limits<T>::lowest()))
    , m_upper_bound(upper_bound.value_or(std::numeric_limits<T>::max()))
    , m_is_nullable(is_nullable)
{
  if (m_upper_bound < m_lower_bound)
  {
    throw RuntimeException("Upper bound is smaller than lower bound");
  }

  m_default_value = std::clamp(default_value, m_lower_bound, m_upper_bound);

  if (value.has_value())
  {
    m_value = std::clamp(value.value(), m_lower_bound, m_upper_bound);
  }
}

template <typename T>
inline std::optional<T> LimitedFloat<T>::GetValue() const
{
  return m_value;
}

template <typename T>
inline T LimitedFloat<T>::GetDefaultValue() const
{
  return m_default_value;
}

template <typename T>
inline T LimitedFloat<T>::GetLowerBound() const
{
  return m_lower_bound;
}

template <typename T>
inline T LimitedFloat<T>::GetUpperBound() const
{
  return m_upper_bound;
}

template <typename T>
inline bool LimitedFloat<T>::SetValue(T value)
{
  if (!IsInBounds(static_cast<double>(value)) || (m_value == value))
  {
    return false;
  }

  m_value = value;
  return true;
}

template <typename T>
inline bool LimitedFloat<T>::IsNullable() const
{
  return m_is_nullable;
}

template <typename T>
inline bool LimitedFloat<T>::HasValue() const
{
  return m_value.has_value();
}

template <typename T>
inline bool LimitedFloat<T>::ResetValue()
{
  if (!m_is_nullable || !m_value.has_value())
  {
    return false;
  }

  m_value.reset();
  return true;
}

template <typename T>
inline variant_t LimitedFloat<T>::GetValueAsVariant() const
{
  return m_value.has_value() ? variant_t{m_value.value()} : variant_t{};
}

template <typename T>
inline variant_t LimitedFloat<T>::GetLowerBoundAsVariant() const
{
  return variant_t{m_lower_bound};
}

template <typename T>
inline variant_t LimitedFloat<T>::GetUpperBoundAsVariant() const
{
  return variant_t{m_upper_bound};
}

template <typename T>
inline bool LimitedFloat<T>::SetValueFromVariant(const variant_t& value)
{
  if (!utils::IsValid(value))
  {
    return ResetValue();
  }

  return SetValue(std::get<T>(value));
}

template <typename T>
inline std::string LimitedFloat<T>::GetValueAsText(std::int32_t precision, bool scientific) const
{
  if (!m_value.has_value())
  {
    return {};
  }

  const auto value = static_cast<double>(m_value.value());
  return scientific ? utils::ScientificDoubleToString(value, precision)
                    : utils::FixedDoubleToString(value, precision);
}

template <typename T>
inline bool LimitedFloat<T>::SetValueFromText(const std::string& text)
{
  const auto parsed_value = ParseText(text);
  return parsed_value.has_value() && SetValueFromVariant(parsed_value.value());
}

template <typename T>
inline std::optional<variant_t> LimitedFloat<T>::ParseText(const std::string& text) const
{
  if (text.empty())
  {
    return m_is_nullable ? std::optional<variant_t>{variant_t{}} : std::nullopt;
  }

  // casting a double outside the float32 range to float32 is undefined, so check bounds first
  const auto parsed_value = utils::StringToDouble(text);
  if (!parsed_value.has_value() || !IsInBounds(parsed_value.value()))
  {
    return std::nullopt;
  }

  return variant_t{static_cast<T>(parsed_value.value())};
}

template <typename T>
inline double LimitedFloat<T>::GetSingleStep() const
{
  return m_step;
}

template <typename T>
inline void LimitedFloat<T>::SetSingleStep(double step)
{
  m_step = step;
}

template <typename T>
inline bool LimitedFloat<T>::StepBy(std::int32_t steps)
{
  if (steps == 0)
  {
    return false;
  }

  if (!m_value.has_value())
  {
    m_value = m_default_value;
    return true;
  }

  // computed in double, the clamp handles an overflow to infinity
  const double new_value =
      static_cast<double>(m_value.value()) + (m_step * static_cast<double>(steps));
  const auto result = static_cast<T>(std::clamp(new_value, static_cast<double>(m_lower_bound),
                                                static_cast<double>(m_upper_bound)));
  return SetValue(result);
}

template <typename T>
inline bool LimitedFloat<T>::IsAtMinimum() const
{
  return m_value == m_lower_bound;
}

template <typename T>
inline bool LimitedFloat<T>::IsAtMaximum() const
{
  return m_value == m_upper_bound;
}

template <typename T>
inline bool LimitedFloat<T>::IsInBounds(double value) const
{
  return (value >= static_cast<double>(m_lower_bound))
         && (value <= static_cast<double>(m_upper_bound));
}

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_FLOAT_H_
