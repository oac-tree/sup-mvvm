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

#include <mvvm/model/item_constants.h>
#include <mvvm/standarditems/editor_constants.h>

#include <functional>

namespace mvvm
{

namespace
{

constexpr double kFallBackMin = 0.0;
constexpr double kFallBackMax = 1.0;

ComboProperty CreateAnimationCombo()
{
  ComboProperty result({std::string(ChartViewportItem::kSeriesAnimation),
                        std::string(ChartViewportItem::kGridAnimation)});
  result.SetSelectedIndices({0}); // only kSeriesAnimation is enabled by default
  return result;
}

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
auto GetMinMax(const std::vector<LineSeriesItem*>& container, MethodT method)
{
  std::vector<double> values;

  // accumulate all data points from all LineSeriesItem in the container.
  for (auto line_serie_item : container)
  {
    const auto array = std::invoke(method, line_serie_item);
    (void)std::copy(std::begin(array), std::end(array), std::back_inserter(values));
  }

  auto [xmin, xmax] = std::minmax_element(std::begin(values), std::end(values));
  return xmin != xmax ? std::make_pair(*xmin, *xmax) : std::make_pair(kFallBackMin, kFallBackMax);
}

}  // namespace

ChartViewportItem::ChartViewportItem() : ViewportItem(GetTypeName<ChartViewportItem>())
{
  RegisterAxes();
  RegisterTag(TagInfo::CreateUniversalTag(std::string(constants::kChildrenTag),
                                          {GetTypeName<LineSeriesItem>()}),
              /*set_default*/ true);
  (void)AddProperty(kAnimation, CreateAnimationCombo())
      .SetDisplayName("Animation")
      .SetToolTip("Apply animations on plot change")
      .SetEditorType(constants::kSelectableComboPropertyEditorType);
  (void)AddProperty(kLegendEnabled, true)
      .SetDisplayName("Legend")
      .SetToolTip("Show/hide canvas legend");
}

std::unique_ptr<SessionItem> ChartViewportItem::Clone() const
{
  return std::make_unique<ChartViewportItem>(*this);
}

std::vector<LineSeriesItem*> ChartViewportItem::GetLineSeries() const
{
  return GetItems<LineSeriesItem>(constants::kChildrenTag);
}

std::size_t ChartViewportItem::GetLineSeriesCount() const
{
  return GetItemCount(std::string(constants::kChildrenTag));
}

std::pair<double, double> ChartViewportItem::GetDataXRangeImpl() const
{
  return GetMinMax(GetLineSeries(), &LineSeriesItem::GetBinCenters);
}

std::pair<double, double> ChartViewportItem::GetDataYRangeImpl() const
{
  return GetMinMax(GetLineSeries(), &LineSeriesItem::GetValues);
}

}  // namespace mvvm
