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

#include "graph_canvas.h"

#include "graph_viewport_plot_controller.h"
#include "status_string_reporter.h"
#include "status_string_reporter_factory.h"

#include <mvvm/standarditems/graph_viewport_item.h>
#include <mvvm/widgets/status_label.h>

#include <qcustomplot.h>

#include <QBoxLayout>

namespace
{

//! Returns policy to which side of the axes box margins can be applied.
//! If number is negative, this side will be callulated automatically.

// FIXME autoMarginPolicy move to utils, provide unit tests
QCP::MarginSides autoMarginPolicy(int left, int top, int right, int bottom)
{
  QCP::MarginSides result{QCP::msAll};
  if (left >= 0)
    result &= ~QCP::msLeft;
  if (top >= 0)
    result &= ~QCP::msTop;
  if (right >= 0)
    result &= ~QCP::msRight;
  if (bottom >= 0)
    result &= ~QCP::msBottom;
  return result;
}
}  // namespace

namespace mvvm
{

struct GraphCanvas::GraphCanvasImpl
{
  QCustomPlot* custom_plot{nullptr};
  std::unique_ptr<GraphViewportPlotController> viewport_controller;
  std::unique_ptr<StatusStringReporter> reporter;
  StatusLabel* status_label{nullptr};

  GraphCanvasImpl() : custom_plot(new QCustomPlot), status_label(new StatusLabel)
  {
    viewport_controller = std::make_unique<GraphViewportPlotController>(custom_plot);

    auto on_mouse_move = [this](const std::string& str)
    { status_label->setText(QString::fromStdString(str)); };
    reporter = CreateGraphReporter(custom_plot, on_mouse_move);
  }

  //! Updates viewport.
  void setViewportToContent()
  {
    if (!viewport_controller->GetItem())
    {
      return;
    }
    viewport_controller->GetItem()->SetViewportToContent();
  }

  //! Updates viewport.
  void setViewportToContent(double left, double top, double right, double bottom)
  {
    if (!viewport_controller->GetItem())
    {
      return;
    }
    viewport_controller->GetItem()->SetViewportToContent(left, top, right, bottom);
  }

  QCustomPlot* customPlot() { return custom_plot; }
};

GraphCanvas::GraphCanvas(QWidget* parent)
    : QWidget(parent), p_impl(std::make_unique<GraphCanvasImpl>())
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(p_impl->custom_plot);
  layout->addWidget(p_impl->status_label);
  setLayout(layout);

  setMouseTracking(true);
  p_impl->customPlot()->setMouseTracking(true);
  p_impl->customPlot()->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  p_impl->customPlot()->axisRect()->setupFullAxesBox(true);

  auto on_replot = [this]()
  {
    QMargins margins = p_impl->customPlot()->axisRect()->margins();
    axisMarginsChanged(margins.left(), margins.top(), margins.right(), margins.bottom());
  };
  connect(p_impl->customPlot(), &QCustomPlot::afterReplot, this, on_replot);
}

GraphCanvas::~GraphCanvas() = default;

void GraphCanvas::SetItem(GraphViewportItem* viewport_item)
{
  p_impl->viewport_controller->SetItem(viewport_item);
}

void GraphCanvas::SetViewportToContent(double left, double top, double right, double bottom)
{
  p_impl->setViewportToContent(left, top, right, bottom);
}

void GraphCanvas::SetViewportToContent()
{
  p_impl->setViewportToContent();
}

//! Set margins between axes rectangle and widget borders.
//! If the value is negative, leave old margin intact and allow automatic margin adjustment.

void GraphCanvas::SetAxisMargins(int left, int top, int right, int bottom)
{
  auto customPlot = p_impl->customPlot();
  customPlot->axisRect()->setAutoMargins(autoMarginPolicy(left, top, right, bottom));

  QMargins orig = customPlot->axisRect()->margins();
  int new_left = left >= 0 ? left : orig.left();
  int new_top = top >= 0 ? top : orig.top();
  int new_right = right >= 0 ? right : orig.right();
  int new_bottom = bottom >= 0 ? bottom : orig.bottom();
  customPlot->axisRect()->setMargins(QMargins(new_left, new_top, new_right, new_bottom));

  customPlot->replot();
}

}  // namespace mvvm
