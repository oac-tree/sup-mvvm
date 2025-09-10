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

#ifndef MVVM_NODEEDITOR_NODE_EDITOR_HELPER_H_
#define MVVM_NODEEDITOR_NODE_EDITOR_HELPER_H_

//! @file
//! Connection of helper functions for NodeEditor machinery.

#include <mvvm/nodeeditor/port_appearance.h>
#include <mvvm/nodeeditor/port_info.h>

#include <vector>

namespace mvvm
{

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
std::vector<PortInfo> GetFilteredPortInfo(const std::vector<PortInfo>& port_infos, PortDirection direction);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NODE_EDITOR_HELPER_H_
