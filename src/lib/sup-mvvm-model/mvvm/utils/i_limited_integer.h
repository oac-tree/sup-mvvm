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

#ifndef MVVM_UTILS_I_LIMITED_INTEGER_H
#define MVVM_UTILS_I_LIMITED_INTEGER_H

#include <mvvm/model_export.h>

namespace mvvm
{

/**
 * @brief The ILimitedInteger it's an interface class to represent integer number with limits.
 */

class ILimitedInteger
{
public:
  virtual ~ILimitedInteger() = default;

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
};

}  // namespace mvvm

#endif  // MVVM_UTILS_LIMITED_INTEGER_H
