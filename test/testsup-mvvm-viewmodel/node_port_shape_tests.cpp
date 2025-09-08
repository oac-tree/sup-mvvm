/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/nodeeditor/node_port_shape.h"

#include <gtest/gtest.h>
#include <testutils/mock_node_port.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodePortShape class.
 */
class NodePortShapeTest : public ::testing::Test
{
public:
  std::unique_ptr<NodePortShape> CreateNodePortShape(PortAppearance port_appearance)
  {
    auto node_port = std::make_unique<NodePortDecorator>(&m_mock_node_port);
    return std::make_unique<NodePortShape>(port_appearance, std::move(node_port), nullptr);
  }

  ::testing::StrictMock<MockNodePort> m_mock_node_port;
};

TEST_F(NodePortShapeTest, InitialState)
{
  const PortAppearance port_appearance(PortLocation::kLeft, "TestPort", QColor(Qt::red));
  auto shape = CreateNodePortShape(port_appearance);

  EXPECT_EQ(shape->boundingRect(), port_appearance.GetBoundingRect());
}

}  // namespace mvvm::test
