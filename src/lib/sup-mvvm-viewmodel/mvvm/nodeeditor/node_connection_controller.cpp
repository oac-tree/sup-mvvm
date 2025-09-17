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

#include "node_connection_controller.h"

#include "connectable_shape.h"
#include "node_connection.h"
#include "node_connection_shape.h"
#include "node_port.h"
#include "node_port_shape.h"
#include "port_info.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace mvvm
{

namespace
{

const bool kEventWasHandled = true;
const bool kEventWasIgnored = false;

std::unique_ptr<NodePort> CreateProxyPort(const QPointF& pos)
{
  const PortInfo port_info{PortDirection::kDefault, "temporary proxy"};
  auto result = std::make_unique<NodePort>(port_info);
  result->SetScenePosition(pos.x(), pos.y());
  return result;
}

}  // namespace

NodeConnectionController::NodeConnectionController(QGraphicsScene* scene) : m_scene(scene)
{
  setParent(m_scene);
  m_scene->installEventFilter(this);
}

NodeConnectionController::~NodeConnectionController() = default;

bool NodeConnectionController::eventFilter(QObject* object, QEvent* event)
{
  bool isProcessedEvent(false);

  if (auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event); mouseEvent)
  {
    if (event->type() == QEvent::GraphicsSceneMousePress)
    {
      isProcessedEvent = ProcessMousePress(mouseEvent);
    }
    else if (event->type() == QEvent::GraphicsSceneMouseMove)
    {
      isProcessedEvent = ProcessMouseMove(mouseEvent);
    }
    else if (event->type() == QEvent::GraphicsSceneMouseRelease)
    {
      isProcessedEvent = ProcessMouseRelease(mouseEvent);
    }
  }

  return isProcessedEvent ? kEventWasHandled : QObject::eventFilter(object, event);
}

NodePortShape* NodeConnectionController::FindNodePortShape(const QPointF& pos)
{
  QRectF area(0, 0, 4, 4);
  area.moveCenter(pos);

  for (auto item : m_scene->items(area))
  {
    if (auto port = dynamic_cast<NodePortShape*>(item); port)
    {
      return port;
    }
  }
  return nullptr;
}

bool NodeConnectionController::ProcessMousePress(QGraphicsSceneMouseEvent* event)
{
  if (!m_current_connection && event->button() == Qt::LeftButton)
  {
    if (auto port = FindNodePortShape(event->scenePos()); port)
    {
      emit connectionStarted();

      m_end_port_proxy = CreateProxyPort(event->scenePos());
      auto connection =
          std::make_unique<NodeConnection>(port->GetNodePort(), m_end_port_proxy.get());

      m_current_connection = std::make_unique<NodeConnectionShape>(NodeTreeLayout::kTopToBottom,
                                                                   std::move(connection));
      m_scene->addItem(m_current_connection.get());
      m_current_connection->UpdatePath();
      return kEventWasHandled;
    }
  }
  return kEventWasIgnored;
}

bool NodeConnectionController::ProcessMouseMove(QGraphicsSceneMouseEvent* event)
{
  if (m_current_connection)
  {
    m_end_port_proxy->SetScenePosition(event->scenePos().x(), event->scenePos().y());
    m_current_connection->UpdatePath();
    return kEventWasHandled;
  }
  return kEventWasIgnored;
}

bool NodeConnectionController::ProcessMouseRelease(QGraphicsSceneMouseEvent* event)
{
  if (m_current_connection && event->button() == Qt::LeftButton)
  {
    emit connectionFinished();
    if (auto port_shape = FindNodePortShape(event->scenePos()); port_shape)
    {
      auto start_port = m_current_connection->GetNodeConnection()->GetStartPort();
      auto end_port = port_shape->GetNodePort();
      if (start_port->IsConnectable(end_port))
      {
        emit connectionRequest(start_port, end_port);
      }
    }
    // At this point we do not need connection object anymore.
    // It will be redrawn automatically, when the model process our request.
    m_current_connection.reset();
    return kEventWasHandled;
  }
  return kEventWasIgnored;
}

}  // namespace mvvm
