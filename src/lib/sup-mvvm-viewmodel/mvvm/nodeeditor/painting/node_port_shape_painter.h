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

#ifndef MVVM_NODEEDITOR_PAINTING_NODE_PORT_SHAPE_PAINTER_H_
#define MVVM_NODEEDITOR_PAINTING_NODE_PORT_SHAPE_PAINTER_H_

#include <mvvm/nodeeditor/node_port_style.h>

class QPainter;
class QStyleOptionGraphicsItem;

namespace mvvm
{

/**
 * @brief The NodePortShapePainter class assists NodePortShape in painting.
 */
class NodePortShapePainter
{
public:
  explicit NodePortShapePainter(const NodePortStyle& port_style, const QString& label,
                                const QRectF& label_rect);
  ~NodePortShapePainter() = default;

  NodePortShapePainter(const NodePortShapePainter&) = delete;
  NodePortShapePainter& operator=(const NodePortShapePainter&) = delete;
  NodePortShapePainter(NodePortShapePainter&&) = delete;
  NodePortShapePainter& operator=(NodePortShapePainter&&) = delete;

  void Paint(QPainter* painter, const QStyleOptionGraphicsItem* option, const QColor& port_color);

private:
  NodePortStyle m_port_style;
  QString m_label;
  QRectF m_label_rect;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PAINTING_NODE_PORT_SHAPE_PAINTER_H_
