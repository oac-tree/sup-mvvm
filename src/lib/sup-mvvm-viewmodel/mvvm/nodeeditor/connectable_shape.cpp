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

#include "connectable_shape.h"

#include "i_connectable_shape_adapter.h"
#include "i_port_position_strategy.h"
#include "node_editor_constants.h"
#include "node_editor_helper.h"
#include "node_port.h"
#include "node_port_shape.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace mvvm
{

namespace
{
const int kRoundPar = 5;

/**
 * @brief Returns rectangle to display ConnectableShape label.
 *
 * The label will be occupying top of the bounding box rectangle of a shape.
 */
QRectF GetLabelRectangle(const QRectF &rect)
{
  // take a rectangle which is a half in height of the view's rectangle
  QRectF result(rect.x(), rect.y(), rect.width(), rect.height() / 2);

  // slightly shrink it
  auto side_gap = constants::NodeUnitSize(0.25);
  auto top_gap = constants::NodeUnitSize(0.75);
  return result.marginsRemoved(
      QMarginsF(side_gap, top_gap, side_gap, 0.0));  // left, top, right, bottom
}

/**
 * @brief Returns label font to display the name of the ConnectableShape.
 */
QFont GetLabelFont()
{
  QFont result("Noto Sans", constants::NodeUnitSize(0.8), QFont::Bold);
  result.setWordSpacing(-constants::NodeUnitSize(0.25));
  return result;
}

}  // namespace

ConnectableShape::ConnectableShape(std::unique_ptr<IConnectableShapeAdapter> shape_adapter,
                                   std::unique_ptr<IPortPositionStrategy> port_position_strategy,
                                   QGraphicsItem *parent_item)
    : QGraphicsItem(parent_item)
    , m_shape_adapter(std::move(shape_adapter))
    , m_port_position_strategy(std::move(port_position_strategy))
{
  if (!m_shape_adapter)
  {
    throw RuntimeException("Shape adapter is not provided");
  }

  if (!m_port_position_strategy)
  {
    throw RuntimeException("Port position strategy is not provided");
  }

  CalculateBoundingBox();
  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);

  SetupPorts();
  UpdateViewCoordinatesFromItem();
}

ConnectableShape::~ConnectableShape() = default;

QRectF ConnectableShape::boundingRect() const
{
  return m_bounding_box;
}

void ConnectableShape::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
  (void)widget;

  painter->setPen(Qt::gray);
  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    painter->setPen(Qt::DashLine);
  }

  painter->setBrush(
      constants::ConnectableShapeGradient(m_shape_adapter->GetBaseColor(), boundingRect()));
  painter->drawRoundedRect(boundingRect(), kRoundPar, kRoundPar);

  // plotting label
  static const QFont label_font = GetLabelFont();
  static const QRectF label_rect = GetLabelRectangle(boundingRect());
  painter->setFont(label_font);
  painter->setPen(Qt::black);
  QTextOption text_option(Qt::AlignHCenter | Qt::AlignTop);
  text_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  painter->drawText(label_rect, m_shape_adapter->GetDisplayName(), text_option);
}

void ConnectableShape::CalculateBoundingBox()
{
  // adding margins for a pen
  m_bounding_box =
      constants::DefaultConnectableShapeRect().marginsAdded(QMarginsF(1.0, 1.0, 1.0, 1.0));
}

void ConnectableShape::SetupPorts()
{
  const auto ports_info = m_shape_adapter->GetPortInfos();
  SetupPortsForDirection(ports_info, PortDirection::kInput);
  SetupPortsForDirection(ports_info, PortDirection::kOutput);
}

void ConnectableShape::SetupPortsForDirection(const std::vector<PortInfo> &port_infos,
                                              PortDirection port_direction)
{
  auto same_direction_ports = GetPortInfo(port_infos, port_direction);

  std::size_t index{0};
  for (const auto &port_info : same_direction_ports)
  {
    auto port_shape = CreateNodePortShape(port_info);
    auto port_shape_ptr = port_shape.get();
    const auto position = m_port_position_strategy->GetPortPosition(
        port_direction, index, same_direction_ports.size(), boundingRect());
    port_shape.release()->setParentItem(this);  // take ownership

    port_shape_ptr->setPos(position);

    ++index;
  }
}

std::unique_ptr<QGraphicsItem> ConnectableShape::CreateNodePortShape(const PortInfo &port_info)
{
  const auto appearance =
      GetDefaultPortAppearance(m_port_position_strategy->GetNodeTreeLayout(), port_info);
  return std::make_unique<NodePortShape>(appearance, std::make_unique<NodePort>(port_info),
                                         nullptr);
}

void ConnectableShape::UpdateViewCoordinatesFromItem()
{
  setX(m_shape_adapter->GetX());
  setY(m_shape_adapter->GetY());
}

}  // namespace mvvm
