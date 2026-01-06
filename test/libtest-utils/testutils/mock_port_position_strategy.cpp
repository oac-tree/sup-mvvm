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

#include "mock_port_position_strategy.h"

namespace mvvm::test
{
PortPositionStrategyDecorator::PortPositionStrategyDecorator(IPortPositionStrategy *decoratee)
    : m_decoratee(decoratee)
{
}

NodeTreeLayout PortPositionStrategyDecorator::GetNodeTreeLayout() const
{
  return m_decoratee->GetNodeTreeLayout();
}

QPointF PortPositionStrategyDecorator::GetPortPosition(PortDirection port_direction,
                                                       std::size_t port_index,
                                                       std::size_t total_ports,
                                                       const QRectF &shape_rect) const
{
  return m_decoratee->GetPortPosition(port_direction, port_index, total_ports, shape_rect);
}

}  // namespace mvvm::test
