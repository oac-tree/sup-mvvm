/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_STANDARDITEMS_VIEWPORT_TYPES_H_
#define MVVM_STANDARDITEMS_VIEWPORT_TYPES_H_

//!< @file
//! Helper types for ViewportItem.

namespace mvvm
{

struct ViewportMargins
{
  double left{0.0};
  double top{0.0};
  double right{0.0};
  double bottom{0.0};
};

struct AxisRange
{
  double lower{0.0};
  double upper{0.0};
};

struct ViewportArea
{
  AxisRange xrange;
  AxisRange yrange;
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VIEWPORT_TYPES_H_
