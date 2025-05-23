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

#include "viewport_item.h"

#include "axis_items.h"

#include <mvvm/model/item_utils.h>

namespace mvvm
{

ViewportItem::ViewportItem(const std::string& item_type) : CompoundItem(item_type) {}

ViewportAxisItem* ViewportItem::GetXAxis() const
{
  return GetItem<ViewportAxisItem>(kXaxis);
}

ViewportAxisItem* ViewportItem::GetYAxis() const
{
  return GetItem<ViewportAxisItem>(kYaxis);
}

void ViewportItem::SetViewportToContent()
{
  SetViewportToContent(0.0, 0.0, 0.0, 0.0);
}

void ViewportItem::SetViewportToContent(double left, double top, double right, double bottom)
{
  utils::BeginMacro(*this, "SetViewportToContent");
  auto [xmin, xmax] = GetDataXRange();
  GetXAxis()->SetRange(xmin - (xmax - xmin) * left, xmax + (xmax - xmin) * right);

  auto [ymin, ymax] = GetDataYRange();
  GetYAxis()->SetRange(ymin - (ymax - ymin) * bottom, ymax + (ymax - ymin) * top);
  utils::EndMacro(*this);
}

void ViewportItem::RegisterAxes()
{
  AddProperty<ViewportAxisItem>(kXaxis).SetDisplayName("X axis").SetEditable(false);
  AddProperty<ViewportAxisItem>(kYaxis).SetDisplayName("Y axis").SetEditable(false);
}

}  // namespace mvvm
