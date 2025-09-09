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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_PORT_POSITION_STRATEGY_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_PORT_POSITION_STRATEGY_H_

#include <mvvm/nodeeditor/i_port_position_strategy.h>

#include <gmock/gmock.h>

namespace mvvm::test
{

/**
 * The MockPortPositionStrategy class validates calls to IPortPositionStrategy.
 */
class MockPortPositionStrategy : public IPortPositionStrategy
{
public:
  MOCK_METHOD(NodeTreeLayout, GetNodeTreeLayout, (), (const, override));
  MOCK_METHOD(QPointF, GetPortPosition,
              (PortDirection port_direction, std::size_t port_index, std::size_t total_ports,
               const QRectF& shape_rect),
              (const, override));
};

/**
 * @brief The PortPositionStrategyDecorator class is a simple forward decorator to an
 * IPortPositionStrategy.
 */
class PortPositionStrategyDecorator : public IPortPositionStrategy
{
public:
  explicit PortPositionStrategyDecorator(IPortPositionStrategy* decoratee);

  NodeTreeLayout GetNodeTreeLayout() const override;
  QPointF GetPortPosition(PortDirection port_direction, std::size_t port_index,
                          std::size_t total_ports, const QRectF& shape_rect) const override;

private:
  IPortPositionStrategy* m_decoratee{nullptr};
};

}  // namespace mvvm::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_PORT_POSITION_STRATEGY_H_
