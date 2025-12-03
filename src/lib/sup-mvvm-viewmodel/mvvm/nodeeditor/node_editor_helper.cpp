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

#include "node_editor_helper.h"

#include "i_node_port.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

#include <algorithm>

namespace mvvm
{

PortLocation GetPortLocation(NodeTreeLayout layout, PortDirection direction)
{
  if ((direction != PortDirection::kInput) && (direction != PortDirection::kOutput))
  {
    return PortLocation::kUnknown;
  }

  if (layout == NodeTreeLayout::kTopToBottom)
  {
    return (direction == PortDirection::kInput) ? PortLocation::kBottom : PortLocation::kTop;
  }

  if (layout == NodeTreeLayout::kLeftToRight)
  {
    return (direction == PortDirection::kInput) ? PortLocation::kLeft : PortLocation::kRight;
  }

  return PortLocation::kUnknown;
}

PortAppearance GetDefaultPortAppearance(NodeTreeLayout layout, const PortInfo& port_info)
{
  const auto location = GetPortLocation(layout, port_info.GetPortDirection());
  return {location, QString::fromStdString(port_info.GetPortLabel()), CreateDefaultNodePortStyle()};
}

std::vector<PortInfo> GetFilteredPortInfo(const std::vector<PortInfo>& port_infos,
                                          PortDirection direction)
{
  std::vector<PortInfo> result;
  (void)std::copy_if(port_infos.begin(), port_infos.end(), std::back_inserter(result),
                     [direction](const PortInfo& info)
                     { return info.GetPortDirection() == direction; });
  return result;
}

std::vector<INodePort*> GetPortsByDirection(const std::vector<INodePort*>& ports,
                                            PortDirection direction)
{
  std::vector<INodePort*> result;
  (void)std::copy_if(ports.begin(), ports.end(), std::back_inserter(result),
                     [direction](const INodePort* port)
                     { return port->GetPortDirection() == direction; });
  return result;
}

SessionItem* GetConnectableChild(const INodePort* port1, const INodePort* port2)
{
  if ((port1 == nullptr) || (port2 == nullptr))
  {
    return nullptr;
  }

  if (port1 == port2)
  {
    return nullptr;
  }

  if (!port1->IsConnectable(port2))
  {
    return nullptr;
  }

  return (port1->GetPortDirection() == PortDirection::kOutput) ? port1->GetPortInfo().GetOrigin()
                                                               : port2->GetPortInfo().GetOrigin();
}

SessionItem* GetConnectableParent(const INodePort* port1, const INodePort* port2)
{
  if ((port1 == nullptr) || (port2 == nullptr))
  {
    return nullptr;
  }

  if (port1 == port2)
  {
    return nullptr;
  }

  if (!port1->IsConnectable(port2))
  {
    return nullptr;
  }

  return (port1->GetPortDirection() == PortDirection::kInput) ? port1->GetPortInfo().GetOrigin()
                                                              : port2->GetPortInfo().GetOrigin();
}

bool ConnectPorts(const INodePort* port1, const INodePort* port2)
{
  auto parent = GetConnectableParent(port1, port2);
  auto child = GetConnectableChild(port1, port2);

  if ((parent != nullptr) && (child != nullptr))
  {
    if (auto model = parent->GetModel(); model)
    {
      model->MoveItem(child, parent, TagIndex::Append());
      return true;
    }
  }

  return false;
}

bool DisconnectPortsAndMove(const INodePort* port1, const INodePort* port2, SessionItem* new_parent)
{
  auto parent = GetConnectableParent(port1, port2);
  auto child = GetConnectableChild(port1, port2);
  if ((parent != nullptr) && (child != nullptr) && (new_parent != nullptr))
  {
    if (auto model = parent->GetModel(); model)
    {
      model->MoveItem(child, new_parent, TagIndex::Append());
      return true;
    }
  }
  return false;
}

SessionItem* GetPortOrigin(const INodePort* port)
{
  return (port != nullptr) ? port->GetPortInfo().GetOrigin() : nullptr;
}

}  // namespace mvvm
