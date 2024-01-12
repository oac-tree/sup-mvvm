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

#include "mouse_move_reporter.h"

#include "mouse_pos_info.h"

#include <mvvm/core/exceptions.h>

#include <qcustomplot.h>

#include <QMouseEvent>

namespace mvvm
{

struct MouseMoveReporter::MouseMoveReporterImpl
{
  MouseMoveReporter* reporter{nullptr};
  QCustomPlot* custom_plot{nullptr};
  callback_t callback;
  MouseMoveReporterImpl(MouseMoveReporter* reporter, QCustomPlot* custom_plot, callback_t callback)
      : reporter(reporter), custom_plot(custom_plot), callback(std::move(callback))
  {
    if (!custom_plot)
    {
      throw RuntimeException("MouseMoveReporter: not initialized custom plot");
    }

    custom_plot->setMouseTracking(true);
    set_connected();
  }

  void set_connected()
  {
    auto on_mouse_move = [this](QMouseEvent* event)
    {
      double x = pixelToXaxisCoord(event->pos().x());
      double y = pixelToYaxisCoord(event->pos().y());
      if (callback)
        callback({x, y, axesRangeContains(x, y)});
    };

    QObject::connect(custom_plot, &QCustomPlot::mouseMove, on_mouse_move);
  }

  double pixelToXaxisCoord(double pixel) const { return custom_plot->xAxis->pixelToCoord(pixel); }

  double pixelToYaxisCoord(double pixel) const { return custom_plot->yAxis->pixelToCoord(pixel); }

  bool axesRangeContains(double xpos, double ypos) const
  {
    return custom_plot->xAxis->range().contains(xpos) && custom_plot->yAxis->range().contains(ypos);
  }
};

MouseMoveReporter::MouseMoveReporter(QCustomPlot* custom_plot, callback_t callback)
    : p_impl(std::make_unique<MouseMoveReporterImpl>(this, custom_plot, callback))
{
}

MouseMoveReporter::~MouseMoveReporter() = default;

}  // namespace mvvm
