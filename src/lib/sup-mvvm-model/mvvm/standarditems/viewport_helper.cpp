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

#include "viewport_helper.h"

namespace mvvm
{

AxisRange ExpandRelativeRange(const AxisRange& range, double lower_margin, double upper_margin)
{
  const double delta = range.upper - range.lower;
  AxisRange expanded_range;
  expanded_range.lower = range.lower - (delta * lower_margin);
  expanded_range.upper = range.upper + (delta * upper_margin);
  return expanded_range;
}

ViewportArea ApplyRelativeMargins(const ViewportArea& area, const ViewportMargins& relative_margins)
{
  const AxisRange expanded_xrange =
      ExpandRelativeRange(area.xrange, relative_margins.left, relative_margins.right);
  const AxisRange expanded_yrange =
      ExpandRelativeRange(area.yrange, relative_margins.bottom, relative_margins.top);
  return {expanded_xrange, expanded_yrange};
}

}  // namespace mvvm
