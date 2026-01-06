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

#include "mvvm/standarditems/node_item.h"

#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodeItem class.
 */
class NodeItemTests : public ::testing::Test
{
};

TEST_F(NodeItemTests, InitialState)
{
  const NodeItem item;

  EXPECT_EQ(item_traits<NodeItem>::type_name(), "NodeItem");
  EXPECT_EQ(item_traits<NodeItem>::type_name(), item.GetType());
  EXPECT_EQ(item.GetType(), item.GetDisplayName());
  EXPECT_TRUE(test::IsCloneImplemented<NodeItem>());

  EXPECT_EQ(item.GetX(), 0.0);
  EXPECT_EQ(item.GetY(), 0.0);
  EXPECT_EQ(item.GetBaseColor(), std::string());
}

TEST_F(NodeItemTests, GetAndSet)
{
  NodeItem item;
  item.SetX(42.0);
  EXPECT_EQ(item.GetX(), 42.0);

  item.SetY(43.0);
  EXPECT_EQ(item.GetY(), 43.0);

  item.SetBaseColor("abc");
  EXPECT_EQ(item.GetBaseColor(), "abc");
}

}  // namespace mvvm::test
