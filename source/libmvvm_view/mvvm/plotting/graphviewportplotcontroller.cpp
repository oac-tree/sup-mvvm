// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/plotting/graphviewportplotcontroller.h"

#include "mvvm/plotting/graphplotcontroller.h"
#include "mvvm/plotting/viewportaxisplotcontroller.h"
#include "mvvm/standarditems/axisitems.h"
#include "mvvm/standarditems/graphitem.h"
#include "mvvm/standarditems/graphviewportitem.h"

#include <qcustomplot.h>

#include <list>
#include <stdexcept>

namespace mvvm
{

struct GraphViewportPlotController::GraphViewportPlotControllerImpl
{
  GraphViewportPlotController* master{nullptr};
  QCustomPlot* custom_plot{nullptr};
  std::list<std::unique_ptr<GraphPlotController>> graph_controllers;
  std::unique_ptr<ViewportAxisPlotController> xAxisController;
  std::unique_ptr<ViewportAxisPlotController> yAxisController;

  GraphViewportPlotControllerImpl(GraphViewportPlotController* master, QCustomPlot* plot)
      : master(master), custom_plot(plot)
  {
  }

  GraphViewportItem* viewport_item() { return master->GetItem(); }

  //! Setup controller components.
  void setup_components()
  {
    create_axis_controllers();
    create_graph_controllers();
  }

  //! Creates axes controllers.

  void create_axis_controllers()
  {
    auto viewport = viewport_item();

    xAxisController = std::make_unique<ViewportAxisPlotController>(custom_plot->xAxis);
    xAxisController->SetItem(viewport->GetXAxis());

    yAxisController = std::make_unique<ViewportAxisPlotController>(custom_plot->yAxis);
    yAxisController->SetItem(viewport->GetYAxis());
  }

  //! Run through all GraphItem's and create graph controllers for QCustomPlot.

  void create_graph_controllers()
  {
    graph_controllers.clear();
    auto viewport = viewport_item();
    for (auto graph_item : viewport->GetGraphItems())
    {
      auto controller = std::make_unique<GraphPlotController>(custom_plot);
      controller->SetItem(graph_item);
      graph_controllers.push_back(std::move(controller));
    }
    viewport->SetViewportToContent();
  }

  //! Adds controller for item.
  void add_controller_for_item(SessionItem* parent, const TagIndex& tagrow)
  {
    auto added_child = dynamic_cast<GraphItem*>(parent->GetItem(tagrow.tag, tagrow.index));

    for (auto& controller : graph_controllers)
      if (controller->GetItem() == added_child)
        throw std::runtime_error("Attempt to create second controller");

    auto controller = std::make_unique<GraphPlotController>(custom_plot);
    controller->SetItem(added_child);
    graph_controllers.push_back(std::move(controller));
    custom_plot->replot();
  }

  //! Remove GraphPlotController corresponding to GraphItem.

  void remove_controller_for_item(SessionItem* parent, const TagIndex& tagrow)
  {
    auto child_about_to_be_removed = parent->GetItem(tagrow.tag, tagrow.index);
    auto if_func = [&](const std::unique_ptr<GraphPlotController>& cntrl) -> bool
    { return cntrl->GetItem() == child_about_to_be_removed; };
    graph_controllers.remove_if(if_func);
    custom_plot->replot();
  }
};

GraphViewportPlotController::GraphViewportPlotController(QCustomPlot* custom_plot)
    : p_impl(std::make_unique<GraphViewportPlotControllerImpl>(this, custom_plot))
{
}

void GraphViewportPlotController::Subscribe()
{
  auto on_item_inserted = [this](SessionItem* parent, TagIndex tagrow)
  { p_impl->add_controller_for_item(parent, tagrow); };
  SetOnItemInserted(on_item_inserted);

  auto on_about_to_remove_item = [this](SessionItem* parent, TagIndex tagrow)
  { p_impl->remove_controller_for_item(parent, tagrow); };
  SetOnAboutToRemoveItem(on_about_to_remove_item);

  p_impl->setup_components();
}

GraphViewportPlotController::~GraphViewportPlotController() = default;

}  // namespace mvvm
