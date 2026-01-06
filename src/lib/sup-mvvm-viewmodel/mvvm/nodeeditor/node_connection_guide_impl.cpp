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

#include "node_connection_guide_impl.h"

#include "graphics_scene_helper.h"
#include "node_connection.h"
#include "node_connection_shape.h"
#include "node_editor_constants.h"
#include "node_port.h"
#include "node_port_shape.h"

#include <QGraphicsScene>

namespace mvvm
{

namespace
{

std::unique_ptr<NodePort> CreateProxyPort(const QPointF& pos)
{
  const PortInfo port_info{PortDirection::kDefault, "temporary proxy"};
  auto result = std::make_unique<NodePort>(port_info);
  result->SetScenePosition(pos.x(), pos.y());
  return result;
}

}  // namespace

NodeConnectionGuideImpl::NodeConnectionGuideImpl(const NodeConnectionGuideContext& context,
                                                 QGraphicsScene* scene)
    : m_scene(scene), m_context(context)
{
}

NodeConnectionGuideImpl::~NodeConnectionGuideImpl() = default;

NodePortShape* NodeConnectionGuideImpl::FindNodePortShape(const QPointF& scene_pos)
{
  const double size = 2 * constants::GetDefaultPortRadius();
  QRectF area(0, 0, size, size);
  area.moveCenter(scene_pos);

  auto ports = GetShapes<NodePortShape>(*m_scene, area);
  return ports.empty() ? nullptr : ports.front();
}

EventProcessStatus NodeConnectionGuideImpl::ProcessMousePress(const QPointF& scene_pos,
                                                              Qt::MouseButton button)
{
  if (!m_current_connection && (button == Qt::LeftButton))
  {
    if (auto port_shape = FindNodePortShape(scene_pos); port_shape)
    {
      m_context.connection_started();

      m_end_port_proxy = CreateProxyPort(scene_pos);
      auto connection =
          std::make_unique<NodeConnection>(port_shape->GetNodePort(), m_end_port_proxy.get());

      m_current_connection = std::make_unique<NodeConnectionShape>(NodeTreeLayout::kTopToBottom,
                                                                   std::move(connection));
      m_scene->addItem(m_current_connection.get());
      m_current_connection->UpdatePath({});
      return EventProcessStatus::kEventWasHandled;
    }
  }
  return EventProcessStatus::kEventWasIgnored;
}

EventProcessStatus NodeConnectionGuideImpl::ProcessMouseMove(const QPointF& scene_pos)
{
  if (m_current_connection)
  {
    m_end_port_proxy->SetScenePosition(scene_pos.x(), scene_pos.y());
    m_current_connection->UpdatePath({});
    return EventProcessStatus::kEventWasHandled;
  }
  return EventProcessStatus::kEventWasIgnored;
}

EventProcessStatus NodeConnectionGuideImpl::ProcessMouseRelease(const QPointF& scene_pos,
                                                                Qt::MouseButton button)
{
  if (m_current_connection && (button == Qt::LeftButton))
  {
    m_context.connection_finished();
    if (auto port_shape = FindNodePortShape(scene_pos); port_shape)
    {
      auto start_port = m_current_connection->GetNodeConnection()->GetStartPort();
      auto end_port = port_shape->GetNodePort();
      if (start_port->IsConnectable(end_port))
      {
        // Position of the proxy port is not important anymore. Connections should be reset before
        // sending a signal
        ResetCurrentConnection();
        m_context.connection_request(start_port, end_port);
      }
    }
    // At this point we do not need connection object anymore.
    // It will be redrawn automatically, when the model process our request.
    ResetCurrentConnection();
    return EventProcessStatus::kEventWasHandled;
  }
  return EventProcessStatus::kEventWasIgnored;
}

NodeConnectionShape* NodeConnectionGuideImpl::GetCurrentConnection() const
{
  return m_current_connection.get();
}

INodePort* NodeConnectionGuideImpl::GetEndPortProxy() const
{
  return m_end_port_proxy.get();
}

void NodeConnectionGuideImpl::ResetCurrentConnection()
{
  m_current_connection.reset();
  m_end_port_proxy.reset();
}

}  // namespace mvvm
