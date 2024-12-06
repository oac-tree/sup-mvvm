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

#include "chart_axis_title_controller.h"

#include <mvvm/standarditems/plottable_items.h>

#include "qt_charts.h"

namespace mvvm
{

ChartAxisTitleController::ChartAxisTitleController(QAbstractAxis *axis) : m_axis(axis)
{
  if (!m_axis)
  {
    throw RuntimeException("Error in ChartAxisTitleController: axis is not initialized");
  }
}

QAbstractAxis *ChartAxisTitleController::GetQtAxis()
{
  return m_axis;
}

void ChartAxisTitleController::Subscribe()
{
  auto on_property_change = [this](auto) { UpdateAxisTitleFromItem(); };
  Listener()->Connect<PropertyChangedEvent>(on_property_change);

  UpdateAxisTitleFromItem();
}

void ChartAxisTitleController::UpdateAxisTitleFromItem()
{
  auto font = GetQtAxis()->titleFont();
  font.setPointSize(GetItem()->GetPointSize());
  font.setFamily(QString::fromStdString(GetItem()->GetFont()));
  m_axis->setTitleText(QString::fromStdString(GetItem()->GetText()));
  m_axis->setTitleFont(font);
}

}  // namespace mvvm
