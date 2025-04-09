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

#ifndef MVVM_UTILS_VARIANT_LIMITS_HELPER_H_
#define MVVM_UTILS_VARIANT_LIMITS_HELPER_H_

//! @file
//! Helper methods for variant limits.

#include <mvvm/core/variant.h>

namespace mvvm
{

/**
 * @brief Returns pairs of integers representing limits of stored data.
 *
 * @param value The value for which we have to find min and max values.
 * @param lower_bound User defined additional lower bound.
 * @param upper_bound User defined additional upper bound.
 *
 * @details Reported limits will obtained by the combination of user defined limits and numeric
 * limits. If one or both of user limits is not set, std::numeric_limit::min() and
 * std::numeric_limit::max() will be used for lower and upper bounds.
 *
 * @details The method is used in the context of integer editors, when item's data type and limits
 * type are the same and are one of: int32: int8, uint8, int16, uint16, int32. The returned pair is
 * always casted to maximum allowed for integer editors type: int32
 */
std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value, const variant_t& lower_bound = {},
                                          const variant_t& upper_bound = {});

/**
 * @brief Returns pairs of doubles representing limits of stored data.
 *
 * @param value The value for which we have to find min and max values.
 * @param lower_bound User defined additional lower bound.
 * @param upper_bound User defined additional upper bound.
 *
 * @details Reported limits will obtained by the combination of user defined limits and numeric
 * limits. If one or both of user limits is not set, std::numeric_limit::lowest() and
 * std::numeric_limit::max() will be used for lower and upper bounds.
 *
 * @details The method is used in the context of double editors, when item's data type and limits
 * type are the same and are either float32 or float64. The returned pair is the result of the cast
 * to float64.
 */
std::pair<double, double> GetFloat64MinMaxNumeric(const variant_t& value,
                                                  const variant_t& lower_bound = {},
                                                  const variant_t& upper_bound = {});

}  // namespace mvvm

#endif  // MVVM_UTILS_VARIANT_LIMITS_HELPER_H_
