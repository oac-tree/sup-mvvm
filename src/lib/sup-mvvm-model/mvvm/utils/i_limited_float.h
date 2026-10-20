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

#ifndef MVVM_UTILS_I_LIMITED_FLOAT_H_
#define MVVM_UTILS_I_LIMITED_FLOAT_H_

#include <mvvm/core/variant.h>

#include <optional>
#include <string>

namespace mvvm
{

/**
 * @brief The ILimitedFloat is an interface class to represent floating point number with limits.
 */
class ILimitedFloat
{
public:
  ILimitedFloat() = default;
  virtual ~ILimitedFloat() = default;

  ILimitedFloat(const ILimitedFloat&) = delete;
  ILimitedFloat& operator=(const ILimitedFloat&) = delete;
  ILimitedFloat(ILimitedFloat&&) = delete;
  ILimitedFloat& operator=(ILimitedFloat&&) = delete;

  /**
   * @brief Returns true if the number can have no value.
   */
  virtual bool IsNullable() const = 0;

  /**
   * @brief Returns true if the number has a value.
   */
  virtual bool HasValue() const = 0;

  /**
   * @brief Removes the value, returns true if the value was removed.
   *
   * @details Fails for a non-nullable number and for a number without value.
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
   * @details Invalid variant removes the value of a nullable number.
   */
  virtual bool SetValueFromVariant(const variant_t& variant) = 0;

  /**
   * @brief Returns the value as a string, empty string if there is no value.
   *
   * @param precision Number of digits after decimal point.
   * @param scientific Exponential notation if true, fixed notation otherwise.
   */
  virtual std::string GetValueAsText(std::int32_t precision, bool scientific) const = 0;

  /**
   * @brief Sets the value from text, returns true if the value was changed.
   *
   * @details Empty text removes the value of a nullable number.
   */
  virtual bool SetValueFromText(const std::string& text) = 0;

  /**
   * @brief Returns the value parsed from text.
   *
   * @details std::nullopt means the text is not a valid input, so the caller keeps the current
   * value. An empty variant_t{} is a special case that appears for nullable numbers when empty text
   * is given. It denotes that the value was cleared and an editor should display it as reset to
   * default.
   *
   * @details Accepts fixed and exponential notation.
   */
  virtual std::optional<variant_t> ParseText(const std::string& text) const = 0;

  /**
   * @brief Returns the step used by StepBy.
   */
  virtual double GetSingleStep() const = 0;

  /**
   * @brief Sets the step used by StepBy.
   */
  virtual void SetSingleStep(double step) = 0;

  /**
   * @brief Performs number of steps.
   *
   * @param steps Number of increments (if positive) or number of decrements (if negative).
   * @return True if the value was changed.
   *
   * @details The result is forced into bounds. The first step from no value sets the default
   * value.
   */
  virtual bool StepBy(std::int32_t steps) = 0;

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

#endif  // MVVM_UTILS_I_LIMITED_FLOAT_H_
