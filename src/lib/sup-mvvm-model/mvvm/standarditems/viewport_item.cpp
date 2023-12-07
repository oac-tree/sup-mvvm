/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewport_item.h"

#include "axis_items.h"

#include <mvvm/model/model_utils.h>

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

//! Sets range of x,y window to show all data.
//! Allows adding an additional margin to automatically calculated axis range. Margins are
//! given in relative units wrt calculated axis range.
//! Example: setViewportToContent(0.0, 0.1, 0.0, 0.1) will set axes to show all graphs with 10% gap
//! above and below graph's max and min.

void ViewportItem::SetViewportToContent(double left, double top, double right, double bottom)
{
  //  Utils::BeginMacros(this, "setViewportToContent");  // FIXME restore undo/redo
  auto [xmin, xmax] = GetDataXRange();
  GetXAxis()->SetRange(xmin - (xmax - xmin) * left, xmax + (xmax - xmin) * right);

  auto [ymin, ymax] = GetDataYRange();
  GetYAxis()->SetRange(ymin - (ymax - ymin) * bottom, ymax + (ymax - ymin) * top);
  //  Utils::EndMacros(this);
}

//! Sets range of x,y window to show all data.

void ViewportItem::SetViewportToContent()
{
  //  Utils::BeginMacros(this, "setViewportToContent"); // FIXME restore undo/redo
  auto [xmin, xmax] = GetDataXRange();
  GetXAxis()->SetRange(xmin, xmax);

  auto [ymin, ymax] = GetDataYRange();
  GetYAxis()->SetRange(ymin, ymax);
  //  Utils::EndMacros(this);
}

void ViewportItem::RegisterAxes()
{
  AddProperty<ViewportAxisItem>(kXaxis).SetDisplayName("X axis");
  AddProperty<ViewportAxisItem>(kYaxis).SetDisplayName("Y axis");
}

}  // namespace mvvm
