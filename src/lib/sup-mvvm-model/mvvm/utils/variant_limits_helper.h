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

#ifndef MVVM_UTILS_VARIANT_LIMITS_HELPER_H_
#define MVVM_UTILS_VARIANT_LIMITS_HELPER_H_

//! @file
//! Helper methods for variant limits.

#include <mvvm/core/exceptions.h>
#include <mvvm/core/variant.h>

namespace mvvm
{

/**
 * @brief Returns pairs of integers representing limits of stored type.
 *
 * @param value The value for which we have to find min and max values.
 * @param lower_bound User defined additional lower bound.
 * @param upper_bound User defined additional upper bound.
 *
 * @details Used in the context of integer editors. It is implemented only for types which we can
 * fit into int32: int8, uint8, int16, uint16, int32. When no user defined limits are provided, will
 * return limits deduced from std::numeric.
 */
std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value, const variant_t& lower_bound = {},
                                          const variant_t& upper_bound = {});

std::pair<double, double> GetFloat64MinMaxNumeric(const variant_t& value,
                                                  const variant_t& lower_bound = {},
                                                  const variant_t& upper_bound = {});

}  // namespace mvvm

#endif  // MVVM_UTILS_VARIANT_LIMITS_HELPER_H_
