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

#include "node_editor_constants.h"

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
    return direction == PortDirection::kInput ? PortLocation::kBottom : PortLocation::kTop;
  }

  if (layout == NodeTreeLayout::kLeftToRight)
  {
    return direction == PortDirection::kInput ? PortLocation::kLeft : PortLocation::kRight;
  }

  return PortLocation::kUnknown;
}

PortAppearance GetDefaultPortAppearance(NodeTreeLayout layout, const PortInfo &port_info)
{
  const auto location = GetPortLocation(layout, port_info.GetPortDirection());
  return {location, QString::fromStdString(port_info.GetPortType()),
          constants::GetDefaultPortColor()};
}

}  // namespace mvvm
