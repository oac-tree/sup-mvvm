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

#include "graphics_scene_helper.h"

#include "connectable_shape.h"
#include "i_connectable_shape_adapter.h"
#include "node_connection_shape.h"
#include "node_editor_helper.h"

#include <mvvm/model/session_item.h>

namespace mvvm
{

SessionItem *GetUnderlyingItem(ConnectableShape *shape)
{
  return shape->GetConnectableShapeAdapter()->GetItem();
}

INodePort *GetInputPort(const ConnectableShape &shape, std::size_t index)
{
  auto ports = GetPortsByDirection(shape.GetPorts(), PortDirection::kInput);
  return index >= ports.size() ? nullptr : ports.at(index);
}

INodePort *GetOutputPort(const ConnectableShape &shape, std::size_t index)
{
  auto ports = GetPortsByDirection(shape.GetPorts(), PortDirection::kOutput);
  return index >= ports.size() ? nullptr : ports.at(index);
}

bool IsParentChildConnection(ConnectableShape *parent, ConnectableShape *child,
                             NodeConnectionShape *connection_shape)
{
  if (!connection_shape || !parent || !child)
  {
    return false;
  }

  auto child_output_port = GetOutputPort(*child);
  auto parent_input_port = GetInputPort(*parent);
  auto connection = connection_shape->GetNodeConnection();

  return connection->GetStartPort() == child_output_port
         && connection->GetEndPort() == parent_input_port;
}

}  // namespace mvvm
