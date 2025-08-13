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

#include "mouse_move_reporter.h"

#include "mouse_pos_info.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <qcustomplot.h>

#include <QMouseEvent>

namespace mvvm
{

struct MouseMoveReporter::MouseMoveReporterImpl
{
  QCustomPlot* m_custom_plot{nullptr};
  callback_t m_callback;
  MouseMoveReporterImpl(QCustomPlot* custom_plot, callback_t callback)
      : m_custom_plot(custom_plot), m_callback(std::move(callback))
  {
    if (!m_custom_plot)
    {
      throw RuntimeException("MouseMoveReporter: not initialized custom plot");
    }

    if (!m_callback)
    {
      throw RuntimeException("MouseMoveReporter: not initialized callback");
    }

    custom_plot->setMouseTracking(true);
    set_connected();
  }

  void set_connected()
  {
    auto on_mouse_move = [this](QMouseEvent* event)
    {
      const double x = pixelToXaxisCoord(event->pos().x());
      const double y = pixelToYaxisCoord(event->pos().y());
      m_callback({x, y, axesRangeContains(x, y)});
    };

    QObject::connect(m_custom_plot, &QCustomPlot::mouseMove, on_mouse_move);
  }

  double pixelToXaxisCoord(double pixel) const { return m_custom_plot->xAxis->pixelToCoord(pixel); }

  double pixelToYaxisCoord(double pixel) const { return m_custom_plot->yAxis->pixelToCoord(pixel); }

  bool axesRangeContains(double xpos, double ypos) const
  {
    return m_custom_plot->xAxis->range().contains(xpos)
           && m_custom_plot->yAxis->range().contains(ypos);
  }
};

MouseMoveReporter::MouseMoveReporter(QCustomPlot* custom_plot, callback_t callback)
    : p_impl(std::make_unique<MouseMoveReporterImpl>(custom_plot, callback))
{
}

MouseMoveReporter::~MouseMoveReporter() = default;

}  // namespace mvvm
