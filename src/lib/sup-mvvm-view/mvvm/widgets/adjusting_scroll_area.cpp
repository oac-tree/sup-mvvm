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

#include "adjusting_scroll_area.h"

#include <QEvent>
#include <QScrollBar>

namespace mvvm
{

AdjustingScrollArea::AdjustingScrollArea(QWidget* parent) : QScrollArea(parent)
{
  setObjectName("AdjustingScrollArea");
  setContentsMargins(0, 0, 0, 0);
  setWidgetResizable(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setStyleSheet("QScrollArea#AdjustingScrollArea {border: 0px; background-color:transparent;}");
}

void AdjustingScrollArea::SetWidget(QWidget* widget)
{
  QScrollArea::setWidget(widget);
  widget->installEventFilter(this);
}

QSize AdjustingScrollArea::sizeHint() const
{
  auto horizontal = horizontalScrollBar();
  QSize result(viewport()->width(), widget()->height() + horizontal->height() * 2);
  return result;
}

bool AdjustingScrollArea::eventFilter(QObject* object, QEvent* event)
{
  if (object == widget() && event->type() != QEvent::Resize)
  {
    widget()->setMaximumWidth(viewport()->width());
    setMaximumHeight(height() - viewport()->height() + widget()->height());
  }

  return QScrollArea::eventFilter(object, event);
}

}  // namespace mvvm
