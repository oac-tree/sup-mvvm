/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/graphplotcontroller.h"

#include "mvvm/plotting/data1dplotcontroller.h"
#include "mvvm/plotting/pencontroller.h"
#include "mvvm/standarditems/data1ditem.h"
#include "mvvm/standarditems/graphitem.h"
#include "mvvm/standarditems/plottableitems.h"

#include <qcustomplot.h>

namespace mvvm
{

struct GraphPlotController::GraphItemControllerImpl
{
  GraphPlotController* m_self{nullptr};
  QCustomPlot* m_customPlot{nullptr};
  QCPGraph* m_graph{nullptr};
  std::unique_ptr<Data1DPlotController> m_dataController;
  std::unique_ptr<PenController> m_penController;

  GraphItemControllerImpl(GraphPlotController* master, QCustomPlot* plot)
      : m_self(master), m_customPlot(plot)
  {
  }

  //! Setups controllers and updates graph properties.

  void init_graph()
  {
    m_graph = m_customPlot->addGraph();
    m_dataController = std::make_unique<Data1DPlotController>(m_graph);
    m_penController = std::make_unique<PenController>(m_graph);

    update_data_controller();
    update_graph_pen();
    update_visible();
  }

  ~GraphItemControllerImpl()
  {
    if (m_graph)
      m_customPlot->removePlottable(m_graph);
  }

  GraphItem* graph_item() { return m_self->GetItem(); }

  void update_data_controller() { m_dataController->SetItem(graph_item()->GetDataItem()); }

  //! Updates graph pen from GraphItem.

  void update_graph_pen() { m_penController->SetItem(graph_item()->GetPenItem()); }

  //! Update visible
  void update_visible()
  {
    m_graph->setVisible(graph_item()->Property<bool>(GraphItem::kDisplayed));
    m_customPlot->replot();
  }

  void reset_graph()
  {
    m_dataController->SetItem(nullptr);
    m_penController->SetItem(nullptr);
    m_customPlot->removePlottable(m_graph);
    m_graph = nullptr;
    m_customPlot->replot();
  }
};

GraphPlotController::GraphPlotController(QCustomPlot* custom_plot)
    : p_impl(std::make_unique<GraphItemControllerImpl>(this, custom_plot))
{
}

void GraphPlotController::Subscribe()
{
  auto on_property_change = [this](SessionItem*, const std::string& property_name)
  {
    if (property_name == GraphItem::kLink)
    {
      p_impl->update_data_controller();
    }

    if (property_name == GraphItem::kDisplayed)
    {
      p_impl->update_visible();
    }
  };
  SetOnPropertyChanged(on_property_change);

  p_impl->init_graph();
}

void GraphPlotController::Unsubscribe()
{
  p_impl->reset_graph();
}

GraphPlotController::~GraphPlotController() = default;

}  // namespace mvvm
