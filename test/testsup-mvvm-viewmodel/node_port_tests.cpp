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

#include "mvvm/nodeeditor/node_port.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/nodeeditor/node_connection.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodePort class.
 */
class NodePortTest : public ::testing::Test
{
};

TEST_F(NodePortTest, InitialState)
{
  const PortInfo port_info{PortDirection::kDefault, "abc", "deg"};

  const NodePort node_port(port_info);

  EXPECT_EQ(node_port.GetPortInfo(), port_info);
  EXPECT_TRUE(node_port.GetConnections().empty());
}

TEST_F(NodePortTest, AppendRemoveConnection)
{
  NodeConnection connection;

  NodePort node_port({PortDirection::kDefault, "abc", "deg"});
  EXPECT_TRUE(node_port.GetConnections().empty());
  node_port.AppendConnection(&connection);
  EXPECT_EQ(node_port.GetConnections().size(), 1);
  EXPECT_EQ(node_port.GetConnections().front(), &connection);
  node_port.RemoveConnection(&connection);
  EXPECT_TRUE(node_port.GetConnections().empty());

  EXPECT_THROW(node_port.RemoveConnection(&connection), RuntimeException)
      << "Removing non-existing connection should throw";

  node_port.AppendConnection(&connection);
  EXPECT_THROW(node_port.AppendConnection(&connection), RuntimeException)
      << "Appending the same connection twice should throw";
}

TEST_F(NodePortTest, AppendThreeConnections)
{
  NodeConnection connection1;
  NodeConnection connection2;
  NodeConnection connection3;

  NodePort node_port({PortDirection::kDefault, "abc", "deg"});
  node_port.AppendConnection(&connection1);
  node_port.AppendConnection(&connection2);
  node_port.AppendConnection(&connection3);

  const std::vector<const INodeConnection*> expected{&connection1, &connection2, &connection3};
  EXPECT_EQ(node_port.GetConnections(), expected);

  node_port.RemoveConnection(&connection2);
  const std::vector<const INodeConnection*> expected_after_removal{&connection1, &connection3};
  EXPECT_EQ(node_port.GetConnections(), expected_after_removal);
}

}  // namespace mvvm::test
