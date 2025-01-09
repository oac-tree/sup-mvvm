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

#include "chart_pen_controller.h"

#include <mvvm/plotting/plot_helper.h>
#include <mvvm/standarditems/plottable_items.h>

#include <stdexcept>

namespace mvvm
{

ChartPenController::ChartPenController(QLineSeries *line_series) : m_qt_line_series(line_series)
{
  if (!line_series)
  {
    throw std::runtime_error("Error in PenController: uninitialized line series.");
  }
}

QLineSeries *ChartPenController::GetQtLineSeries() const
{
  return m_qt_line_series;
}

void ChartPenController::Subscribe()
{
  Listener()->Connect<PropertyChangedEvent>(this, &ChartPenController::OnPropertyChanged);
  UpdateLineSeriesFromItem();
}

void ChartPenController::OnPropertyChanged(const PropertyChangedEvent &event)
{
  (void)event;
  UpdateLineSeriesFromItem();
}

void ChartPenController::UpdateLineSeriesFromItem()
{
  QColor color(QString::fromStdString(GetItem()->GetNamedColor()));

  QPen pen;
  pen.setColor(color);
  pen.setStyle(GetQtPenStyle(GetItem()));
  pen.setWidth(GetItem()->GetWidth());
  m_qt_line_series->setPen(pen);
}

}  // namespace mvvm
