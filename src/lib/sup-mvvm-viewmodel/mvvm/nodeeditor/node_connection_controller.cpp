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

#include "node_connection_controller_impl.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace mvvm
{

NodeConnectionController::NodeConnectionController(QGraphicsScene* scene)
    : QObject(scene)
    , m_controller_impl(std::make_unique<NodeConnectionControllerImpl>(CreateContext(), scene))
{
  scene->installEventFilter(this);
}

NodeConnectionController::~NodeConnectionController() = default;

bool NodeConnectionController::eventFilter(QObject* object, QEvent* event)
{
  EventProcessStatus status = EventProcessStatus::kStatusUnknown;

  if (auto mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(event); mouseEvent)
  {
    if (event->type() == QEvent::GraphicsSceneMousePress)
    {
      status = m_controller_impl->ProcessMousePress(mouseEvent->scenePos(), mouseEvent->button());
    }
    else if (event->type() == QEvent::GraphicsSceneMouseMove)
    {
      status = m_controller_impl->ProcessMouseMove(mouseEvent->scenePos());
    }
    else if (event->type() == QEvent::GraphicsSceneMouseRelease)
    {
      status = m_controller_impl->ProcessMouseRelease(mouseEvent->scenePos(), mouseEvent->button());
    }
  }

  if (status == EventProcessStatus::kEventWasHandled)
  {
    event->accept();
    return true;
  }

  return QObject::eventFilter(object, event);
}

NodeConnectionControllerContext NodeConnectionController::CreateContext()
{
  NodeConnectionControllerContext result;

  result.connection_request = [this](const INodePort* start_port, const INodePort* end_port)
  { emit connectionRequest(start_port, end_port); };
  result.connection_started = [this]() { emit connectionStarted(); };
  result.connection_finished = [this]() { emit connectionFinished(); };

  return result;
}
}  // namespace mvvm
