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

#ifndef MVVM_STANDARDITEMS_VIEWPORT_HELPER_H_
#define MVVM_STANDARDITEMS_VIEWPORT_HELPER_H_

#include <mvvm/standarditems/viewport_types.h>

#include <algorithm>
#include <functional>
#include <vector>

namespace mvvm
{

namespace constants
{
//!< defines which axes range we want to see when viewport is empty
constexpr double kFallBackMin = 0.0;
constexpr double kFallBackMax = 1.0;
}  // namespace constants

/**
 * @brief Finds min and max values along all data points for all items in the container.
 *
 * It is assumed, that an item in the container represents a graph, and it has a method to get array
 * of data points. This can be the method returning all x-values, or all y-values. Used to find min,
 * max range of all points either along x-axis, or y-axis.
 *
 * @tparam ContainerT Container type with line series items.
 * @tparam MethodT A method which retuns array of values from the item.
 * @param container Container with line series items.
 * @param method Method from graph item in the container.
 */
template <typename ContainerT, typename MethodT>
auto GetMinMax(const ContainerT& container, MethodT method)
{
  std::vector<double> values;

  // accumulate all data points from all LineSeriesItem in the container.
  for (auto container_item : container)
  {
    const auto array = std::invoke(method, container_item);
    (void)std::copy(std::begin(array), std::end(array), std::back_inserter(values));
  }

  auto [xmin, xmax] = std::minmax_element(std::begin(values), std::end(values));
  return xmin != xmax ? std::make_pair(*xmin, *xmax)
                      : std::make_pair(constants::kFallBackMin, constants::kFallBackMax);
}

/**
 * @brief Expands given axis range by relative margins.
 */
AxisRange ExpandRelativeRange(const AxisRange& range, double lower_margin, double upper_margin);

/**
 * @brief Returns viewport area with applied relative margins.
 */
ViewportArea ApplyRelativeMargins(const ViewportArea& area,
                                  const ViewportMargins& relative_margins);

/**
 * @brief Apply margins to viewport area, ensuring that the area is not zero-sized.
 *
 * If the size of the area along any axis is less than min_delta, the area is expanded by
 * safety_margin.
 */
ViewportArea ApplySafetyMargins(const ViewportArea& area, double min_delta = 1e-5,
                                double safety_margin = 0.5);

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VIEWPORT_HELPER_H_
