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

#include "graph_viewport_item.h"

#include "graph_item.h"
#include "viewport_helper.h"

#include <mvvm/model/item_constants.h>

#include <algorithm>
#include <vector>

namespace mvvm
{

GraphViewportItem::GraphViewportItem(const std::string& model_type) : ViewportItem(model_type)
{
  RegisterAxes();
  RegisterTag(
      TagInfo::CreateUniversalTag(std::string(constants::kChildrenTag), {GetTypeName<GraphItem>()}),
      /*set_default*/ true);
}

std::unique_ptr<SessionItem> GraphViewportItem::Clone() const
{
  return std::make_unique<GraphViewportItem>(*this);
}

std::size_t GraphViewportItem::GetGraphCount() const
{
  return GetItemCount(std::string(constants::kChildrenTag));
}

std::vector<GraphItem*> GraphViewportItem::GetGraphItems() const
{
  return GetItems<GraphItem>(constants::kChildrenTag);
}

std::vector<GraphItem*> GraphViewportItem::GetVisibleGraphItems() const
{
  std::vector<GraphItem*> all_items = GetItems<GraphItem>(constants::kChildrenTag);
  std::vector<GraphItem*> visible_items;
  (void)std::copy_if(all_items.begin(), all_items.end(), std::back_inserter(visible_items),
                     [](const GraphItem* graph_item)
                     { return graph_item->Property<bool>(constants::kDisplayed); });
  return visible_items;
}

void GraphViewportItem::SetVisibleGraphItems(const std::vector<GraphItem*>& visible_graph_items)
{
  for (auto graph_item : GetItems<GraphItem>(constants::kChildrenTag))
  {
    if (std::find(visible_graph_items.begin(), visible_graph_items.end(), graph_item)
        != visible_graph_items.end())
    {
      (void)graph_item->SetProperty(constants::kDisplayed, true);
    }
    else
    {
      (void)graph_item->SetProperty(constants::kDisplayed, false);
    }
  }
}

void GraphViewportItem::SetAllVisible()
{
  for (auto graph_item : GetItems<GraphItem>(constants::kChildrenTag))
  {
    (void)graph_item->SetProperty(constants::kDisplayed, true);
  }
}

std::pair<double, double> GraphViewportItem::GetDataXRangeImpl() const
{
  return GetMinMax(GetVisibleGraphItems(), &GraphItem::GetBinCenters);
}

std::pair<double, double> GraphViewportItem::GetDataYRangeImpl() const
{
  return GetMinMax(GetVisibleGraphItems(), &GraphItem::GetValues);
}

}  // namespace mvvm
