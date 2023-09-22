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

#include <mvvm/core/variant.h>
#include <mvvm/model_export.h>

namespace mvvm
{

/**
 * @brief The LimitedInteger class represents an integer number with limits.
 *
 * @details It can be one of (int8, uin8, int16, uin16, int32, uint32, int64, uin64), as supported
 * by our mvvm::variant_t type. The class provides aditional machinery for safe increment and
 * decrement. It is used in the context of spin box editors.
 */

class LimitedInteger
{
public:
  /**
   * @brief Main constructor.
   *
   * @param value The value.
   * @param lower_bound Lower bound.
   * @param upper_bound Upper bound.
   *
   * @details The type of all 3 variants should be the same and represent one of int8, uin8, int16,
   * uin16, int32, uint32, int64, uin64.
   *
   * @details Although, lower_bound and upper_bound can be default-constructed. In this case all
   * limits will be deduced from std::numeric_limits.
   */
  LimitedInteger(const variant_t& value, const variant_t& lower_bound,
                 const variant_t& upper_bound);

  /**
   * @brief Returns current value.
   */
  variant_t GetValue() const;

  /**
   * @brief Sets the value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if variant has different type then stored value, or new value is
   * located outside of bounds. In this case will return false.
   */
  bool SetValue(const variant_t& value);

  /**
   * @brief Increments internal value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if current value is at the upper boundary already. In this case current value
   * remain unchanged. Call is safe for all types of integers, no overflow can occur.
   */
  bool Increment();

  /**
   * @brief Decrements internal value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if current value is at the lower boundary already. In this case current value
   * remain unchanged. Call is safe for all types of integers, no underflow can occur.
   */
  bool Decrement();

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
   * @details If as_much_as_possible is true, and adding the value could lead to overflow, will stop
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
  variant_t m_value;
  variant_t m_lower_bound;
  variant_t m_upper_bound;
};

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_INTEGER_H
