/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/plotting/axistitlecontroller.h"

#include "mvvm/standarditems/plottableitems.h"

#include <qcustomplot.h>

#include <stdexcept>

namespace mvvm
{

struct AxisTitleController::AxisTitleControllerImpl
{
  QCPAxis* m_axis{nullptr};

  AxisTitleControllerImpl(QCPAxis* axis) : m_axis(axis)
  {
    if (!axis)
    {
      throw std::runtime_error("AxisTitleController: axis is not initialized.");
    }
  }

  void UpdateAxisFromItem(TextItem* item)
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
  auto on_property_change = [this](auto, auto) { p_impl->UpdateAxisFromItem(GetItem()); };
  SetOnPropertyChanged(on_property_change);

  p_impl->UpdateAxisFromItem(GetItem());
}

}  // namespace mvvm
