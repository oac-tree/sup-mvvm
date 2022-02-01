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
      p_impl->UpdateDataController();
    }

    if (property_name == GraphItem::kDisplayed)
    {
      p_impl->UpdateVisibility();
    }
  };
  SetOnPropertyChanged(on_property_change);

  p_impl->InitGraph();
}

void GraphPlotController::Unsubscribe()
{
  p_impl->ResetGraph();
}

GraphPlotController::~GraphPlotController() = default;

}  // namespace mvvm
