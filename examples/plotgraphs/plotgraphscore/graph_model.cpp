/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/utils/numeric_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QColor>
#include <cmath>
#include <stdexcept>

namespace
{

constexpr double pi = 3.14159265358979323846;
constexpr int npoints = 400;
constexpr double xmin = 0.0;
constexpr double xmax = 5.0;
constexpr double dx = (xmax - xmin) / npoints;

std::vector<double> GetBinValues(double amp_factor = 1.0)
{
  std::vector<double> result;
  for (int i = 0; i < npoints; ++i)
  {
    double x = xmin + i * dx;
    double value = amp_factor * 10.0 * std::sin(2.0 * pi * 2 * x)
                   + amp_factor * 5.0 * std::sin(2 * pi * 2.25 * x);
    result.push_back(value);
  }
  return result;
}
}  // namespace

namespace plotgraphs
{

GraphModel::GraphModel() : mvvm::ApplicationModel("GraphModel")
{
  SetUndoEnabled(true);

  auto container = InsertItem<mvvm::ContainerItem>();
  container->SetDisplayName("Data container");

  auto viewport = InsertItem<mvvm::GraphViewportItem>();
  viewport->SetDisplayName("Graph container");

  AddGraph();
}

void GraphModel::AddGraph()
{
  mvvm::utils::BeginMacro(*this, "AddGraph");

  auto data = InsertItem<mvvm::Data1DItem>(GetDataContainer());
  data->SetAxis<mvvm::FixedBinAxisItem>(npoints, xmin, xmax);
  data->SetValues(GetBinValues(mvvm::utils::RandDouble(0.5, 1.0)));

  auto graph = InsertItem<mvvm::GraphItem>(GetViewport());
  graph->SetDataItem(data);
  graph->SetNamedColor(mvvm::utils::RandomNamedColor());

  mvvm::utils::EndMacro(*this);
}

void GraphModel::RemoveGraph()
{
  mvvm::utils::BeginMacro(*this, "RemoveGraph");

  const int graph_count = GetViewport()->GetGraphCount();
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

  mvvm::utils::EndMacro(*this);
}

void GraphModel::RandomizeGraphs()
{
  for (auto item : GetDataContainer()->GetItems<mvvm::Data1DItem>({}))
  {
    auto values = item->GetValues();
    std::transform(std::begin(values), std::end(values), std::begin(values),
                   [](auto x) { return x * mvvm::utils::RandDouble(0.8, 1.2); });
    item->SetValues(values);
  }
}

void GraphModel::Undo()
{
  GetCommandStack()->Undo();
}

void GraphModel::Redo()
{
  GetCommandStack()->Redo();
}

mvvm::GraphViewportItem* GraphModel::GetViewport()
{
  return ::mvvm::utils::GetTopItem<mvvm::GraphViewportItem>(this);
}

mvvm::ContainerItem* GraphModel::GetDataContainer()
{
  return ::mvvm::utils::GetTopItem<mvvm::ContainerItem>(this);
}

}  // namespace plotgraphs
