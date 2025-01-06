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

#include "chart_canvas.h"

#include <mvvm/plotting/charts/chart_viewport_controller.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/plotting/plot_constants.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/views/chart_view.h>

#include <QVBoxLayout>

namespace mvvm
{

ChartCanvas::ChartCanvas(QWidget *parent)
    : QWidget(parent), m_chart(new QChart), m_chart_view(new mvvm::ChartView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_chart_view);

  m_chart_view->setChart(m_chart);

  // The only way to eliminate the narrow gray frame around canvas which came from underlying scroll
  // area container
  m_chart_view->setBackgroundBrush(m_chart->backgroundBrush());

  connect(m_chart_view, &mvvm::ChartView::OperationModeChanged, this,
          &ChartCanvas::OperationModeChanged);
}

ChartCanvas::~ChartCanvas() = default;

void ChartCanvas::SetViewport(ChartViewportItem *viewport)
{
  m_viewport = viewport;
  m_viewport_controller = std::make_unique<ChartViewportController>(m_chart);
  m_viewport_controller->SetItem(viewport);
  m_chart_view->setRenderHint(QPainter::Antialiasing);
}

void ChartCanvas::ZoomIn()
{
  m_chart->zoom(constants::kDefaultZoomInFactor);
}

void ChartCanvas::ZoomOut()
{
  m_chart->zoom(1. / constants::kDefaultZoomInFactor);
}

void ChartCanvas::SetViewportToContent()
{
  // add 10% empty gap on top, above all graphs
  m_viewport->SetViewportToContent(0.0, 0.1, 0.0, 0.0);
}

void ChartCanvas::SetOperationMode(int mode)
{
  m_chart_view->SetOperationMode(static_cast<CanvasOperationMode>(mode));
}

void ChartCanvas::wheelEvent(QWheelEvent *event)
{
  if (event->angleDelta().y() > 0)
  {
    m_chart->zoom(constants::kWheelDefaultZoomInFactor);
  }
  else
  {
    m_chart->zoom(1. / constants::kWheelDefaultZoomInFactor);
  }
}

}  // namespace mvvm
