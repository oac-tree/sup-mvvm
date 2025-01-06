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

#include "graph_model.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/utils/numeric_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QColor>
#include <cmath>

namespace
{

constexpr double pi = 3.14159265358979323846;
constexpr int npoints = 100;
constexpr double xmin = 0.0;
constexpr double xmax = 5.0;
constexpr double dx = (xmax - xmin) / npoints;

std::vector<std::pair<double, double>> GetWaveform(double amp_factor = 1.0)
{
  std::vector<std::pair<double, double>> result;
  for (int i = 0; i < npoints; ++i)
  {
    double x = xmin + i * dx;
    double value = amp_factor * 10.0 * std::sin(2.0 * pi * 2 * x)
                   + amp_factor * 5.0 * std::sin(2 * pi * 2.25 * x);
    result.emplace_back(x, value);
  }
  return result;
}

}  // namespace

namespace lineseries
{

GraphModel::GraphModel() : mvvm::ApplicationModel("GraphModel")
{
  auto container = InsertItem<mvvm::ContainerItem>();
  container->SetDisplayName("Data container");

  auto viewport = InsertItem<mvvm::ChartViewportItem>();
  viewport->SetDisplayName("Graph container");

  AddGraph();
}

void GraphModel::AddGraph()
{
  // adds LineSeriesDataItem with data to plot, and LineSeriesItem looking to it.

  auto data = InsertItem<mvvm::LineSeriesDataItem>(GetDataContainer());
  data->SetWaveform(GetWaveform(mvvm::utils::RandDouble(0.5, 1.0)));

  auto graph = InsertItem<mvvm::LineSeriesItem>(GetViewport());
  graph->SetDataItem(data);
  graph->SetNamedColor(mvvm::utils::RandomNamedColor());
}

void GraphModel::RemoveGraph()
{
  const int graph_count = GetViewport()->GetLineSeriesCount();
  const int data_count = GetDataContainer()->GetSize();

  if (graph_count != data_count)
  {
    throw std::runtime_error("Number of graphs do not much number of data items.");
  }

  if (graph_count)
  {
    TakeItem(GetViewport(), mvvm::TagIndex::Default(graph_count - 1));
  }

  if (data_count)
  {
    TakeItem(GetDataContainer(), mvvm::TagIndex::Default(data_count - 1));
  }
}

void GraphModel::RandomizeGraphs()
{
  for (auto item : GetDataContainer()->GetItems<mvvm::LineSeriesDataItem>({}))
  {
    auto waveform = item->GetWaveform();
    auto on_element = [](auto point)
    { return std::make_pair(point.first, point.second * mvvm::utils::RandDouble(0.8, 1.2)); };
    std::transform(std::begin(waveform), std::end(waveform), std::begin(waveform), on_element);
    item->SetWaveform(waveform);
  }
}

mvvm::ChartViewportItem* GraphModel::GetViewport()
{
  return ::mvvm::utils::GetTopItem<mvvm::ChartViewportItem>(this);
}

mvvm::ContainerItem* GraphModel::GetDataContainer()
{
  return ::mvvm::utils::GetTopItem<mvvm::ContainerItem>(this);
}

}  // namespace lineseries
