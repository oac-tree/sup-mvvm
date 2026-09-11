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

#include "chart_viewport_item.h"

#include "line_series_item.h"
#include "viewport_helper.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/model/make_clone.h>
#include <mvvm/standarditems/editor_constants.h>

namespace mvvm
{

namespace
{

ComboProperty CreateAnimationCombo()
{
  ComboProperty result({std::string(ChartViewportItem::kSeriesAnimation),
                        std::string(ChartViewportItem::kGridAnimation)});
  result.SetSelectedIndices({0});  // only kSeriesAnimation is enabled by default
  return result;
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
  return MakeClone(*this);
}

std::vector<const LineSeriesItem*> ChartViewportItem::GetLineSeries() const
{
  return FindItems<LineSeriesItem>(constants::kChildrenTag);
}

std::vector<LineSeriesItem*> ChartViewportItem::GetLineSeries()
{
  return FindItems<LineSeriesItem>(constants::kChildrenTag);
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
