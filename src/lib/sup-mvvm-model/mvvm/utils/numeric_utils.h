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

#include <cstdint>

namespace mvvm::utils
{

//! Returns true if two doubles agree within epsilon*tolerance.
bool AreAlmostEqual(double lhs, double rhs, double tolerance = 1.0);

//! Produces random integer values uniformly distributed on the closed interval [low, high].
std::int32_t RandInt(std::int32_t low, std::int32_t high);

//! Produces random FLOAT values uniformly distributed on the  interval [low, high).
double RandDouble(double low, double high);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_NUMERIC_UTILS_H_
