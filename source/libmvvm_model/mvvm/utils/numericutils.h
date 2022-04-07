/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_UTILS_NUMERICUTILS_H
#define MVVM_UTILS_NUMERICUTILS_H

#include "mvvm/model_export.h"

namespace mvvm::utils
{

//! Returns true if two doubles agree within epsilon*tolerance.
MVVM_MODEL_EXPORT bool AreAlmostEqual(double a, double b, double tolerance_factor = 1.0);

//! Produces random integer values uniformly distributed on the closed interval [low, high].
MVVM_MODEL_EXPORT int RandInt(int low, int high);

//! Produces random FLOAT values uniformly distributed on the  interval [low, high).
MVVM_MODEL_EXPORT double RandDouble(double low, double high);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_NUMERICUTILS_H
