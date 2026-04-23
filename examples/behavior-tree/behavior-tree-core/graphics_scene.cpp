/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "graphics_scene.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>

namespace behaviortree
{

namespace
{

QRectF GetDefaultSceneRect()
{
  const double kWindowSize = 640;
  const QPointF scene_origin{-kWindowSize / 2.0, 0.0};
  const QSizeF scene_size{kWindowSize, kWindowSize};

  return {scene_origin, scene_size};
}

}  // namespace

GraphicsScene::GraphicsScene(QObject *parent_object) : QGraphicsScene(parent_object)
{
  setSceneRect(GetDefaultSceneRect());
}

void GraphicsScene::OnDeleteSelectedRequest()
{
  for (auto shape : mvvm::GetSelectedShapes<mvvm::ConnectableShape>(*this))
  {
    mvvm::utils::RemoveItem(*mvvm::GetUnderlyingItem(shape));
  }
}

}  // namespace behaviortree
