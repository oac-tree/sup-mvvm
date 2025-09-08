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

#ifndef MVVM_NODEEDITOR_PORT_POSITION_STRATEGY_H_
#define MVVM_NODEEDITOR_PORT_POSITION_STRATEGY_H_

#include <mvvm/nodeeditor/i_port_position_strategy.h>

namespace mvvm
{

/**
 * @brief The TopBottomPortPositionStrategy i.
 */
class TopBottomPortPositionStrategy : public IPortPositionStrategy
{
public:
  TopBottomPortPositionStrategy() = default;
  ~TopBottomPortPositionStrategy() override = default;

  TopBottomPortPositionStrategy(const TopBottomPortPositionStrategy&) = delete;
  TopBottomPortPositionStrategy& operator=(const TopBottomPortPositionStrategy&) = delete;
  TopBottomPortPositionStrategy(TopBottomPortPositionStrategy&&) = delete;
  TopBottomPortPositionStrategy& operator=(TopBottomPortPositionStrategy&&) = delete;

  QPointF GetPortPosition(PortDirection port_direction, std::size_t port_index,
                          std::size_t total_ports, const QRectF& shape_rect) const override;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_POSITION_STRATEGY_H_
