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

#ifndef MVVM_UTILS_NUMERIC_UTILS_H_
#define MVVM_UTILS_NUMERIC_UTILS_H_

#include <mvvm/model_export.h>

#include <cstdint>

namespace mvvm::utils
{

//! @cond DoNotParse
MVVM_MODEL_EXPORT
//! @endcond
//! Returns true if two doubles agree within epsilon*tolerance.
bool AreAlmostEqual(double a, double b, double tolerance = 1.0);

//! @cond DoNotParse
MVVM_MODEL_EXPORT
//! @endcond
//! Produces random integer values uniformly distributed on the closed interval [low, high].
std::int32_t RandInt(std::int32_t low, std::int32_t high);

//! @cond DoNotParse
MVVM_MODEL_EXPORT
//! @endcond
//! Produces random FLOAT values uniformly distributed on the  interval [low, high).
double RandDouble(double low, double high);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_NUMERIC_UTILS_H_
