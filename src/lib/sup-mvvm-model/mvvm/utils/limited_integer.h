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

  /**
   * @brief Adds the value to the current value.
   *
   * @param value The value to add.
   * @param as_much_as_possible When item is close to its bound, will add as mush as possible and
   * report success.
   *
   * @return True in the case of success (when current value has changed).
   *
   * @details When as_much_as_possible is false the behavior is the following: the method can fail
   * if the value is close to the upper bound and adding would lead to overflow. In this case will
   * return false, the value will remain unchanged.
   *
   * @details If as_much_as_possible is true, and adding the value could lead to overflow, will
   stop
   * at the upper bound, and report success.
   */
  bool AddValue(const variant_t& value, bool as_much_as_possible);

  /**
   * @brief Substracts the value from the current value.
   *
   * @param value The value to substract.
   * @param as_much_as_possible When item is close to its bound, will substract as mush as possible
   * and report success.
   *
   * @return True in the case of success (when current value has changed).
   *
   * @details When as_much_as_possible is false the behavior is the following: the method can fail
   * if the value is close to the lower bound and adding would lead to underflow. In this case will
   * return false, the value will remain unchanged.
   *
   * @details If as_much_as_possible is true, and substracting the value could lead to underflow,
   * will stop at the lower bound, and report success.
   */
  bool SubstractValue(const variant_t& value, bool as_much_as_possible);

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
  T parsed_value;
  std::from_chars_result result =
      std::from_chars(text.data(), text.data() + text.size(), parsed_value);

  if (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
  {
    return false;
  }

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
  if (steps > 0)
  {
    // converting steps to current type
    T n_steps = steps > std::numeric_limits<T>::max() ? std::numeric_limits<T>::max()
                                                      : static_cast<T>(steps);
    // avoiding using the sum to not to overflow
    T diff = m_upper_bound - m_value;

    // increasing the value, but not more than the maximum
    return SetValue(diff > n_steps ? m_value + n_steps : m_upper_bound);
  }

  if (steps < 0)
  {
    // converting steps to current type
    T n_steps = steps < std::numeric_limits<T>::min() ? std::numeric_limits<T>::min()
                                                      : static_cast<T>(steps);
    // avoiding using the sum to not to overflow
    T diff = m_value - m_lower_bound;

    // decreasing the value, but not less than the minimum
    return SetValue(diff < std::abs(n_steps) ? m_lower_bound : m_value + n_steps);
  }

  return false;
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
inline bool LimitedInteger<T>::AddValue(const variant_t& value, bool as_much_as_possible)
{
  return false;
}

template <typename T>
inline bool LimitedInteger<T>::SubstractValue(const variant_t& value, bool as_much_as_possible)
{
  return false;
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
