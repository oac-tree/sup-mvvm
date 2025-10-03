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

#include "node_editor_constants.h"

#include <QFontMetrics>
#include <QPainter>

namespace mvvm
{

NodePortShape::NodePortShape(PortAppearance port_appearance, std::unique_ptr<INodePort> node_port,
                             QGraphicsItem *parent)
    : QGraphicsItem(parent), m_port_appearance(port_appearance), m_node_port(std::move(node_port))
{
  CalculateBoundingRect();
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
  UpdateNodePort();
}

QRectF NodePortShape::boundingRect() const
{
  return m_bounding_rect;
}

void NodePortShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
  (void)widget;
  (void)option;
  painter->save();

  const auto &port_style = m_port_appearance.GetPortStyle();

  static const QPen pen = QPen(port_style.pen_color, port_style.pen_width);
  static const QBrush brush = QBrush(port_style.base_color);

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(pen);
  painter->setBrush(brush);

  const double x_size = 2.0 * m_port_appearance.GetPortRadius();
  const QPointF center(0.0, 0.0);

  QRectF ellipse_rect(0.0, 0.0, x_size, x_size);
  ellipse_rect.moveCenter(center);
  painter->drawEllipse(ellipse_rect);

  if (!m_port_appearance.GetPortLabel().isEmpty())
  {
    painter->setPen(Qt::black);
    painter->drawText(
        GetLabelRect(m_port_appearance.GetPortLocation(), center, m_port_appearance.GetPortLabel()),
        Qt::AlignCenter, m_port_appearance.GetPortLabel());
  }

  painter->restore();
}

INodePort *NodePortShape::GetNodePort() const
{
  return m_node_port.get();
}

QVariant NodePortShape::itemChange(GraphicsItemChange change, const QVariant &value)
{
  if (change == ItemScenePositionHasChanged || change == ItemSceneHasChanged)
  {
    UpdateNodePort();
  }
  return value;
}

void NodePortShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  (void)event;
}

void NodePortShape::CalculateBoundingRect()
{
  const double extra = 1.0;  // for the pen width
  const double radius = m_port_appearance.GetPortRadius() + extra;
  const double diameter = 2.0 * radius;

  // rect for circle with enough space for pen
  const QRectF circle_rect(-radius, -radius, diameter, diameter);

  const QRectF label_rect = GetLabelRect(m_port_appearance.GetPortLocation(), {0.0, 0.0},
                                         m_port_appearance.GetPortLabel());

  m_bounding_rect = circle_rect.united(label_rect);
}

void NodePortShape::UpdateNodePort()
{
  const auto pos = scenePos();
  m_node_port->SetScenePosition(pos.x(), pos.y());
}

/**
 * @brief Returns a rectangle for label painting.
 *
 * @param port_location The location of the port.
 * @param port_position Port coordinates (normally (0,0))
 * @param label The text of the label.
 * @return The rectangle to plot the text.
 */
QRectF GetLabelRect(PortLocation port_location, const QPointF &port_position, const QString &label)
{
  if (label.isEmpty())
  {
    return {};
  }

  QRectF label_rect;
  const QFontMetrics font_metrics = QFontMetrics(constants::DefaultPortLabelFont());
  label_rect = font_metrics.boundingRect(label);
  const double label_gap = 2.0 * constants::GetDefaultPortRadius();

  switch (port_location)
  {
  case PortLocation::kTop:
    label_rect.moveTop(port_position.y() + label_gap);
    label_rect.moveLeft(port_position.x() - (label_rect.width() / 2.0));
    break;

  case PortLocation::kBottom:
    label_rect.moveBottom(port_position.y() - label_gap);
    label_rect.moveLeft(port_position.x() - (label_rect.width() / 2.0));
    break;

  case PortLocation::kLeft:
    label_rect.moveRight(port_position.x() - label_gap);
    label_rect.moveTop(port_position.y() - (label_rect.height() / 2.0));
    break;

  case PortLocation::kRight:
    label_rect.moveLeft(port_position.x() + label_gap);
    label_rect.moveTop(port_position.y() - (label_rect.height() / 2.0));
    break;

  default:
    // unknown location
    label_rect.moveTopLeft(port_position);
    break;
  }
  return label_rect;
}

}  // namespace mvvm
