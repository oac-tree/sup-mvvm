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

#ifndef MVVM_NODEEDITOR_PAINTING_CONNECTABLE_SHAPE_PAINTER_H
#define MVVM_NODEEDITOR_PAINTING_CONNECTABLE_SHAPE_PAINTER_H

#include <mvvm/nodeeditor/connectable_shape_style.h>

class QStyleOptionGraphicsItem;

namespace mvvm
{

class ConnectableShape;
class IConnectableShapeAdapter;

/**
 * @brief The ConnectableShapePainter class assists ConnectableShape in painting.
 */
class ConnectableShapePainter
{
public:
  ConnectableShapePainter(ConnectableShapeStyle m_shape_style, ConnectableShape* shape);

  void Paint(QPainter* painter, const QStyleOptionGraphicsItem* option);

  /**
   * @brief Returns exact rectangle of the shape without margins.
   */
  QRectF GetShapeRect() const;

private:
  void InstallGraphicsEffects();

  /**
   * @brief Returns underlying connectable shape adapter.
   */
  IConnectableShapeAdapter* GetConnectableShapeAdapter();

  ConnectableShapeStyle m_shape_style;
  ConnectableShape* m_shape{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PAINTING_CONNECTABLE_SHAPE_PAINTER_H
