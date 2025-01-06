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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "line_series_controller.h"

#include "line_series_data_controller.h"
#include "qt_charts.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/plotting/charts/chart_pen_controller.h>
#include <mvvm/standarditems/line_series_data_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/plottable_items.h>

namespace mvvm
{

LineSeriesController::LineSeriesController(QLineSeries *line_series)
    : m_data_controller(std::make_unique<LineSeriesDataController>(line_series))
    , m_pen_controller(std::make_unique<ChartPenController>(line_series))
{
}

LineSeriesController::~LineSeriesController() = default;

void LineSeriesController::Subscribe()
{
  auto line_series_item = GetItem();
  m_data_controller->SetXOffset(GetItem()->GetXOffset());
  m_data_controller->SetItem(line_series_item->GetDataItem());

  m_pen_controller->SetItem(line_series_item->GetPenItem());

  GetQtLineSeries()->setName(QString::fromStdString(line_series_item->GetDisplayName()));
  GetQtLineSeries()->setVisible(GetItem()->IsDisplayed());

  Listener()->Connect<PropertyChangedEvent>(this, &LineSeriesController::OnPropertyChanged);
  Listener()->Connect<DataChangedEvent>(this, &LineSeriesController::OnDataChanged);
}

QLineSeries *LineSeriesController::GetQtLineSeries() const
{
  return m_data_controller->GetQtLineSeries();
}

void LineSeriesController::OnPropertyChanged(const PropertyChangedEvent &event)
{
  if (event.name == constants::kLink)
  {
    // If linked property has changed, it could mean that we've got a new data to look at.
    auto line_series_item = GetItem();
    m_data_controller->SetItem(line_series_item->GetDataItem());
    return;
  }

  if (event.name == LineSeriesItem::kOffset)
  {
    m_data_controller->SetXOffset(GetItem()->GetXOffset());
    return;
  }

  if (event.name == constants::kDisplayed)
  {
    GetQtLineSeries()->setVisible(GetItem()->IsDisplayed());
    return;
  }
}

void LineSeriesController::OnDataChanged(const mvvm::DataChangedEvent &event)
{
  if (event.data_role == mvvm::DataRole::kDisplay)
  {
    GetQtLineSeries()->setName(QString::fromStdString(GetItem()->GetDisplayName()));
  }
}

}  // namespace mvvm
