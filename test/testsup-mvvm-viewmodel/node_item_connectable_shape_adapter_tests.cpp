/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/nodeeditor/node_item_connectable_shape_adapter.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/standarditems/node_item.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

namespace mvvm::test
{

/**
 * @brief Tests NodeItemConnectableShapeAdapter class.
 */
class NodeItemConnectableShapeAdapterTest : public ::testing::Test
{
public:
};

TEST_F(NodeItemConnectableShapeAdapterTest, InitialState)
{
  EXPECT_THROW(NodeItemConnectableShapeAdapter(nullptr), RuntimeException);

  NodeItem node_item;
  node_item.SetDisplayName("abc");
  node_item.SetBaseColor("red");
  node_item.SetX(42.0);
  node_item.SetY(43.0);

  const NodeItemConnectableShapeAdapter adapter(&node_item);
  EXPECT_EQ(adapter.GetItem(), &node_item);
  EXPECT_EQ(adapter.GetDisplayName(), "abc");
  EXPECT_EQ(adapter.GetX(), 42.0);
  EXPECT_EQ(adapter.GetY(), 43.0);

  ASSERT_EQ(adapter.GetPortInfos().size(), 1U);
  EXPECT_EQ(adapter.GetPortInfos().at(0).GetPortDirection(), PortDirection::kOutput);
  EXPECT_EQ(adapter.GetPortInfos().at(0).GetPortDirection(), PortDirection::kOutput);
  EXPECT_TRUE(adapter.GetOperationStates().None());

  auto style = adapter.GetStyle();
  EXPECT_EQ(style.base_color, QColor(Qt::red));
}

TEST_F(NodeItemConnectableShapeAdapterTest, PortInfoOfSequenceItem)
{
  test::toyitems::SequenceItem node_item;
  const NodeItemConnectableShapeAdapter adapter(&node_item);

  ASSERT_EQ(adapter.GetPortInfos().size(), 2U);
  EXPECT_EQ(adapter.GetPortInfos().at(0).GetPortDirection(), PortDirection::kInput);
  EXPECT_EQ(adapter.GetPortInfos().at(1).GetPortDirection(), PortDirection::kOutput);
}

TEST_F(NodeItemConnectableShapeAdapterTest, SetCoordinates)
{
  NodeItem node_item;
  EXPECT_EQ(node_item.GetX(), 0.0);
  EXPECT_EQ(node_item.GetY(), 0.0);

  NodeItemConnectableShapeAdapter adapter(&node_item);
  adapter.SetX(42.0);
  adapter.SetY(43.0);
  EXPECT_EQ(node_item.GetX(), 42.0);
  EXPECT_EQ(node_item.GetY(), 43.0);
}

}  // namespace mvvm::test
