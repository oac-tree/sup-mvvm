
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

#include "graphics_view.h"

#include "graphics_scene.h"

#include <QKeyEvent>

namespace behaviortree
{

GraphicsView::GraphicsView(GraphicsScene *graphics_scene, QWidget *parent_widget)
    : QGraphicsView(graphics_scene, parent_widget)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setRenderHint(QPainter::Antialiasing);
  setAcceptDrops(true);
  setMouseTracking(true);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
  // Currently the only function is to emit signal on delete keys.
  const bool delete_key = event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace;
  if (delete_key)
  {
    deleteSelectedRequest();
  }
  else
  {
    QGraphicsView::keyPressEvent(event);
  }
}

}  // namespace behaviortree
