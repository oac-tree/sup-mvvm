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

#include <mvvm/nodeeditor/node_editor_constants.h>

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
}

TEST_F(NodePortShapeTest, BoundingBoxForEmptyLabel)
{
  const PortAppearance port_appearance(PortLocation::kLeft, "", QColor(Qt::red));
  auto shape = CreateNodePortShape(port_appearance);
  const QRectF bounding_rect = shape->boundingRect();
  const double expected_size = 2.0 * (port_appearance.GetPortRadius() + 1.0);
  EXPECT_DOUBLE_EQ(bounding_rect.width(), expected_size);
  EXPECT_DOUBLE_EQ(bounding_rect.height(), expected_size);
}

TEST_F(NodePortShapeTest, GetLabelRectForTopLocation)
{
  const QPointF port_position{0.0, 0.0};
  const QString label_text("abc");
  const QRectF label_rect = GetLabelRect(PortLocation::kTop, port_position, label_text);
  EXPECT_LT(label_rect.top(), port_position.y());
  EXPECT_GT(label_rect.bottom(), port_position.y() - (4.0 * constants::GetDefaultPortRadius()));
  EXPECT_LT(label_rect.left(), port_position.x());
  EXPECT_GT(label_rect.right(), port_position.x());
}

}  // namespace mvvm::test
