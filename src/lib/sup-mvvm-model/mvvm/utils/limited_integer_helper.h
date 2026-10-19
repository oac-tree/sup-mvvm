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

#ifndef MVVM_UTILS_LIMITED_INTEGER_HELPER_H_
#define MVVM_UTILS_LIMITED_INTEGER_HELPER_H_

#include <mvvm/core/variant.h>
#include <mvvm/utils/i_limited_integer.h>

#include <memory>

namespace mvvm
{

/**
 * @brief Creates limited integer.
 *
 * @param value The value of an integer.
 * @param lower_bound Lower bound
 * @param upper_bound Upper boud.
 *
 * @return Unique ptr with limited integer.
 *
 * @details Throws if the value is not an integer, or if bounds are of another type.
 */
std::unique_ptr<ILimitedInteger> CreateLimitedInteger(const variant_t& value,
                                                      const variant_t& lower_bound,
                                                      const variant_t& upper_bound);

/**
 * @brief Creates nullable limited integer.
 *
 * @param value The value of an integer, can be default-constructed.
 * @param default_value The value set by the first step from no value. Defines the type.
 * @param lower_bound Lower bound.
 * @param upper_bound Upper bound.
 *
 * @return Unique ptr with limited integer.
 *
 * @details Throws if the default value is not an integer, or if the value or bounds are of another
 * type.
 */
std::unique_ptr<ILimitedInteger> CreateNullableLimitedInteger(const variant_t& value,
                                                              const variant_t& default_value,
                                                              const variant_t& lower_bound,
                                                              const variant_t& upper_bound);

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_INTEGER_HELPER_H_
