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

#ifndef MVVM_MODEL_LIMITS_H_
#define MVVM_MODEL_LIMITS_H_

#include <mvvm/model_export.h>

#include <limits>
#include <optional>

namespace mvvm
{

//! Defines limits for scalars to bound its values to certain range.
template <typename T>
class Limits
{
public:
  Limits();

  static Limits<T> CreateLowerLimited(T bound_value);
  static Limits<T> CreatePositive();
  static Limits<T> CreateNonnegative();
  static Limits<T> CreateUpperLimited(T bound_value);
  static Limits<T> CreateLimited(T left_bound_value, T right_bound_value);
  static Limits<T> CreateLimitless();

  bool HasLowerLimit() const;
  bool HasUpperLimit() const;
  T GetLowerLimit() const;
  T GetUpperLimit() const;
  bool IsInRange(T value) const;

  bool IsLimitless() const;
  bool IsPositive() const;
  bool IsNonnegative() const;
  bool IsLowerLimited() const;
  bool IsUpperLimited() const;
  bool IsLimited() const;

  bool operator==(const Limits<T>& other) const;
  bool operator!=(const Limits<T>& other) const;

private:
  Limits(const std::optional<T>& lower_limit, const std::optional<T>& upper_limit)
      : m_lower_limit(lower_limit), m_upper_limit(upper_limit)
  {
  }

  std::optional<T> m_lower_limit;
  std::optional<T> m_upper_limit;
};

using RealLimits = Limits<double>;
using IntLimits = Limits<int>;

//! Default constructor for limitless object.
template <typename T>
Limits<T>::Limits() : Limits({}, {})
{
}

//! Creates an object bounded from the left.
template <typename T>
Limits<T> Limits<T>::CreateLowerLimited(T bound_value)
{
  return Limits<T>(bound_value, {});
}

//! Creates an object which can have only positive values (zero is not included).
template <typename T>
Limits<T> Limits<T>::CreatePositive()
{
  if constexpr (std::is_same_v<T, double>)
    return CreateLowerLimited(std::numeric_limits<T>::min());
  else
    return CreateLowerLimited(1);
}

//! Creates an object which can have only positive values with zero included.
template <typename T>
Limits<T> Limits<T>::CreateNonnegative()
{
  return CreateLowerLimited(0);
}

//! Creates an object bounded from the right.
template <typename T>
Limits<T> Limits<T>::CreateUpperLimited(T bound_value)
{
  return Limits<T>({}, bound_value);
}

//! Creates an object bounded from the left and right.
template <typename T>
Limits<T> Limits<T>::CreateLimited(T left_bound_value, T right_bound_value)
{
  return Limits<T>(left_bound_value, right_bound_value);
}

//! Creates an object without bounds (equivalent of default-constructed object).
template <typename T>
Limits<T> Limits<T>::CreateLimitless()
{
  return Limits<T>({}, {});
}

//! Returns true if an object has a lower limit defined.
template <typename T>
bool Limits<T>::HasLowerLimit() const
{
  return m_lower_limit.has_value();
}

//! Returns true if an object has an upper limit defined.
template <typename T>
bool Limits<T>::HasUpperLimit() const
{
  return m_upper_limit.has_value();
}

//! Returns lower limit value. Will throw if the value is not defined.
template <typename T>
T Limits<T>::GetLowerLimit() const
{
  return m_lower_limit.value();
}

//! Returns upper limit value. Will throw if the value is not defined.
template <typename T>
T Limits<T>::GetUpperLimit() const
{
  return m_upper_limit.value();
}

//! Returns true if proposed value is in limits range (lower_limit <= value < upper_limit).
template <typename T>
bool Limits<T>::IsInRange(T value) const
{
  if (HasLowerLimit() && value < m_lower_limit.value())
  {
    return false;
  }
  if (HasUpperLimit() && value >= m_upper_limit.value())
  {
    return false;
  }
  return true;
}

//! Returns true if there are no limits set.
template <typename T>
bool Limits<T>::IsLimitless() const
{
  return !HasLowerLimit() && !HasUpperLimit();
}

//! Returns true if object is defined for all positive values (0 excluded).
template <typename T>
bool Limits<T>::IsPositive() const
{
  if constexpr (std::is_same_v<T, double>)
    return HasLowerLimit() && !HasUpperLimit() && GetLowerLimit() == std::numeric_limits<T>::min();
  else
    return HasLowerLimit() && !HasUpperLimit() && GetLowerLimit() == 1;
}

//! Returns true if object is defined for all non-negative values (0 included).
template <typename T>
bool Limits<T>::IsNonnegative() const
{
  return HasLowerLimit() && !HasUpperLimit() && GetLowerLimit() == 0;
}

//! Returns true if an object has lower limit set.
template <typename T>
bool Limits<T>::IsLowerLimited() const
{
  return !IsPositive() && !IsNonnegative() && HasLowerLimit() && !HasUpperLimit();
}

//! Returns true if an object has upper limit set.
template <typename T>
bool Limits<T>::IsUpperLimited() const
{
  return !HasLowerLimit() && HasUpperLimit();
}

//! Returns true uf an object has lower and upper limits set.
template <typename T>
bool Limits<T>::IsLimited() const
{
  return HasLowerLimit() && HasUpperLimit();
}

template <typename T>
bool Limits<T>::operator==(const Limits<T>& other) const
{
  return m_lower_limit == other.m_lower_limit && m_upper_limit == other.m_upper_limit;
}

template <typename T>
bool Limits<T>::operator!=(const Limits<T>& other) const
{
  return !(*this == other);
}

}  // namespace mvvm

#endif  // MVVM_MODEL_LIMITS_H_
