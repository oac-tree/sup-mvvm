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

#include "axis_title_controller.h"

#include <mvvm/standarditems/plottable_items.h>

#include <qcustomplot.h>

namespace mvvm
{

struct AxisTitleController::AxisTitleControllerImpl
{
  QCPAxis* m_axis{nullptr};

  explicit AxisTitleControllerImpl(QCPAxis* axis) : m_axis(axis)
  {
    if (!axis)
    {
      throw RuntimeException("AxisTitleController: axis is not initialized.");
    }
  }

  void UpdateAxisFromItem(TextItem* item) const
  {
    auto font = m_axis->labelFont();
    font.setPointSize(item->GetSize());
    font.setFamily(QString::fromStdString(item->GetFont()));
    m_axis->setLabel(QString::fromStdString(item->GetText()));
    m_axis->setLabelFont(font);

    m_axis->parentPlot()->replot();
  }
};

AxisTitleController::AxisTitleController(QCPAxis* axis)
    : p_impl(std::make_unique<AxisTitleControllerImpl>(axis))

{
}

AxisTitleController::~AxisTitleController() = default;

void AxisTitleController::Subscribe()
{
  auto on_property_change = [this](auto) { p_impl->UpdateAxisFromItem(GetItem()); };
  Listener()->Connect<PropertyChangedEvent>(on_property_change);

  p_impl->UpdateAxisFromItem(GetItem());
}

}  // namespace mvvm
