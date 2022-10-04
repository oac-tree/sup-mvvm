// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "graph_model.h"

#include "mvvm/standarditems/axis_items.h"
#include "mvvm/standarditems/container_item.h"
#include "mvvm/standarditems/data1d_item.h"
#include "mvvm/standarditems/graph_item.h"
#include "mvvm/standarditems/graph_viewport_item.h"
#include "mvvm/utils/numeric_utils.h"
#include "mvvm/widgets/widget_utils.h"
#include "mvvm/model/model_utils.h"
#include "toysimulation.h"

#include <QColor>

namespace
{

auto simulation_result(double amp_factor = 1.0)
{
  ToySimulation simulation(amp_factor);
  simulation.runSimulation();
  return simulation.simulationResult();
}

}  // namespace

using namespace mvvm;

GraphModel::GraphModel() : ApplicationModel("GraphModel")
{
  init_model();
}

//! Sets new values for existing data item.

void GraphModel::set_data(const std::vector<double>& data)
{
  auto item = utils::GetTopItem<ContainerItem>(this)->GetItem<Data1DItem>(ContainerItem::kChildren);
  item->SetValues(data);
}

//! Creates data container, Data1DItem, viewport and GraphItem.

void GraphModel::init_model()
{
  auto container = InsertItem<ContainerItem>();
  container->SetDisplayName("Data container");

  auto viewport = InsertItem<GraphViewportItem>();
  viewport->SetDisplayName("Graph container");

  add_graph(container, viewport);
}

//! Adds Graph1DItem with some random points.

void GraphModel::add_graph(mvvm::ContainerItem* container,
                           mvvm::GraphViewportItem* viewport)
{
  auto [xmin, xmax, points] = simulation_result(mvvm::utils::RandDouble(0.5, 1.0));

  auto data = InsertItem<Data1DItem>(container);
  data->SetAxis<FixedBinAxisItem>(static_cast<int>(points.size()), xmin, xmax);
  data->SetValues(points);

  auto graph = InsertItem<GraphItem>(viewport);
  graph->SetDataItem(data);
  graph->SetNamedColor(mvvm::utils::RandomNamedColor());
}
