/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "graph_plot_controller.h"

#include "data1d_plot_controller.h"
#include "pen_controller.h"

#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/plottable_items.h>

#include <qcustomplot.h>

namespace mvvm
{

struct GraphPlotController::GraphItemControllerImpl
{
  GraphPlotController* m_self{nullptr};
  QCustomPlot* m_custom_plot{nullptr};
  QCPGraph* m_graph{nullptr};
  std::unique_ptr<Data1DPlotController> m_data_controller;
  std::unique_ptr<PenController> m_pen_controller;

  GraphItemControllerImpl(GraphPlotController* master, QCustomPlot* plot)
      : m_self(master), m_custom_plot(plot)
  {
  }

  //! Setups controllers and updates graph properties.

  void InitGraph()
  {
    m_graph = m_custom_plot->addGraph();
    m_data_controller = std::make_unique<Data1DPlotController>(m_graph);
    m_pen_controller = std::make_unique<PenController>(m_graph);

    UpdateDataController();
    UpdateGraphPen();
    UpdateVisibility();
  }

  ~GraphItemControllerImpl()
  {
    if (m_graph)
    {
      m_custom_plot->removePlottable(m_graph);
    }
  }

  GraphItem* GetGraphItem() { return m_self->GetItem(); }

  void UpdateDataController() { m_data_controller->SetItem(GetGraphItem()->GetDataItem()); }

  //! Updates graph pen from GraphItem.

  void UpdateGraphPen() { m_pen_controller->SetItem(GetGraphItem()->GetPenItem()); }

  //! Update visible
  void UpdateVisibility()
  {
    m_graph->setVisible(GetGraphItem()->Property<bool>(GraphItem::kDisplayed));
    m_custom_plot->replot();
  }

  void ResetGraph()
  {
    m_data_controller->SetItem(nullptr);
    m_pen_controller->SetItem(nullptr);
    m_custom_plot->removePlottable(m_graph);
    m_graph = nullptr;
    m_custom_plot->replot();
  }

  void OnPropertyChanged(const PropertyChangedEvent& event)
  {
    if (event.m_name == GraphItem::kLink)
    {
      UpdateDataController();
    }

    if (event.m_name == GraphItem::kDisplayed)
    {
      UpdateVisibility();
    }
  }
};

GraphPlotController::GraphPlotController(QCustomPlot* custom_plot)
    : p_impl(std::make_unique<GraphItemControllerImpl>(this, custom_plot))
{
}

void GraphPlotController::Subscribe()
{
  Connect<PropertyChangedEvent>(p_impl.get(), &GraphItemControllerImpl::OnPropertyChanged);

  p_impl->InitGraph();
}

void GraphPlotController::Unsubscribe()
{
  p_impl->ResetGraph();
}

GraphPlotController::~GraphPlotController() = default;

}  // namespace mvvm
