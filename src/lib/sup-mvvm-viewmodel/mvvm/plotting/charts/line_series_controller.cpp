/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "line_series_controller.h"

#include "line_series_data_controller.h"
#include "qt_charts.h"  // IWYU pragma: export

#include <mvvm/model/item_constants.h>
#include <mvvm/plotting/charts/chart_pen_controller.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/plottable_items.h>
#include <mvvm/standarditems/point_item.h>

#include <iostream>

namespace mvvm
{

LineSeriesController::LineSeriesController(QLineSeries* line_series)
    : m_data_controller(std::make_unique<LineSeriesDataController>(line_series))
    , m_pen_controller(std::make_unique<ChartPenController>(line_series))
{
}

LineSeriesController::~LineSeriesController() = default;

void LineSeriesController::Subscribe()
{
  auto line_series_item = GetItem();
  m_data_controller->SetXOffset(GetItemRef().GetXOffset());
  m_data_controller->SetItem(line_series_item->GetDataItem());

  m_pen_controller->SetItem(line_series_item->GetPenItem());

  GetQtLineSeries()->setName(QString::fromStdString(line_series_item->GetDisplayName()));
  GetQtLineSeries()->setVisible(GetItemRef().IsDisplayed());
  GetQtLineSeries()->setPointsVisible(GetItemRef().IsPointsVisible());
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  GetQtLineSeries()->setMarkerSize(GetItemRef().GetMarkerSize());
#else
  std::cerr << " Marker size is not supported for Qt5\n";
#endif

  Listener()->Connect<PropertyChangedEvent>(this, &LineSeriesController::OnPropertyChanged);
  Listener()->Connect<DataChangedEvent>(this, &LineSeriesController::OnDataChanged);
}

QLineSeries* LineSeriesController::GetQtLineSeries() const
{
  return m_data_controller->GetQtLineSeries();
}

void LineSeriesController::UpdateActivePointCoordinate(double x, double y)
{
  if (auto active_point = m_data_controller->GetActivePoint(); active_point != nullptr)
  {
    active_point->SetX(x);
    active_point->SetY(y);
  }
}

void LineSeriesController::SetPointsSelected(const std::vector<PointItem*>& points)
{
  m_data_controller->SetPointsSelected(points);
}

void LineSeriesController::OnPropertyChanged(const PropertyChangedEvent& event)
{
  if (event.name == constants::kLink)
  {
    // If linked property has changed, it could mean that we've got a new data to look at.
    m_data_controller->SetItem(GetItemRef().GetDataItem());
    return;
  }

  if (event.name == LineSeriesItem::kOffset)
  {
    m_data_controller->SetXOffset(GetItemRef().GetXOffset());
    return;
  }

  if (event.name == constants::kDisplayed)
  {
    GetQtLineSeries()->setVisible(GetItemRef().IsDisplayed());
    return;
  }

  if (event.name == LineSeriesItem::kPointsVisible)
  {
    GetQtLineSeries()->setPointsVisible(GetItemRef().IsPointsVisible());
    return;
  }

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  if (event.name == LineSeriesItem::kMarkerSize)
  {
    GetQtLineSeries()->setMarkerSize(GetItemRef().GetMarkerSize());
    return;
  }
#else
  std::cerr << " Marker size is not supported for Qt5\n";
#endif
}

void LineSeriesController::OnDataChanged(const DataChangedEvent& event)
{
  if (event.data_role == DataRole::kDisplay)
  {
    GetQtLineSeries()->setName(QString::fromStdString(GetItemRef().GetDisplayName()));
  }
}

}  // namespace mvvm
