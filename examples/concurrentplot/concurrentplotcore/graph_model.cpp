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

#include "graph_model.h"

#include "toy_simulation.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/graph_viewport_item.h>
#include <mvvm/utils/numeric_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QColor>

namespace
{

auto GetSimulationResult(double amp_factor = 1.0)
{
  concurrentplot::ToySimulation simulation(amp_factor);
  simulation.RunSimulation();
  return simulation.GetSimulationResult();
}

}  // namespace

namespace concurrentplot
{

GraphModel::GraphModel() : ApplicationModel("GraphModel")
{
  PopulateModel();
}

//! Sets new values for existing data item.

void GraphModel::SetData(const std::vector<double>& data)
{
  auto item = mvvm::utils::GetTopItem<mvvm::ContainerItem>(this)->GetItem<mvvm::Data1DItem>(
      mvvm::TagIndex::kDefaultTag);
  item->SetValues(data);
}

//! Creates data container, Data1DItem, viewport and GraphItem.

void GraphModel::PopulateModel()
{
  auto container = InsertItem<mvvm::ContainerItem>();
  container->SetDisplayName("Data container");

  auto viewport = InsertItem<mvvm::GraphViewportItem>();
  viewport->SetDisplayName("Graph container");

  AddGraph(container, viewport);
}

//! Adds Graph1DItem with some random points.

void GraphModel::AddGraph(mvvm::ContainerItem* container, mvvm::GraphViewportItem* viewport)
{
  auto [xmin, xmax, points] = GetSimulationResult(mvvm::utils::RandDouble(0.5, 1.0));

  auto data = InsertItem<mvvm::Data1DItem>(container);
  data->SetAxis<mvvm::FixedBinAxisItem>(static_cast<int>(points.size()), xmin, xmax);
  data->SetValues(points);

  auto graph = InsertItem<mvvm::GraphItem>(viewport);
  graph->SetDataItem(data);
  graph->SetNamedColor(mvvm::utils::RandomNamedColor());
}

}  // namespace concurrentplot
