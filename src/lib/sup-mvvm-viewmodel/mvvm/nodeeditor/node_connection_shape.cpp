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

#include "node_connection_shape.h"

#include "i_node_port.h"

#include <mvvm/nodeeditor/painting/node_connection_painter.h>

#include <QPainter>
#include <QPen>

namespace mvvm
{

NodeConnectionShape::NodeConnectionShape(NodeTreeLayout tree_layout,
                                         std::unique_ptr<INodeConnection> node_connection)
    : QGraphicsPathItem(nullptr)
    , m_tree_layout(tree_layout)
    , m_node_connection(std::move(node_connection))
    , m_painter(std::make_unique<NodeConnectionPainter>())
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);

  setZValue(-1);  // hidden beneath port circles

  m_node_connection->SetNotifyShapeCallback([this]() { UpdatePath(); });
  UpdatePath();
}

NodeConnectionShape::~NodeConnectionShape() = default;

INodeConnection* NodeConnectionShape::GetNodeConnection()
{
  return m_node_connection.get();
}

void NodeConnectionShape::UpdatePath()
{
  auto path = CreatePainterPath();
  setPath(path);
}

void NodeConnectionShape::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                QWidget* widget)
{
  (void)widget;
  m_painter->Paint(painter, option, path());
}

bool NodeConnectionShape::IsHanging() const
{
  return m_node_connection->GetStartPort() == nullptr || m_node_connection->GetEndPort() == nullptr;
}

QPainterPath NodeConnectionShape::CreatePainterPath() const
{
  auto start_port = m_node_connection->GetStartPort();
  auto end_port = m_node_connection->GetEndPort();

  if (!start_port || !end_port)
  {
    return {};
  }

  const auto [xpos1, ypos1] = start_port->GetScenePosition();
  const auto [xpos2, ypos2] = end_port->GetScenePosition();
  const bool start_is_input = start_port->GetPortDirection() == PortDirection::kInput;
  const QPointF pos1(xpos1, ypos1);
  const QPointF pos2(xpos2, ypos2);

  QPainterPath painter_path;
  painter_path.moveTo(pos1);

  if (m_tree_layout == NodeTreeLayout::kTopToBottom)
  {
    qreal dist = pos2.y() - pos1.y();
    dist = qMax(dist, 200.);
    const double step = start_is_input ? dist * 0.25 : -dist * 0.25;
    const QPointF ctr1(pos1.x(), pos1.y() + step);
    const QPointF ctr2(pos2.x(), pos2.y() - step);
    painter_path.cubicTo(ctr1, ctr2, pos2);
  }
  else
  {
    qreal dist = pos2.x() - pos1.x();
    dist = qMax(dist, 200.);
    const QPointF ctr1(pos1.x() + (dist * 0.25), pos1.y());
    const QPointF ctr2(pos2.x() - (dist * 0.25), pos2.y());
    painter_path.cubicTo(ctr1, ctr2, pos2);
  }

  return painter_path;
}

}  // namespace mvvm
