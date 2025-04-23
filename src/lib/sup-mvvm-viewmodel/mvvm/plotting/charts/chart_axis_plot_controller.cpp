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

#include "chart_axis_plot_controller.h"

#include "chart_axis_title_controller.h"
#include "qt_charts.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/plottable_items.h>

namespace mvvm
{

ChartAxisPlotController::ChartAxisPlotController(QAbstractAxis *axis)
    : m_axis(axis), m_axis_connection(std::make_unique<QMetaObject::Connection>())
{
  if (!m_axis)
  {
    throw RuntimeException("Error in ChartAxisPlotController: axis is not initialized");
  }
}

ChartAxisPlotController::~ChartAxisPlotController() = default;

QAbstractAxis *ChartAxisPlotController::GetQtAxis()
{
  return m_axis;
}

void ChartAxisPlotController::Subscribe()
{
  SetAxisRangeFromItem();

  m_title_controller = std::make_unique<ChartAxisTitleController>(GetQtAxis());
  m_title_controller->SetItem(GetItem()->GetTitleItem());

  SetQtConnected();

  Listener()->Connect<PropertyChangedEvent>(this, &ChartAxisPlotController::OnPropertyChangedEvent);
}

void ChartAxisPlotController::Unsubscribe()
{
  SetQtDisonnected();
}

void ChartAxisPlotController::SetQtConnected()
{
  if (auto value_axis = dynamic_cast<QValueAxis *>(m_axis); value_axis)
  {
    auto on_axis_range = [this](double min, double max) { GetItem()->SetRange(min, max); };

    *m_axis_connection = QObject::connect(value_axis, &QValueAxis::rangeChanged, on_axis_range);
  }
}

void ChartAxisPlotController::SetQtDisonnected()
{
  QObject::disconnect(*m_axis_connection);
}

void ChartAxisPlotController::SetAxisRangeFromItem()
{
  m_axis->setRange(GetItem()->GetMin(), GetItem()->GetMax());
}

void ChartAxisPlotController::OnPropertyChangedEvent(const PropertyChangedEvent &event)
{
  auto [item, name] = event;

  if (name == ViewportAxisItem::kMin || name == ViewportAxisItem::kMax)
  {
    SetQtDisonnected();
    SetAxisRangeFromItem();
    SetQtConnected();
  }
}

}  // namespace mvvm
