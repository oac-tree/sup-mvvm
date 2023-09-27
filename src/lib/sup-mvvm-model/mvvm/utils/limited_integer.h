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

#ifndef MVVM_UTILS_LIMITED_INTEGER_H
#define MVVM_UTILS_LIMITED_INTEGER_H

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/i_limited_integer.h>

#include <algorithm>
#include <charconv>
#include <iostream>
#include <vector>

namespace mvvm
{

/**
 * @brief The LimitedInteger class represents an integer number with limits.
 *
 * @details It can be one of (int8, uin8, int16, uin16, int32, uint32, int64, uin64), as supported
 * by our mvvm::variant_t type. The class provides aditional machinery for safe increment and
 * decrement. It is used in the context of spin box editors.
 */

template <typename T>
class LimitedInteger : public ILimitedInteger
{
public:
  /**
   * @brief Main constructor.
   *
   * @param value The value.
   * @param lower_bound Lower bound.
   * @param upper_bound Upper bound.
   *
   * @details The type of all 3 variants should be the same (except the case with bounds are absent)
   * and represent one of int8, uin8, int16, uin16, int32, uint32, int64, uint64.
   *
   * @details Lower_bound and upper_bound can be default-constructed. In this case all
   * limits will be deduced from std::numeric_limits.
   */
  LimitedInteger(const variant_t& value, const variant_t& lower_bound,
                 const variant_t& upper_bound);

  /**
   * @brief Returns current value.
   */
  T GetValue() const;

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
   * @details Can fail if variant has different type then stored value, or new value is
   * located outside of bounds. In this case will return false.
   */
  bool SetValue(const T& value);

  variant_t GetValueAsVariant() const override;

  bool SetValueFromVariant(const variant_t& value) override;

  std::string GetValueAsText() const override;

  bool SetValueFromText(const std::string& text) override;

  bool Increment() override;

  bool Decrement() override;

  bool StepBy(int steps) override;

  bool IsAtMinimum() const override;

  bool IsAtMaximum() const override;

private:
  bool IsSupportedVariant(const variant_t& variant) const;

  T m_value;
  T m_lower_bound;
  T m_upper_bound;
};

template <typename T>
inline LimitedInteger<T>::LimitedInteger(const variant_t& value, const variant_t& lower_bound,
                                         const variant_t& upper_bound)
{
  if (!IsSupportedVariant(value))
  {
    throw RuntimeException("The value type [" + utils::TypeName(value) + "] is not supported");
  }

  if (utils::IsValid(lower_bound) && lower_bound.index() != value.index())
  {
    throw RuntimeException("The lower bound type=[" + utils::TypeName(lower_bound)
                           + "] doesn't coincide with value type=[" + utils::TypeName(lower_bound)
                           + "]");
  }

  if (utils::IsValid(upper_bound) && upper_bound.index() != value.index())
  {
    throw RuntimeException("The upper bound type=[" + utils::TypeName(upper_bound)
                           + "] doesn't coincide with value type=[" + utils::TypeName(upper_bound)
                           + "]");
  }

  m_value = std::get<T>(value);
  m_lower_bound =
      utils::IsValid(lower_bound) ? std::get<T>(lower_bound) : std::numeric_limits<T>::min();
  m_upper_bound =
      utils::IsValid(upper_bound) ? std::get<T>(upper_bound) : std::numeric_limits<T>::max();

  if (m_upper_bound < m_lower_bound)
  {
    throw RuntimeException("Upper bound is smaller than lower bound");
  }

  if (m_value < m_lower_bound)
  {
    m_value = m_lower_bound;
  }

  if (m_value > m_upper_bound)
  {
    m_value = m_upper_bound;
  }
}

template <typename T>
inline T LimitedInteger<T>::GetValue() const
{
  return m_value;
}

template <typename T>
inline T LimitedInteger<T>::GetLowerBound() const
{
  return m_lower_bound;
}

template <typename T>
inline T LimitedInteger<T>::GetUpperBound() const
{
  return m_upper_bound;
}

template <typename T>
inline bool LimitedInteger<T>::SetValue(const T& value)
{
  std::cout << "LimitedInteger<T>::SetValue "
            << " m_value " << m_value << " value " << value << " m_lower_bound " << m_lower_bound
            << " m_upper_bound " << m_upper_bound << " " << std::endl;

  if (value < m_lower_bound || value > m_upper_bound || value == m_value)
  {
    return false;
  }

  m_value = value;
  return true;
}

template <typename T>
inline variant_t LimitedInteger<T>::GetValueAsVariant() const
{
  return variant_t{m_value};
}

template <typename T>
inline bool LimitedInteger<T>::SetValueFromVariant(const variant_t& value)
{
  return SetValue(std::get<T>(value));
}

template <typename T>
inline std::string LimitedInteger<T>::GetValueAsText() const
{
  return std::to_string(m_value);
}

template <typename T>
inline bool LimitedInteger<T>::SetValueFromText(const std::string& text)
{
  std::cout << "Limited Integer " << text << std::endl;
  T parsed_value;
  std::from_chars_result result =
      std::from_chars(text.data(), text.data() + text.size(), parsed_value);

  if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
  {
    return false;
  }

  // FIXME Another behavior is necessary. If text value is outside of allowed range, it should be
  // forced to one of the bounds.

  std::cout << "Limited Integer 1.4 " << std::endl;
  return SetValue(parsed_value);
}

template <typename T>
inline bool LimitedInteger<T>::Increment()
{
  if (m_value >= m_upper_bound)
  {
    return false;
  }

  m_value++;
  return true;
}

template <typename T>
inline bool LimitedInteger<T>::Decrement()
{
  if (m_value <= m_lower_bound)
  {
    return false;
  }

  m_value--;
  return true;
}

template <typename T>
inline bool LimitedInteger<T>::StepBy(int steps)
{
  // This is a simplified implementation which is not very efficient for large amount of steps,
  // but it guarantees that no bit flip can happen with any comparison/add/substract operations
  // applied to any possible values of `steps` and
  // INT_MIN...lower_bound....0...value...upper_bound...INT_MAX.

  bool value_was_changed{false};
  for (size_t step = 0; step < std::abs(steps); ++step)
  {
    bool value_changed = steps > 0 ? Increment() : Decrement();
    if (!value_changed)
    {
      break;
    }
    {
      value_was_changed = true;
    }
  }
  return value_was_changed;
}

template <typename T>
inline bool LimitedInteger<T>::IsAtMinimum() const
{
  return m_value == m_lower_bound;
}

template <typename T>
inline bool LimitedInteger<T>::IsAtMaximum() const
{
  return m_value == m_upper_bound;
}

template <typename T>
inline bool LimitedInteger<T>::IsSupportedVariant(const variant_t& variant) const
{
  static const std::vector<mvvm::TypeCode> kSupportedTypes{
      mvvm::TypeCode::Int8,  mvvm::TypeCode::UInt8,  mvvm::TypeCode::Int16, mvvm::TypeCode::UInt16,
      mvvm::TypeCode::Int32, mvvm::TypeCode::UInt32, mvvm::TypeCode::Int64, mvvm::TypeCode::UInt64};

  return std::find(kSupportedTypes.begin(), kSupportedTypes.end(), GetTypeCode(variant))
         != kSupportedTypes.end();
}

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_INTEGER_H
