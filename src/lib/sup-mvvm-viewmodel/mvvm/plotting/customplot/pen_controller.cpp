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

#include "pen_controller.h"

#include <mvvm/model/combo_property.h>
#include <mvvm/plotting/plot_helper.h>
#include <mvvm/standarditems/plottable_items.h>

#include <qcustomplot.h>

namespace mvvm
{

struct PenController::PenControllerImpl
{
  QCPGraph* m_graph{nullptr};
  explicit PenControllerImpl(QCPGraph* graph) : m_graph(graph)
  {
    if (!m_graph)
    {
      throw RuntimeException("Error in PenController: uninitialised graph");
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
  auto on_property_change = [this](auto) { p_impl->UpdateGraphFromItem(GetItem()); };
  Listener()->Connect<PropertyChangedEvent>(on_property_change);

  p_impl->UpdateGraphFromItem(GetItem());
}

}  // namespace mvvm
