/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "chart_viewport_item.h"

#include "line_series_item.h"

#include <functional>

namespace
{
const double kFallBackMin = 0.0;
const double kFallBackMax = 1.0;

/**
 * @brief Finds min and max values along all data points in all graphs.
 *
 * Used to find min, max range of all points either along x-axis, or y-axis.
 *
 * @tparam MethodT Either GetBinCenters or GetValues method.
 * @param container Container with line series items.
 * @param method Method from LineSeriesItem.
 */
template <typename MethodT>
auto GetMinMax(const std::vector<mvvm::LineSeriesItem *> &container, MethodT method)
{
  std::vector<double> values;

  // accumulate all data points from all LineSeriesItem in the container.
  for (auto line_serie_item : container)
  {
    const auto array = std::invoke(method, line_serie_item);
    std::copy(std::begin(array), std::end(array), std::back_inserter(values));
  }

  auto [xmin, xmax] = std::minmax_element(std::begin(values), std::end(values));
  return xmin != xmax ? std::make_pair(*xmin, *xmax) : std::make_pair(kFallBackMin, kFallBackMax);
}

}  // namespace

namespace mvvm
{

ChartViewportItem::ChartViewportItem() : mvvm::ViewportItem(Type)
{
  RegisterAxes();
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kItems, {LineSeriesItem::Type}),
              /*set_default*/ true);
}

std::vector<LineSeriesItem *> ChartViewportItem::GetLineSeries() const
{
  return GetItems<LineSeriesItem>(kItems);
}

std::pair<double, double> ChartViewportItem::GetDataXRange() const
{
  return GetMinMax(GetLineSeries(), &LineSeriesItem::GetBinCenters);
}

std::pair<double, double> ChartViewportItem::GetDataYRange() const
{
  return GetMinMax(GetLineSeries(), &LineSeriesItem::GetValues);
}

}  // namespace mvvm
