/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_COMBO_LIMITS_H_
#define MVVM_MODEL_COMBO_LIMITS_H_

#include <mvvm/model_export.h>

#include <limits>
#include <optional>

namespace mvvm
{

template <typename T>
class Limits
{
public:
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

private:
  Limits(const std::optional<T>& lower_limit, const std::optional<T>& upper_limit)
      : m_lower_limit(lower_limit), m_upper_limit(upper_limit)
  {
  }

  std::optional<T> m_lower_limit;
  std::optional<T> m_upper_limit;
};

//! Returns true if an object has a lower limit defined.
template <typename T>
bool Limits<T>::HasLowerLimit() const
{
  return m_lower_limit;
}

//! Returns true if an object has an upper limit defined.
template <typename T>
bool Limits<T>::HasUpperLimit() const
{
  return m_upper_limit;
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
  const auto poszero = std::numeric_limits<T>::min();
  return HasLowerLimit() && !HasUpperLimit() && GetLowerLimit() == poszero;
}

//! Returns true if object is defined for all non-negative values (0 included).
template <typename T>
bool Limits<T>::IsNonnegative() const
{
  return HasLowerLimit() && !HasUpperLimit() && GetLowerLimit() == 0;
}

//! Returns true uf an object has lower limit set.
template <typename T>
bool Limits<T>::IsLowerLimited() const
{
  return !IsPositive() && !IsNonnegative() && HasLowerLimit() && !HasUpperLimit();
}

//! Returns true uf an object has upper limit set.
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

}  // namespace mvvm

#endif  // MVVM_MODEL_COMBO_LIMITS_H_
