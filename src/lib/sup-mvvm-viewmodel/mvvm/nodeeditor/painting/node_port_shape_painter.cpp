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

#include "node_port_shape_painter.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

namespace mvvm
{

NodePortShapePainter::NodePortShapePainter(const NodePortStyle& port_style, const QString& label,
                                           const QRectF& label_rect)
    : m_port_style(std::move(port_style)), m_label(label), m_label_rect(label_rect)
{
}

void NodePortShapePainter::Paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                 const QColor& port_color)
{
  (void)option;
  painter->save();

  const QPen pen = QPen(m_port_style.pen_color, m_port_style.pen_width);
  const QBrush brush = QBrush(port_color);

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(pen);
  painter->setBrush(brush);

  const double x_size = 2.0 * m_port_style.radius;
  const QPointF center(0.0, 0.0);

  QRectF ellipse_rect(0.0, 0.0, x_size, x_size);
  ellipse_rect.moveCenter(center);
  painter->drawEllipse(ellipse_rect);

  if (!m_label.isEmpty())
  {
    painter->setPen(Qt::black);
    painter->drawText(m_label_rect, Qt::AlignCenter, m_label);
  }

  painter->restore();
}

}  // namespace mvvm
