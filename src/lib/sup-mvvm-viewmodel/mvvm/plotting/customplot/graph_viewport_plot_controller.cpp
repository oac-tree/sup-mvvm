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

#include "graph_viewport_plot_controller.h"

#include "graph_plot_controller.h"
#include "viewport_axis_plot_controller.h"

#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/graph_viewport_item.h>

#include <qcustomplot.h>

#include <list>

namespace mvvm
{

struct GraphViewportPlotController::GraphViewportPlotControllerImpl
{
  GraphViewportPlotController* m_self{nullptr};
  QCustomPlot* m_custom_plot{nullptr};
  std::list<std::unique_ptr<GraphPlotController>> m_graph_controllers;
  std::unique_ptr<ViewportAxisPlotController> m_xaxis_controller;
  std::unique_ptr<ViewportAxisPlotController> m_yaxis_controller;

  GraphViewportPlotControllerImpl(GraphViewportPlotController* master, QCustomPlot* plot)
      : m_self(master), m_custom_plot(plot)
  {
  }

  GraphViewportItem* GetViewportItem() { return m_self->GetItem(); }

  //! Setup controller components.
  void SetupComponents()
  {
    CreateAxisControllers();
    CreateGraphControllers();
  }

  //! Creates axes controllers.

  void CreateAxisControllers()
  {
    auto viewport = GetViewportItem();

    m_xaxis_controller = std::make_unique<ViewportAxisPlotController>(m_custom_plot->xAxis);
    m_xaxis_controller->SetItem(viewport->GetXAxis());

    m_yaxis_controller = std::make_unique<ViewportAxisPlotController>(m_custom_plot->yAxis);
    m_yaxis_controller->SetItem(viewport->GetYAxis());
  }

  //! Run through all GraphItem's and create graph controllers for QCustomPlot.

  void CreateGraphControllers()
  {
    m_graph_controllers.clear();
    auto viewport = GetViewportItem();
    for (auto graph_item : viewport->GetGraphItems())
    {
      auto controller = std::make_unique<GraphPlotController>(m_custom_plot);
      controller->SetItem(graph_item);
      m_graph_controllers.push_back(std::move(controller));
    }
    viewport->SetViewportToContent();
  }

  //! Adds controller for item.
  void AddController(const ItemInsertedEvent& event)
  {
    const auto [parent, tagindex] = event;

    auto added_child = dynamic_cast<GraphItem*>(parent->GetItem(tagindex));

    for (auto& controller : m_graph_controllers)
    {
      if (controller->GetItem() == added_child)
      {
        throw RuntimeException(
            "Error in GraphViewportPlotController: attempt to create second controller");
      }
    }

    auto controller = std::make_unique<GraphPlotController>(m_custom_plot);
    controller->SetItem(added_child);
    m_graph_controllers.push_back(std::move(controller));
    m_custom_plot->replot();
  }

  //! Remove GraphPlotController corresponding to GraphItem.

  void RemoveController(const AboutToRemoveItemEvent& event)
  {
    const auto [parent, tagindex] = event;

    auto child_about_to_be_removed = parent->GetItem(tagindex);
    auto if_func = [&](const std::unique_ptr<GraphPlotController>& cntrl) -> bool
    { return cntrl->GetItem() == child_about_to_be_removed; };
    m_graph_controllers.remove_if(if_func);
    m_custom_plot->replot();
  }
};

GraphViewportPlotController::GraphViewportPlotController(QCustomPlot* custom_plot)
    : p_impl(std::make_unique<GraphViewportPlotControllerImpl>(this, custom_plot))
{
}

void GraphViewportPlotController::Subscribe()
{
  Listener()->Connect<ItemInsertedEvent>(p_impl.get(),
                                         &GraphViewportPlotControllerImpl::AddController);
  Listener()->Connect<AboutToRemoveItemEvent>(p_impl.get(),
                                              &GraphViewportPlotControllerImpl::RemoveController);
  p_impl->SetupComponents();
}

GraphViewportPlotController::~GraphViewportPlotController() = default;

}  // namespace mvvm
