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

#ifndef MVVM_MODEL_ITEM_LIMITS_HELPER_H_
#define MVVM_MODEL_ITEM_LIMITS_HELPER_H_

#include <mvvm/core/variant.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief Returns true if given item has user defined lower limit.
 */
bool HasLowerLimit(const SessionItem& item);

/**
 * @brief Returns non-empty variant representing lower limit of the given item, or empty
 * variant, if there was no limit set.
 */
variant_t GetLowerLimit(const SessionItem& item);

/**
 * @brief Returns true if given item has user defined upper limit.
 */
bool HasUpperLimit(const SessionItem& item);

/**
 * @brief Returns non-empty variant representing upper limit of the given item, or empty
 * variant, if there was no limit set.
 */
variant_t GetUpperLimit(const SessionItem& item);

/**
 * @brief Returns true if proposed value is in range for given item limits (lower_limit <= value <
 * upper_limit).
 *
 * @param item The item which possibly carries the data role, and limits.
 * @param value The value to test if it is in alowed range.
 *
 * @details If item has data role set, the proposed value should have the same type. As long as it
 * is the case, the value is checked against limits.
 */
bool IsInRange(const SessionItem& item, const variant_t& value);

/**
 * @brief Returns true it there are no limits set.
 */
bool IsLimitless(const SessionItem& item);

/**
 * @brief Returns true uf an object has lower and upper limits set.
 */
bool IsLimited(const SessionItem& item);

/**
 * @brief Set limits so item is bounded from the left.
 *
 * @param bound_value The value of lower bound.
 * @param item The item to set limits.
 *
 * @details All previous limits will be removed. If item has data already, bound value should have
 * the same type. There is no check if old value satisfy new bound value.
 */
void SetLowerLimited(const variant_t& bound_value, SessionItem& item);

/**
 * @brief Set limits so item is bounded from the right.
 *
 * @param bound_value The value of lower bound.
 * @param item The item to set limits.
 *
 * @details All previous limits will be removed. If item has data already, bound value should have
 * the same type. There is no check if old value satisfy new bound value.
 */
void SetUpperLimited(const variant_t& bound_value, SessionItem& item);

/**
 * @brief Set limits for values bounded from left and right.
 *
 * @param left_bound_value The value of lower bound.
 * @param right_bound_value The value of upper bound.
 * @param item The item to set limits.
 *
 * @details All previous limits will be removed. If item has data already, bound value should have
 * the same type. There is no check if old value satisfy new bound values.
 */
void SetLimited(const variant_t& left_bound_value, const variant_t& right_bound_value,
                SessionItem& item);

/**
 * @brief Removes limits from the given item.
 */
void RemoveLimits(mvvm::SessionItem& item);

/**
 * @brief Return pair of integer representing limits of the given item.
 *
 * @details The function is used to set min/max limits of QSpinBox. It is intended for editing
 * all integer-like numbers that can fit inside `int32` type: int8, uint8, int16, uint16, int32.
 *
 * If the user has lower and upper limit defined on board of the item, they will be returned to the
 * user as it is. If some limits are not defined, std::numeric_limits will be used instead. The
 * function will throw if item' holds limits that can't be fit into pair of integers.
 */
std::pair<int, int> GetInt32Limits(const SessionItem& item);

/**
 * @brief Return pair of doubles representing limits of the given item.
 *
 * @details The function is used to set min/max limits of QDoubleSpinBox. It is intended for editing
 * mvvm::float32 and mvvm::float64.
 *
 * If the user has lower and upper limit defined on board of the item, they will be returned to the
 * user as it is. If some limits are not defined, std::numeric_limits will be used instead. The
 * function will throw if item' users limits are based on types other than float32 and float64.
 */
std::pair<double, double> GetFloat64Limits(const SessionItem& item);

}  // namespace mvvm

#endif  // MVVM_MODEL_LIMITS_HELPER_H_
