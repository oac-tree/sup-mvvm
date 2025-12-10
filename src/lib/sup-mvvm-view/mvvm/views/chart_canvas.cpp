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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "chart_canvas.h"

#include <mvvm/plotting/charts/chart_viewport_controller.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/plotting/plot_constants.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/viewport_helper.h>
#include <mvvm/style/mvvm_style_helper.h>
#include <mvvm/views/chart_view.h>

#include <QVBoxLayout>

namespace mvvm
{

ChartCanvas::ChartCanvas(QWidget* parent_widget)
    : QWidget(parent_widget), m_chart(new QChart), m_chart_view(new mvvm::ChartView(m_chart))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(m_chart_view);

  // seems that GridAxisAnimations is buggy and can cause crash on negative axis ranges
  // FIXME: to be investigated after migration from QtChart to QtGraphs
  m_chart->setAnimationOptions(QChart::SeriesAnimations);
  if (style::IsDarkTheme())
  {
    m_chart->setTheme(QChart::ChartThemeDark);
  }

  // The only way to eliminate the narrow gray frame around canvas which came from underlying scroll
  // area container
  m_chart_view->setBackgroundBrush(m_chart->backgroundBrush());

  connect(m_chart_view, &mvvm::ChartView::OperationModeChanged, this,
          &ChartCanvas::OperationModeChanged);

  auto on_move_point_request = [this](const QPointF& position)
  {
    if (m_viewport_controller)
    {
      auto position_in_chart = m_chart->mapToValue(position);
      m_viewport_controller->UpdateActivePointCoordinate(position_in_chart.x(),
                                                         position_in_chart.y());
    }
  };
  connect(m_chart_view, &ChartView::MovePointRequested, this, on_move_point_request);
}

ChartCanvas::~ChartCanvas() = default;

void ChartCanvas::SetViewport(ChartViewportItem* viewport)
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
  auto data_area = m_viewport->GetDataArea();
  auto adjusted_area = ApplyRelativeMargins(data_area, {0.0, 0.1, 0.0, 0.0});
  auto adjusted_for_safety_area = ApplySafetyMargins(adjusted_area);
  m_viewport->SetViewportArea(adjusted_for_safety_area);
}

void ChartCanvas::SetOperationMode(int mode)
{
  m_chart_view->SetOperationMode(static_cast<CanvasOperationMode>(mode));
}

QChart* ChartCanvas::GetQtChart() const
{
  return m_chart;
}

void ChartCanvas::SetSelectedPoints(const std::vector<PointItem*>& points)
{
  if (m_viewport_controller)
  {
    m_viewport_controller->SetSelectedPoints(points);
  }
}

void ChartCanvas::wheelEvent(QWheelEvent* event)
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
