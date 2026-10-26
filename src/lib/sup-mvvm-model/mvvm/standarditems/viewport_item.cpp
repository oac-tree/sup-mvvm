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

#include "viewport_item.h"

#include "axis_items.h"
#include "viewport_helper.h"

#include <mvvm/model/item_utils.h>

namespace mvvm
{

ViewportItem::ViewportItem(const std::string& item_type) : CompoundItem(item_type) {}

const ViewportAxisItem* ViewportItem::GetXAxis() const
{
  return &GetItem<ViewportAxisItem>(kXaxis);
}

ViewportAxisItem* ViewportItem::GetXAxis()
{
  return &GetItem<ViewportAxisItem>(kXaxis);
}

const ViewportAxisItem* ViewportItem::GetYAxis() const
{
  return &GetItem<ViewportAxisItem>(kYaxis);
}

ViewportAxisItem* ViewportItem::GetYAxis()
{
  return &GetItem<ViewportAxisItem>(kYaxis);
}

std::pair<double, double> ViewportItem::GetDataXRange() const
{
  return GetDataXRangeImpl();
}

std::pair<double, double> ViewportItem::GetDataYRange() const
{
  return GetDataYRangeImpl();
}

ViewportArea ViewportItem::GetViewportArea() const
{
  ViewportArea area{};
  area.xrange.lower = GetXAxis()->GetMin();
  area.xrange.upper = GetXAxis()->GetMax();
  area.yrange.lower = GetYAxis()->GetMin();
  area.yrange.upper = GetYAxis()->GetMax();
  return area;
}

ViewportArea ViewportItem::GetDataArea() const
{
  auto [xmin, xmax] = GetDataXRange();
  auto [ymin, ymax] = GetDataYRange();
  return {{xmin, xmax}, {ymin, ymax}};
}

void ViewportItem::SetViewportArea(const ViewportArea& area)
{
  utils::BeginMacro(*this, "SetViewportToContent");
  GetXAxis()->SetRange(area.xrange.lower, area.xrange.upper);
  GetYAxis()->SetRange(area.yrange.lower, area.yrange.upper);
  utils::EndMacro(*this);
}

void ViewportItem::SetViewportToContent()
{
  SetViewportToContent({0.0, 0.0, 0.0, 0.0});
}

void ViewportItem::SetViewportToContent(const ViewportMargins& margins)
{
  SetViewportArea(ApplyRelativeMargins(GetDataArea(), margins));
}

void ViewportItem::RegisterAxes()
{
  (void)AddProperty<ViewportAxisItem>(kXaxis).SetDisplayName("X axis").SetEditable(false);
  (void)AddProperty<ViewportAxisItem>(kYaxis).SetDisplayName("Y axis").SetEditable(false);
}

}  // namespace mvvm
