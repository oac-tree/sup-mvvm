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

ViewportArea ApplySafetyMargins(const ViewportArea& area, double smallest_range_to_display,
                                double safety_margin)
{
  AxisRange safe_xrange;
  const bool xaxis_is_too_short =
      (area.xrange.upper - area.xrange.lower) < smallest_range_to_display;

  safe_xrange.lower = xaxis_is_too_short ? (area.xrange.lower - safety_margin) : area.xrange.lower;
  safe_xrange.upper = xaxis_is_too_short ? (area.xrange.upper + safety_margin) : area.xrange.upper;

  AxisRange safe_yrange;
  const bool yaxis_is_too_short =
      (area.yrange.upper - area.yrange.lower) < smallest_range_to_display;

  safe_yrange.lower = yaxis_is_too_short ? (area.yrange.lower - safety_margin) : area.yrange.lower;
  safe_yrange.upper = yaxis_is_too_short ? (area.yrange.upper + safety_margin) : area.yrange.upper;

  return {safe_xrange, safe_yrange};
}

}  // namespace mvvm
