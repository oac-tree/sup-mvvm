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

#include "painter_helper.h"

#include <mvvm/style/mvvm_style_helper.h>

#include <algorithm>
#include <cmath>

namespace mvvm
{

double ScalePenWidth(double value, double min_value)
{
  return std::max(min_value, style::ScaleToSystem(value));
}

double ScaleAndRoundPenWidth(double value, double min_value)
{
  return std::max(min_value, std::round(style::ScaleToSystem(value)));
}

}  // namespace mvvm
