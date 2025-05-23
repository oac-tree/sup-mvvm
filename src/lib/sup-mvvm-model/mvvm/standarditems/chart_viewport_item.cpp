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

#include "chart_viewport_item.h"

#include "line_series_item.h"
#include "mvvm/standarditems/editor_constants.h"

#include <functional>

namespace
{

const mvvm::ComboProperty CreateAnimationCombo()
{
  mvvm::ComboProperty result(
      {mvvm::ChartViewportItem::kSeriesAnimation, mvvm::ChartViewportItem::kGridAnimation});
  result.SetSelectedIndices({0, 1});
  return result;
}

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

ChartViewportItem::ChartViewportItem() : mvvm::ViewportItem(GetStaticType())
{
  RegisterAxes();
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kItems, {LineSeriesItem::GetStaticType()}),
              /*set_default*/ true);
  AddProperty(kAnimation, CreateAnimationCombo())
      .SetDisplayName("Animation")
      .SetToolTip("Apply animations on plot change")
      .SetEditorType(constants::kSelectableComboPropertyEditorType);
  AddProperty(kLegendEnabled, true).SetDisplayName("Legend").SetToolTip("Show/hide canvas legend");
}

std::string ChartViewportItem::GetStaticType()
{
  return "ChartViewport";
}

std::unique_ptr<SessionItem> ChartViewportItem::Clone() const
{
  return std::make_unique<ChartViewportItem>(*this);
}

std::vector<LineSeriesItem *> ChartViewportItem::GetLineSeries() const
{
  return GetItems<LineSeriesItem>(kItems);
}

int ChartViewportItem::GetLineSeriesCount() const
{
  return GetItemCount(kItems);
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
