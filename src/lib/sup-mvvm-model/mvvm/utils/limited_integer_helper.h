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
 */
std::unique_ptr<ILimitedInteger> CreateLimitedInteger(const variant_t& value,
                                                      const variant_t& lower_bound,
                                                      const variant_t& upper_bound);

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_INTEGER_HELPER_H_
