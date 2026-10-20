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

#ifndef MVVM_UTILS_I_LIMITED_INTEGER_H_
#define MVVM_UTILS_I_LIMITED_INTEGER_H_

#include <mvvm/core/variant.h>

#include <optional>
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
   * @brief Returns true if the integer can have no value.
   */
  virtual bool IsNullable() const = 0;

  /**
   * @brief Returns true if the integer has a value.
   */
  virtual bool HasValue() const = 0;

  /**
   * @brief Removes the value, returns true if the value was removed.
   *
   * @details Fails for a non-nullable integer and for an integer without value.
   */
  virtual bool ResetValue() = 0;

  /**
   * @brief Returns the value as variant, invalid variant if there is no value.
   */
  virtual variant_t GetValueAsVariant() const = 0;

  /**
   * @brief Returns lower bound as variant.
   */
  virtual variant_t GetLowerBoundAsVariant() const = 0;

  /**
   * @brief Returns upper bound as variant.
   */
  virtual variant_t GetUpperBoundAsVariant() const = 0;

  /**
   * @brief Sets the value from variant, returns true if the value was changed.
   *
   * @details Invalid variant removes the value of a nullable integer.
   */
  virtual bool SetValueFromVariant(const variant_t& variant) = 0;

  /**
   * @brief Returns the value as a string, empty string if there is no value.
   */
  virtual std::string GetValueAsText() const = 0;

  /**
   * @brief Sets the value from text, returns true if the value was changed.
   *
   * @details Empty text removes the value of a nullable integer.
   */
  virtual bool SetValueFromText(const std::string& text) = 0;

  /**
   * @brief Returns the value parsed from text.
   *
   * @details std::nullopt means the text is not a valid input, so the caller keeps the current
   * value. An empty variant_t{} is a special case that appears for nullable numbers when empty text
   * is given. It denotes that the value was cleared and an editor should display it as reset to
   * default.
   */
  virtual std::optional<variant_t> ParseText(const std::string& text) const = 0;

  /**
   * @brief Performs number of steps.
   *
   * @param steps Number of increments (if positive) or number of decrements (if negative).
   * @return True if the value was changed.
   *
   * @details The first step from no value sets the default value.
   */
  virtual bool StepBy(std::int32_t steps) = 0;

  /**
   * @brief Increments internal value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if current value is at the upper boundary already. In this case current value
   * remain unchanged. Call is safe for all types of integers, no overflow can occur. Sets the
   * default value if there is no value.
   */
  virtual bool Increment() = 0;

  /**
   * @brief Decrements internal value.
   * @return True in the case of success (when current value has changed).
   *
   * @details Can fail if current value is at the lower boundary already. In this case current value
   * remain unchanged. Call is safe for all types of integers, no underflow can occur. Sets the
   * default value if there is no value.
   */
  virtual bool Decrement() = 0;

  /**
   * @brief Returns true if the value coincides with lower bound, false if there is no value.
   */
  virtual bool IsAtMinimum() const = 0;

  /**
   * @brief Returns true if the value coincides with upper bound, false if there is no value.
   */
  virtual bool IsAtMaximum() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_UTILS_I_LIMITED_INTEGER_H_
