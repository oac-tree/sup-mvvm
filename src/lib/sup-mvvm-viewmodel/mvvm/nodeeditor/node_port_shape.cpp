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

#include "node_port_shape.h"

#include <QPainter>

namespace mvvm
{

NodePortShape::NodePortShape(PortAppearance port_appearance, std::unique_ptr<INodePort> node_port,
                             QGraphicsItem *parent)
    : QGraphicsItem(parent), m_port_appearance(port_appearance), m_node_port(std::move(node_port))
{
}

QRectF NodePortShape::boundingRect() const
{
  return m_port_appearance.GetBoundingRect();
}

void NodePortShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
  (void)widget;
  (void)option;

  static const QPen pen = QPen(m_port_appearance.GetPortColor().darker(180));
  static const QBrush brush = QBrush(m_port_appearance.GetPortColor());

  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(pen);
  painter->setBrush(brush);

  const double x_size = 2.0 * m_port_appearance.GetPortRadius();
  const QPointF center(0.0, 0.0);

  QRectF ellipse_rect(0.0, 0.0, x_size, x_size);
  ellipse_rect.moveCenter(center);
  painter->drawEllipse(ellipse_rect);
  painter->restore();
}

}  // namespace mvvm
