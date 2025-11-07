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

#include "top_bottom_port_position_strategy.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace mvvm
{

namespace
{

/**
 * @brief Returns evenly distributed coordinate.
 *
 * Single point will be at the center, two oints will be at 1/3 and 2/3, etc.
 */
double GetUniformPos(std::size_t port_index, std::size_t total_ports, double length)
{
  const double step = length / static_cast<double>(total_ports + 1);
  return static_cast<double>(port_index + 1) * step;
}

QPointF GetBottomLocation(std::size_t port_index, std::size_t total_ports, const QRectF &shape_rect)
{
  const double x_pos =
      shape_rect.left() + GetUniformPos(port_index, total_ports, shape_rect.width());
  const double y_pos = shape_rect.bottom();
  return {x_pos, y_pos};
}

QPointF GetTopLocation(std::size_t port_index, std::size_t total_ports, const QRectF &shape_rect)
{
  const double x_pos =
      shape_rect.left() + GetUniformPos(port_index, total_ports, shape_rect.width());
  const double y_pos = shape_rect.top();
  return {x_pos, y_pos};
}

}  // namespace

NodeTreeLayout TopBottomPortPositionStrategy::GetNodeTreeLayout() const
{
  return NodeTreeLayout::kTopToBottom;
}

QPointF TopBottomPortPositionStrategy::GetPortPosition(PortDirection port_direction,
                                                       std::size_t port_index,
                                                       std::size_t total_ports,
                                                       const QRectF &shape_rect) const
{
  if ((total_ports == 0) || (port_index >= total_ports))
  {
    throw RuntimeException("Wrong port number");
  }

  if (port_direction == PortDirection::kInput)
  {
    return GetBottomLocation(port_index, total_ports, shape_rect);
  }

  if (port_direction == PortDirection::kOutput)
  {
    return GetTopLocation(port_index, total_ports, shape_rect);
  }

  return {};
}

}  // namespace mvvm
