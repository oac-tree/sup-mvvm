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
#include "node_connection.h"
#include "node_connection_shape.h"
#include "node_editor_constants.h"
#include "node_editor_helper.h"
#include "node_port.h"
#include "node_port_shape.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/signals/item_listener.h>

#include <QGraphicsScene>
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
  const QRectF result(rect.x(), rect.y(), rect.width(), rect.height() / 2);

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
  QFont result("Noto Sans", constants::NodeUnitSize(0.8), QFont::Normal);
  result.setWordSpacing(-constants::NodeUnitSize(0.25));
  return result;
}

/**
 * @brief Returns first input port or nullptr if no such port.
 */
INodePort *GetInputPort(const std::vector<INodePort *>& all_ports)
{
  auto input_ports = GetPortsByDirection(all_ports, PortDirection::kInput);
  return input_ports.empty() ? nullptr : input_ports.front();
}

/**
 * @brief Returns first output port or nullptr if no such port.
 */
INodePort *GetOutputPort(const std::vector<INodePort *>& all_ports)
{
  auto output_ports = GetPortsByDirection(all_ports, PortDirection::kOutput);
  return output_ports.empty() ? nullptr : output_ports.front();
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
  SetupItemPropertyListener();
  UpdateViewFromItem();
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
  painter->save();

  painter->setRenderHint(QPainter::Antialiasing);

  painter->setPen(Qt::gray);
  if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    painter->setPen(Qt::DashLine);
  }

  painter->setBrush(
      constants::ConnectableShapeGradient(m_shape_adapter->GetBaseColor(), GetShapeRect()));
  painter->drawRoundedRect(GetShapeRect(), kRoundPar, kRoundPar);

  // plotting label
  static const QFont label_font = GetLabelFont();
  static const QRectF label_rect = GetLabelRectangle(GetShapeRect());
  painter->setFont(label_font);
  painter->setPen(Qt::black);
  QTextOption text_option(Qt::AlignHCenter | Qt::AlignTop);
  text_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  painter->drawText(label_rect, m_shape_adapter->GetDisplayName(), text_option);

  painter->restore();
}

QRectF ConnectableShape::GetShapeRect()
{
  return constants::DefaultConnectableShapeRect();
}

void ConnectableShape::UpdateItemFromView()
{
  if (m_block_item_update)
  {
    return;
  }

  m_block_view_update = true;
  m_shape_adapter->SetX(x());
  m_shape_adapter->SetY(y());
  m_block_view_update = false;
}

void ConnectableShape::UpdateViewFromItem()
{
  if (m_block_view_update)
  {
    return;
  }

  m_block_item_update = true;
  setX(m_shape_adapter->GetX());
  setY(m_shape_adapter->GetY());
  update();  // will pick-up color too through GetBaseColor() call in paint() method
  m_block_item_update = false;
}

void ConnectableShape::MakeChildConnected(ConnectableShape *child_shape)
{
  if (!scene())
  {
    throw RuntimeException("Shape is not in the scene.");
  }

  if (child_shape == nullptr)
  {
    throw RuntimeException("Child shape is null");
  }

  if (child_shape == this)
  {
    throw RuntimeException("Cannot connect shape to itself");
  }

  INodePort *input_port = GetInputPort(m_node_ports);
  INodePort *output_port = GetOutputPort(child_shape->m_node_ports);

  if ((input_port == nullptr) || (output_port == nullptr))
  {
    throw RuntimeException("Cannot connect shapes without ports");
  }

  if (!input_port->IsConnectable(output_port))
  {
    throw RuntimeException("Ports are not connectable");
  }
  auto connection = std::make_unique<NodeConnection>(output_port, input_port);

  auto connection_snape = std::make_unique<NodeConnectionShape>(std::move(connection), nullptr);
  scene()->addItem(connection_snape.release());  // take ownership
}

std::vector<INodePort *> ConnectableShape::GetPorts() const
{
  return m_node_ports;
}

IConnectableShapeAdapter *ConnectableShape::GetConnectableShapeAdapter()
{
  return m_shape_adapter.get();
}

QVariant ConnectableShape::itemChange(GraphicsItemChange change, const QVariant &value)
{
  if (change == ItemScenePositionHasChanged)
  {
    UpdateItemFromView();
  }
  return value;
}

void ConnectableShape::CalculateBoundingBox()
{
  // Adding margins for a pen. Currently we have some artifacts in the image, despite of this
  // attempt to give extra space. May be it is only plasma/wayland issue.
  m_bounding_box = GetShapeRect().marginsAdded(QMarginsF(2.0, 2.0, 2.0, 2.0));
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
  auto same_direction_ports = GetFilteredPortInfo(port_infos, port_direction);

  std::size_t index{0};
  for (const auto &port_info : same_direction_ports)
  {
    auto port_shape = CreateNodePortShape(port_info);
    auto port_shape_ptr = port_shape.get();
    const auto position = m_port_position_strategy->GetPortPosition(
        port_direction, index, same_direction_ports.size(), GetShapeRect());
    port_shape.release()->setParentItem(this);  // take ownership

    port_shape_ptr->setPos(position);

    ++index;
  }
}

void ConnectableShape::SetupItemPropertyListener()
{
  if (utils::HasNotifications(m_shape_adapter->GetItem()))
  {
    m_item_listener = std::make_unique<ItemListener>(m_shape_adapter->GetItem());
    m_item_listener->Connect<PropertyChangedEvent>([this](auto) { UpdateViewFromItem(); });
  }
}

std::unique_ptr<QGraphicsItem> ConnectableShape::CreateNodePortShape(const PortInfo &port_info)
{
  const auto appearance =
      GetDefaultPortAppearance(m_port_position_strategy->GetNodeTreeLayout(), port_info);
  auto node_port = std::make_unique<NodePort>(port_info);
  m_node_ports.push_back(node_port.get());
  return std::make_unique<NodePortShape>(appearance, std::move(node_port), nullptr);
}

}  // namespace mvvm
