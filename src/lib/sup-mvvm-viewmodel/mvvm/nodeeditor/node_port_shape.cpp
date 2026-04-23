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

#include "node_port_shape.h"

#include "node_editor_constants.h"

#include <mvvm/nodeeditor/painting/node_port_shape_painter.h>

#include <QFontMetrics>

namespace mvvm
{

namespace
{

std::unique_ptr<NodePortShapePainter> CreatePainter(const PortAppearance& port_appearance)
{
  const auto label_rect =
      GetLabelRect(port_appearance.GetPortLocation(), {0, 0}, port_appearance.GetPortLabel());
  return std::make_unique<NodePortShapePainter>(port_appearance.GetPortStyle(),
                                                port_appearance.GetPortLabel(), label_rect);
}

}  // namespace

NodePortShape::NodePortShape(PortAppearance port_appearance, std::unique_ptr<INodePort> node_port,
                             QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_port_appearance(std::move(port_appearance))
    , m_node_port(std::move(node_port))
    , m_painter(CreatePainter(m_port_appearance))
{
  CalculateBoundingRect();
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

  m_node_port->SetNotifyShapeCallback(
      [this](auto operation_state)
      {
        m_operation_states = operation_state;
        update();
      });

  PropagateSceneCoordinatesToNodePort();

  if (m_node_port->GetPortDirection() == PortDirection::kInput)
  {
    setToolTip(
        "Connect to output port(s) of other nodes.\nDouble-click to hide/show children, if exist");
  }
  else if (m_node_port->GetPortDirection() == PortDirection::kOutput)
  {
    setToolTip("Connect to input port(s) of other nodes");
  }
  else
  {
  }
}

NodePortShape::~NodePortShape() = default;

QRectF NodePortShape::boundingRect() const
{
  return m_bounding_rect;
}

void NodePortShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                          QWidget* widget)
{
  (void)widget;
  m_painter->Paint(painter, option, GetPortColor());
}

INodePort* NodePortShape::GetNodePort() const
{
  return m_node_port.get();
}

QColor NodePortShape::GetPortColor() const
{
  const auto& port_style = m_port_appearance.GetPortStyle();
  if (m_node_port->GetPortDirection() == PortDirection::kInput)
  {
    return m_operation_states.IsSet(NodeOperationState::kCollapsedChildren)
               ? port_style.base_color_modified
               : port_style.base_color;
  }

  return port_style.base_color;
}

QVariant NodePortShape::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemScenePositionHasChanged || change == ItemSceneHasChanged)
  {
    PropagateSceneCoordinatesToNodePort();
  }
  return value;
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

void NodePortShape::PropagateSceneCoordinatesToNodePort()
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
QRectF GetLabelRect(PortLocation port_location, const QPointF& port_position, const QString& label)
{
  if (label.isEmpty())
  {
    return {};
  }

  QRectF label_rect;
  const QFontMetrics font_metrics = QFontMetrics(constants::GetDefaultPortLabelFont());
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
