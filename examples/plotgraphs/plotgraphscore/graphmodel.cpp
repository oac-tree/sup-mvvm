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

std::vector<double> bin_values(double amp_factor = 1.0)
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

using namespace mvvm;

namespace PlotGraphs
{

GraphModel::GraphModel() : SessionModel("GraphModel")
{
  populateModel();
}

//! Adds Graph1DItem with some random points.

void GraphModel::addGraph()
{
  // FIXME uncomment after undo/redo implementation
  //    if (undoStack())
  //        undoStack()->beginMacro("addGraph");

  auto data = InsertItem<Data1DItem>(dataContainer());
  data->SetAxis<FixedBinAxisItem>(npoints, xmin, xmax);
  data->SetValues(bin_values(mvvm::utils::RandDouble(0.5, 1.0)));

  auto graph = InsertItem<GraphItem>(viewport());
  graph->SetDataItem(data);
  graph->SetNamedColor(mvvm::utils::RandomNamedColor());

  // FIXME uncomment after undo/redo implementation
  //    if (undoStack())
  //        undoStack()->endMacro();
}

//! Remove last graph and data item.

void GraphModel::removeGraph()
{
  // FIXME uncomment after undo/redo implementation
  //    if (undoStack())
  //        undoStack()->beginMacro("removeGraph");

  const int graph_count = viewport()->GetItemCount(ViewportItem::kItems);
  const int data_count = dataContainer()->GetSize();

  if (graph_count != data_count)
    throw std::runtime_error("Number of graphs do not much number of data items.");

  if (graph_count)
    TakeItem(viewport(), {"", graph_count - 1});

  if (data_count)
    TakeItem(dataContainer(), {"", data_count - 1});

  // FIXME uncomment after undo/redo implementation
  //    if (undoStack())
  //        undoStack()->endMacro();
}

//! Put random noise to graphs.

void GraphModel::randomizeGraphs()
{
  for (auto item : dataContainer()->GetItems<Data1DItem>(ContainerItem::kChildren))
  {
    auto values = item->GetValues();
    std::transform(std::begin(values), std::end(values), std::begin(values),
                   [](auto x) { return x * utils::RandDouble(0.8, 1.2); });
    item->SetValues(values);
  }
}

//! Returns viewport item containig graph items.

GraphViewportItem* GraphModel::viewport()
{
  return GetTopItem<GraphViewportItem>();
}

//! Returns container with data items.

ContainerItem* GraphModel::dataContainer()
{
  return GetTopItem<ContainerItem>();
}

void GraphModel::populateModel()
{
  auto container = InsertItem<ContainerItem>();
  container->SetDisplayName("Data container");

  auto viewport = InsertItem<GraphViewportItem>();
  viewport->SetDisplayName("Graph container");

  addGraph();
}

}  // namespace PlotGraphs
