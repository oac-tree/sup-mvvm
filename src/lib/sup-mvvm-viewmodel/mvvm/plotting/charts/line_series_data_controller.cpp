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

#include "line_series_data_controller.h"

#include "line_series_mouse_listener.h"
#include "qt_charts.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_listener.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/utils/container_utils.h>

#include <iostream>

namespace mvvm
{

LineSeriesDataController::LineSeriesDataController(QLineSeries* line_series)
    : m_qt_line_series(line_series), m_mouse_listener(CreateMouseListener())
{
  if (line_series == nullptr)
  {
    throw RuntimeException("Error in LineSeriesController: not initialized line series.");
  }

  m_qt_line_series->clear();
}

LineSeriesDataController::~LineSeriesDataController() = default;

void LineSeriesDataController::SetItem(const LineSeriesDataItem* item)
{
  if (m_data_item == item)
  {
    return;
  }

  Unsubscribe();

  if (item == nullptr)
  {
    return;
  }

  if ((item->GetModel() == nullptr) || (item->GetModel()->GetEventHandler() == nullptr))
  {
    throw RuntimeException("Error in LineSeriesController: model doesn't have signals");
  }

  m_data_item = item;

  Subscribe();
}

void LineSeriesDataController::OnItemInsertedEvent(const ItemInsertedEvent& event)
{
  auto [parent, tag_index] = event;

  if (parent == m_data_item)
  {
    auto index = tag_index.GetIndex();
    auto [new_x, new_y] = m_data_item->GetPointCoordinates(index);
    m_qt_line_series->insert(static_cast<std::int32_t>(index), {new_x, new_y});
  }
}

void LineSeriesDataController::OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent& event)
{
  if (event.item == m_data_item)
  {
    auto index = event.tag_index.GetIndex();
    m_qt_line_series->remove(static_cast<std::int32_t>(index));
  }
}

void LineSeriesDataController::OnDataChangedEvent(const DataChangedEvent& event)
{
  // We are here becase the data of either x-item, or y-item was changed.
  // Expected hierarchy:
  // - LineSeriesDataItem
  //   - PointItem
  //     - X PropertyItem
  //     - Y PropertyItem

  const auto depth = utils::GetNestingDepth(m_data_item, event.item);

  const std::int32_t kDistanceFromDataItemToXY = 2;
  if (depth == kDistanceFromDataItemToXY)
  {
    auto point_item = event.item->GetParent();
    auto index = point_item->GetTagIndex().GetIndex();

    auto [new_x, new_y] = m_data_item->GetPointCoordinates(index);
    m_qt_line_series->replace(static_cast<std::int32_t>(index), new_x + m_x_offset, new_y);
  }

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  const std::int32_t kDistanceFromDataItemToPoint = 1;
  if (depth == kDistanceFromDataItemToPoint && event.data_role == DataRole::kSelected)
  {
    auto index = event.item->GetTagIndex().GetIndex();  // index of PointItem
    m_qt_line_series->setPointSelected(static_cast<std::int32_t>(index),
                                       m_data_item->IsPointSelected(index));
  }
#else
  std::cerr << " Point selection is not supported for Qt5\n";
#endif
}

const LineSeriesDataItem* LineSeriesDataController::GetDataItem() const
{
  return m_data_item;
}

QLineSeries* LineSeriesDataController::GetQtLineSeries() const
{
  return m_qt_line_series;
}

void LineSeriesDataController::SetXOffset(double value)
{
  if (value == m_x_offset)
  {
    return;
  }

  m_x_offset = value;

  if (m_data_item != nullptr)
  {
    QList<QPointF> new_values;
    for (auto [xpos, ypos] : m_data_item->GetWaveform())
    {
      new_values.append({xpos + m_x_offset, ypos});
    }

    if (!new_values.empty())
    {
      GetQtLineSeries()->replace(new_values);
    }
  }
}

PointItem* LineSeriesDataController::GetActivePoint() const
{
  return m_active_point;
}

void LineSeriesDataController::Subscribe()
{
  InitLineSeriesData();

  m_listener = std::make_unique<mvvm::ModelListener>(m_data_item->GetModel());

  m_listener->Connect<mvvm::DataChangedEvent>(this, &LineSeriesDataController::OnDataChangedEvent);
  m_listener->Connect<mvvm::ItemInsertedEvent>(this,
                                               &LineSeriesDataController::OnItemInsertedEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(
      this, &LineSeriesDataController::OnAboutToRemoveItemEvent);
}

void LineSeriesDataController::Unsubscribe()
{
  m_data_item = nullptr;
  m_listener.reset();
  m_qt_line_series->clear();
}

std::unique_ptr<LineSeriesMouseListener> LineSeriesDataController::CreateMouseListener()
{
  auto on_point_event = [this](QLineSeries* series, std::int32_t point_index)
  {
    (void)series;
    if (point_index >= 0)
    {
      // point pressed
      auto point_item = m_data_item->GetPoint(static_cast<std::size_t>(point_index));
      m_active_point = point_item;
    }
    else
    {
      // point released
      m_active_point = nullptr;
    }
  };

  return std::make_unique<LineSeriesMouseListener>(m_qt_line_series, on_point_event);
}

void LineSeriesDataController::InitLineSeriesData()
{
  for (auto [xpos, ypos] : m_data_item->GetWaveform())
  {
    m_qt_line_series->append(xpos + m_x_offset, ypos);
  }
}

}  // namespace mvvm
