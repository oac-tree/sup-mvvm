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

#ifndef MVVM_UTILS_I_LIMITED_INTEGER_H_
#define MVVM_UTILS_I_LIMITED_INTEGER_H_

#include <mvvm/core/variant.h>
#include <mvvm/model_export.h>

#include <string>

namespace mvvm
{

/**
 * @brief The ILimitedInteger it's an interface class to represent integer number with limits.
 */

class ILimitedInteger
{
public:
  ILimitedInteger() = default;
  virtual ~ILimitedInteger() = default;

  ILimitedInteger(const ILimitedInteger&) = delete;
  ILimitedInteger& operator=(const ILimitedInteger&) = delete;
  ILimitedInteger(ILimitedInteger&&) = delete;
  ILimitedInteger& operator=(ILimitedInteger&&) = delete;

  /**
   * @brief Returns the value as variant.
   */
  virtual variant_t GetValueAsVariant() const = 0;

  /**
   * @brief Sets the value from variant, returns true if the value was changed.
   */
  virtual bool SetValueFromVariant(const variant_t& variant) = 0;

  /**
   * @brief Returns the value as a string.
   */
  virtual std::string GetValueAsText() const = 0;

  /**
   * @brief Sets the value from text, returns true if the value was changed.
   */
  virtual bool SetValueFromText(const std::string& text) = 0;

  /**
   * @brief Performs number of steps.
   *
   * @param steps Number of increments (if positive) or number of decrements (if negative).
   * @return True if the value was changed.
   */
  virtual bool StepBy(int steps) = 0;

  /**
   * @brief Increments internal value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if current value is at the upper boundary already. In this case current value
   * remain unchanged. Call is safe for all types of integers, no overflow can occur.
   */
  virtual bool Increment() = 0;

  /**
   * @brief Decrements internal value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if current value is at the lower boundary already. In this case current value
   * remain unchanged. Call is safe for all types of integers, no underflow can occur.
   */
  virtual bool Decrement() = 0;

  /**
   * @brief Returns true if the value coincides with lower bound.
   */
  virtual bool IsAtMinimum() const = 0;

  /**
   * @brief Returns true if the value coincides with lower bound.
   */
  virtual bool IsAtMaximum() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_UTILS_I_LIMITED_INTEGER_H_
