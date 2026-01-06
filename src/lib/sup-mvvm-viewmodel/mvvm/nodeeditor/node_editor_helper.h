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

#ifndef MVVM_NODEEDITOR_NODE_EDITOR_HELPER_H_
#define MVVM_NODEEDITOR_NODE_EDITOR_HELPER_H_

//! @file
//! Connection of helper functions for NodeEditor machinery.

#include <mvvm/nodeeditor/port_appearance.h>
#include <mvvm/nodeeditor/port_info.h>

#include <vector>

namespace mvvm
{

class INodePort;
class SessionItem;

/**
 * @brief Returns port location based on tree layout and port direction.
 */
PortLocation GetPortLocation(NodeTreeLayout layout, PortDirection direction);

/**
 * @brief Returns PortAppearance for the given port info and node tree layout.
 */
PortAppearance GetDefaultPortAppearance(NodeTreeLayout layout, const PortInfo& port_info);

/**
 * @brief Returns vector of PortInfo filtered according to the given direction.
 */
std::vector<PortInfo> GetFilteredPortInfo(const std::vector<PortInfo>& port_infos,
                                          PortDirection direction);

/**
 * @brief Returns ports filtered according to the given direction.
 */
std::vector<INodePort*> GetPortsByDirection(const std::vector<INodePort*>& ports,
                                            PortDirection direction);

/**
 * @brief Returns an item which can be connected as a child to another item.
 *
 * The method checks if ports are connectable and returns the item associated with
 * the output port (the child).
 */
SessionItem* GetConnectableChild(const INodePort* port1, const INodePort* port2);

/**
 * @brief Returns an item which can be connected as a parent to another item.
 *
 * The method checks if ports are connectable and returns the item associated with
 * the input port (the parent).
 */
SessionItem* GetConnectableParent(const INodePort* port1, const INodePort* port2);

/**
 * @brief Connects two ports by moving underlying items so one become child of another.
 */
bool ConnectPorts(const INodePort* port1, const INodePort* port2);

/**
 * @brief Disconnects two ports by moving the child item to a new parent.
 */
bool DisconnectPortsAndMove(const INodePort* port1, const INodePort* port2,
                            SessionItem* new_parent);

/**
 * @brief Helper to get underlying item from the node port.
 */
SessionItem* GetPortOrigin(const INodePort* port);

/**
 * @brief Return unnderlying item casted to specific type.
 */
template <typename ItemT>
ItemT* GetPortOrigin(const INodePort* port)
{
  return dynamic_cast<ItemT*>(GetPortOrigin(port));
}

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_EDITOR_HELPER_H_
