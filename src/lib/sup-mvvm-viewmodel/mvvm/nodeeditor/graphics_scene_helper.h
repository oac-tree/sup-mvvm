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

#ifndef MVVM_NODEEDITOR_GRAPHICS_SCENE_HELPER_H_
#define MVVM_NODEEDITOR_GRAPHICS_SCENE_HELPER_H_

//!< @file
//! Collection of helper functions for QGraphicsScene.

#include <mvvm/utils/container_utils.h>

#include <QGraphicsScene>
#include <algorithm>

namespace mvvm
{

class SessionItem;
class ConnectableShape;
class INodePort;

/**
 * @brief Helper to get shapes of specific type from the scene.
 *
 * The result will be sorted according the order of the addition.
 */
template <typename ShapeT>
std::vector<ShapeT*> GetShapes(const QGraphicsScene& scene)
{
  auto shapes = utils::CastItems<ShapeT>(scene.items());
  std::reverse(shapes.begin(), shapes.end());
  return shapes;
}

/**
 * @brief Helper to get shapes of specific type from the scene in the given area.
 *
 * The result will be sorted according the order of the addition.
 */
template <typename ShapeT>
std::vector<ShapeT*> GetShapes(const QGraphicsScene& scene, const QRectF& area)
{
  auto shapes = utils::CastItems<ShapeT>(scene.items(area));
  std::reverse(shapes.begin(), shapes.end());
  return shapes;
}

/**
 * @brief Helper to get shapes of specific type from the scene.
 *
 * Item returned in no particular order.
 */
template <typename ShapeT>
std::vector<ShapeT*> GetSelectedShapes(const QGraphicsScene& scene)
{
  return utils::CastItems<ShapeT>(scene.selectedItems());
}

/**
 * @brief Helpers to get underlying item from the shape.
 */
SessionItem* GetUnderlyingItem(ConnectableShape* shape);

/**
 * @brief Helper method to get input port by index.
 */
INodePort* GetInputPort(const ConnectableShape& shape, std::size_t index = 0U);

/**
 * @brief Helper method to get output port by index.
 */
INodePort* GetOutputPort(const ConnectableShape& shape, std::size_t index = 0U);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_GRAPHICS_SCENE_HELPER_H_
