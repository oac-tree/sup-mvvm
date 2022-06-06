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

#include "mvvm/plotting/pen_controller.h"

#include "mvvm/model/combo_property.h"
#include "mvvm/standarditems/plottable_items.h"

#include <qcustomplot.h>

#include <stdexcept>

namespace
{
//! Returns Qt pen style from current PenItem.
Qt::PenStyle GetQtPenStyle(mvvm::PenItem* item)
{
  // our ComboProperty for pens coincide with Qt definition
  return static_cast<Qt::PenStyle>(item->GetStyleIndex());
}
}  // namespace

namespace mvvm
{

struct PenController::PenControllerImpl
{
  QCPGraph* m_graph{nullptr};
  explicit PenControllerImpl(QCPGraph* graph) : m_graph(graph)
  {
    if (!m_graph)
    {
      throw std::runtime_error("Error in PenController: uninitialised graph.");
    }
  }

  void UpdateGraphFromItem(PenItem* item) const
  {
    QColor color(QString::fromStdString(item->GetNamedColor()));

    QPen pen;
    pen.setColor(color);
    pen.setStyle(GetQtPenStyle(item));
    pen.setWidth(item->GetWidth());
    m_graph->setPen(pen);

    m_graph->parentPlot()->replot();
  }
};

PenController::PenController(QCPGraph* graph) : p_impl(std::make_unique<PenControllerImpl>(graph))
{
}

PenController::~PenController() = default;

void PenController::Subscribe()
{
  auto on_property_change = [this](auto, auto) { p_impl->UpdateGraphFromItem(GetItem()); };
  SetOnPropertyChanged(on_property_change);

  p_impl->UpdateGraphFromItem(GetItem());
}

}  // namespace mvvm
