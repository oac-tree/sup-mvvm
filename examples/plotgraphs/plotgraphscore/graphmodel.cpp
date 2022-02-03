// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "graphmodel.h"

#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/utils/numericutils.h"
#include "mvvm/widgets/widgetutils.h"

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

namespace PlotGraphs
{

GraphModel::GraphModel() : mvvm::ApplicationModel("GraphModel")
{
  auto container = InsertItem<mvvm::ContainerItem>();
  container->SetDisplayName("Data container");

  auto viewport = InsertItem<mvvm::GraphViewportItem>();
  viewport->SetDisplayName("Graph container");

  AddGraph();
}

//! Adds a graph to the model.
//! Internally it adds Data1DItem carrying data points, and GraphItem containing presentation
//! details and linked to Data1DItem.

void GraphModel::AddGraph()
{
  auto data = InsertItem<mvvm::Data1DItem>(GetDataContainer());
  data->SetAxis<mvvm::FixedBinAxisItem>(npoints, xmin, xmax);
  data->SetValues(GetBinValues(mvvm::utils::RandDouble(0.5, 1.0)));

  auto graph = InsertItem<mvvm::GraphItem>(GetViewport());
  graph->SetDataItem(data);
  graph->SetNamedColor(mvvm::utils::RandomNamedColor());
}

//! Remove last graph and data item.

void GraphModel::RemoveGraph()
{
  const int graph_count = GetViewport()->GetItemCount(mvvm::ViewportItem::kItems);
  const int data_count = GetDataContainer()->GetSize();

  if (graph_count != data_count)
  {
    throw std::runtime_error("Number of graphs do not much number of data items.");
  }

  if (graph_count)
  {
    TakeItem(GetViewport(), {"", graph_count - 1});
  }

  if (data_count)
  {
    TakeItem(GetDataContainer(), {"", data_count - 1});
  }
}

//! Put random noise to graphs.

void GraphModel::RandomizeGraphs()
{
  for (auto item : GetDataContainer()->GetItems<mvvm::Data1DItem>(mvvm::ContainerItem::kChildren))
  {
    auto values = item->GetValues();
    std::transform(std::begin(values), std::end(values), std::begin(values),
                   [](auto x) { return x * mvvm::utils::RandDouble(0.8, 1.2); });
    item->SetValues(values);
  }
}

//! Returns viewport item containig graph items.

mvvm::GraphViewportItem* GraphModel::GetViewport()
{
  return GetTopItem<mvvm::GraphViewportItem>();
}

//! Returns container with data items.

mvvm::ContainerItem* GraphModel::GetDataContainer()
{
  return GetTopItem<mvvm::ContainerItem>();
}

}  // namespace PlotGraphs
