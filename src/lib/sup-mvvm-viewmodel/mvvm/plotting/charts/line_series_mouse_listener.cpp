/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "line_series_mouse_listener.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/providers/qtcore_helper.h>

namespace mvvm
{

LineSeriesMouseListener::LineSeriesMouseListener(QLineSeries* line_series,
                                                 const ReportCallback& callback,
                                                 QObject* parent_object)
    : QObject(parent_object), m_line_series(line_series), m_callback(callback)
{
  if (m_line_series == nullptr)
  {
    throw RuntimeException("Line series is not initialised");
  }

  (void)connect(line_series, &QLineSeries::pressed, this, &LineSeriesMouseListener::OnPressed);
  (void)connect(line_series, &QLineSeries::released, this, &LineSeriesMouseListener::OnReleased);
}

std::int32_t LineSeriesMouseListener::GetNearestPointIndex(const QPointF& point) const
{
  return ::mvvm::utils::GetNearestPointIndex(m_line_series->points(), point);
}

void LineSeriesMouseListener::OnPressed(const QPointF& point)
{
  if (m_callback)
  {
    const auto index = GetNearestPointIndex(point);
    m_callback(m_line_series, index);
  }
}

void LineSeriesMouseListener::OnReleased(const QPointF& point)
{
  (void)point;
  if (m_callback)
  {
    // Report -1 as point index on release.
    // The release can happen on any place of canvas, even when hovering another line series.
    m_callback(m_line_series, -1);
  }
}

}  // namespace mvvm
