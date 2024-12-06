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

#include "line_series_data_controller.h"

#include "qt_charts.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_listener.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/utils/container_utils.h>

namespace mvvm
{

LineSeriesDataController::LineSeriesDataController(QLineSeries *line_series)
    : m_qt_line_series(line_series)
{
  if (!line_series)
  {
    throw std::runtime_error("Error in LineSeriesController: not initialized line series.");
  }

  m_qt_line_series->clear();
}

LineSeriesDataController::~LineSeriesDataController() = default;

void LineSeriesDataController::SetItem(const LineSeriesDataItem *item)
{
  if (m_data_item == item)
  {
    return;
  }

  Unsubscribe();

  if (!item)
  {
    return;
  }

  if (!item->GetModel() || !item->GetModel()->GetEventHandler())
  {
    throw std::runtime_error("Error in LineSeriesController: model doesn't have signals");
  }

  m_data_item = item;

  Subscribe();
}

void LineSeriesDataController::OnModelEvent(const ItemInsertedEvent &event)
{
  auto [parent, tag_index] = event;

  if (parent == m_data_item)
  {
    auto index = tag_index.index;
    auto [new_x, new_y] = m_data_item->GetPointCoordinates(index);
    m_qt_line_series->insert(index, {new_x, new_y});
  }
}

void LineSeriesDataController::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  if (event.item == m_data_item)
  {
    auto index = event.tag_index.index;
    m_qt_line_series->remove(index);
  }
}

void LineSeriesDataController::OnModelEvent(const DataChangedEvent &event)
{
  // We are here becase the data of either x-item, or y-item was changed.
  // Expected hierarchy:
  // - LineSeriesDataItem
  //   - PointItem
  //     - X PropertyItem
  //     - Y PropertyItem

  const int expected_distance_to_data_item = 2;
  if (auto depth = utils::GetNestingDepth(m_data_item, event.item);
      depth == expected_distance_to_data_item)
  {
    auto point_item = event.item->GetParent();
    auto index = point_item->GetTagIndex().index;

    auto [new_x, new_y] = m_data_item->GetPointCoordinates(index);
    m_qt_line_series->replace(index, new_x + m_x_offset, new_y);
  }
}

const LineSeriesDataItem *LineSeriesDataController::GetDataItem() const
{
  return m_data_item;
}

QLineSeries *LineSeriesDataController::GetQtLineSeries() const
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

  if (m_data_item)
  {
    QList<QPointF> new_values;
    for (auto [x, y] : m_data_item->GetWaveform())
    {
      new_values.append({x + m_x_offset, y});
    }

    if (!new_values.empty())
    {
      GetQtLineSeries()->replace(new_values);
    }
  }
}

void LineSeriesDataController::Subscribe()
{
  InitLineSeriesData();

  m_listener = std::make_unique<mvvm::ModelListener>(m_data_item->GetModel());

  m_listener->Connect<mvvm::DataChangedEvent>(this, &LineSeriesDataController::OnModelEvent);
  m_listener->Connect<mvvm::ItemInsertedEvent>(this, &LineSeriesDataController::OnModelEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(this, &LineSeriesDataController::OnModelEvent);
}

void LineSeriesDataController::Unsubscribe()
{
  m_data_item = nullptr;
  m_listener.reset();
  m_qt_line_series->clear();
}

void LineSeriesDataController::InitLineSeriesData()
{
  for (auto [x, y] : m_data_item->GetWaveform())
  {
    m_qt_line_series->append(x + m_x_offset, y);
  }
}

}  // namespace mvvm
