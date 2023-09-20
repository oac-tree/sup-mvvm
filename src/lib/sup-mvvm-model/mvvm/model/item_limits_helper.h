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
 * @brief Returns true if proposed value is in limits range for given item.
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
 * @brief Removes limits from the given item.
 */
void RemoveLimits(mvvm::SessionItem& item);

}  // namespace mvvm

#endif  // MVVM_MODEL_LIMITS_HELPER_H_
