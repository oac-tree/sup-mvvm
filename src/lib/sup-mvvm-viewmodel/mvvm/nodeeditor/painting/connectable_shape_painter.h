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

#ifndef MVVM_NODEEDITOR_PAINTING_CONNECTABLE_SHAPE_PAINTER_H_
#define MVVM_NODEEDITOR_PAINTING_CONNECTABLE_SHAPE_PAINTER_H_

#include <mvvm/nodeeditor/connectable_shape_style.h>
#include <mvvm/nodeeditor/node_editor_types.h>

class QStyleOptionGraphicsItem;
class QGraphicsItem;

namespace mvvm
{

class IConnectableShapeAdapter;

/**
 * @brief The ConnectableShapePainter class assists ConnectableShape in painting.
 */
class ConnectableShapePainter
{
public:
  ConnectableShapePainter(QGraphicsItem& shape, IConnectableShapeAdapter& shape_adapter);
  ~ConnectableShapePainter() = default;

  ConnectableShapePainter(const ConnectableShapePainter&) = delete;
  ConnectableShapePainter& operator=(const ConnectableShapePainter&) = delete;
  ConnectableShapePainter(ConnectableShapePainter&&) = delete;
  ConnectableShapePainter& operator=(ConnectableShapePainter&&) = delete;

  void Paint(QPainter* painter, const QStyleOptionGraphicsItem* option);

  /**
   * @brief Returns exact rectangle of the shape without margins.
   */
  const QRectF& GetShapeRect() const;

  /**
   * @brief Returns boundary color according to operation states.
   */
  const QColor& GetBoundaryColor(const NodeOperationStates& operation_states) const;

private:
  void InstallGraphicsEffects(QGraphicsItem& shape) const;
  static bool IsSelected(const QStyleOptionGraphicsItem* option);
  void PaintRectangle(QPainter* painter, const QStyleOptionGraphicsItem* option) const;
  void PaintLabel(QPainter* painter, const QStyleOptionGraphicsItem* option);

  IConnectableShapeAdapter& m_shape_adapter;
  ConnectableShapeStyle m_shape_style;  //!
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PAINTING_CONNECTABLE_SHAPE_PAINTER_H_
