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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "graph_viewport_item.h"

#include "graph_item.h"

#include <algorithm>
#include <vector>

namespace
{

const double kFallBackMin = 0.0;
const double kFallBackMax = 1.0;

//! Find min and max values along all data points in all graphs.
//! Function 'func' is used to run either through binCenters or binValues.

template <typename T>
auto GetMinMax(const std::vector<mvvm::GraphItem*>& graphs, T func)
{
  std::vector<double> values;
  for (auto graph : graphs)
  {
    const auto array = func(graph);
    std::copy(std::begin(array), std::end(array), std::back_inserter(values));
  }

  auto [xmin, xmax] = std::minmax_element(std::begin(values), std::end(values));
  return xmin != xmax ? std::make_pair(*xmin, *xmax) : std::make_pair(kFallBackMin, kFallBackMax);
}

}  // namespace

namespace mvvm
{

GraphViewportItem::GraphViewportItem(const std::string& model_type) : ViewportItem(model_type)
{
  RegisterAxes();
  RegisterTag(TagInfo::CreateUniversalTag(kItems, {GraphItem::GetStaticType()}), /*set_default*/ true);
}

std::string GraphViewportItem::GetStaticType()
{
  return "GraphViewport";
}

std::unique_ptr<SessionItem> GraphViewportItem::Clone() const
{
  return std::make_unique<GraphViewportItem>(*this);
}

int GraphViewportItem::GetGraphCount() const
{
  return GetItemCount(kItems);
}

//! Returns the selected graph items.

std::vector<GraphItem*> GraphViewportItem::GetGraphItems() const
{
  return GetItems<GraphItem>(kItems);
}

//! Returns the selected graph items.

std::vector<GraphItem*> GraphViewportItem::GetVisibleGraphItems() const
{
  std::vector<GraphItem*> all_items = GetItems<GraphItem>(kItems);
  std::vector<GraphItem*> visible_items;
  std::copy_if(all_items.begin(), all_items.end(), std::back_inserter(visible_items),
               [](const GraphItem* graph_item)
               { return graph_item->Property<bool>(GraphItem::kDisplayed); });
  return visible_items;
}

//! Set the graph selection.

void GraphViewportItem::SetVisible(const std::vector<GraphItem*>& visible_graph_items)
{
  for (auto graph_item : GetItems<GraphItem>(kItems))
  {
    if (std::find(visible_graph_items.begin(), visible_graph_items.end(), graph_item)
        != visible_graph_items.end())
    {
      graph_item->SetProperty(GraphItem::kDisplayed, true);
    }
    else
    {
      graph_item->SetProperty(GraphItem::kDisplayed, false);
    }
  }
}

//! Reset the graph selection.

void GraphViewportItem::SetAllVisible()
{
  for (auto graph_item : GetItems<GraphItem>(kItems))
  {
    graph_item->SetProperty(GraphItem::kDisplayed, true);
  }
}

//! Returns lower, upper range on x-axis occupied by all data points of all graphs.

std::pair<double, double> GraphViewportItem::GetDataXRange() const
{
  return GetMinMax(GetVisibleGraphItems(), [](GraphItem* graph) { return graph->GetBinCenters(); });
}

//! Returns lower, upper range on y-axis occupied by all data points of all graphs.

std::pair<double, double> GraphViewportItem::GetDataYRange() const
{
  return GetMinMax(GetVisibleGraphItems(), [](GraphItem* graph) { return graph->GetValues(); });
}

}  // namespace mvvm
