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

#ifndef MVVM_NODEEDITOR_I_PORT_POSITION_STRATEGY_H_
#define MVVM_NODEEDITOR_I_PORT_POSITION_STRATEGY_H_

#include <mvvm/nodeeditor/node_editor_types.h>

#include <QPointF>
#include <QRectF>

namespace mvvm
{

/**
 * @brief The IPortPositionStrategy class calculates the position of port on one of the side of
 * connectable item.
 */
class IPortPositionStrategy
{
public:
  IPortPositionStrategy() = default;
  virtual ~IPortPositionStrategy() = default;

  IPortPositionStrategy(const IPortPositionStrategy&) = delete;
  IPortPositionStrategy& operator=(const IPortPositionStrategy&) = delete;
  IPortPositionStrategy(IPortPositionStrategy&&) = delete;
  IPortPositionStrategy& operator=(IPortPositionStrategy&&) = delete;

  /**
   * @brief Returns the position of the port on the shape rectangle.
   *
   * @param port_direction The port's direction (input/output).
   * @param port_index Index of the port among all ports on the same side.
   * @param total_ports Total number of ports on the same side.
   * @param shape_rect The rectangle of the shape where ports are placed.
   * @return The position of the port in the shape's coordinate system.
   */
  virtual QPointF GetPortPosition(PortDirection port_direction, std::size_t port_index,
                                  std::size_t total_ports, const QRectF& shape_rect) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_PORT_POSITION_STRATEGY_H_
