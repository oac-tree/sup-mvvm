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

#include "mvvm/nodeeditor/node_connection.h"

#include <mvvm/nodeeditor/node_port.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodeConnection class.
 */
class NodeConnectionTest : public ::testing::Test
{
};

TEST_F(NodeConnectionTest, InitialState)
{
  NodeConnection connection;

  EXPECT_EQ(connection.GetStartPort(), nullptr);
  EXPECT_EQ(connection.GetEndPort(), nullptr);
}

TEST_F(NodeConnectionTest, SetPorts)
{
  const NodePort start_node_port({PortDirection::kOutput, "abc", "deg"});
  const NodePort end_node_port({PortDirection::kInput, "abc", "deg"});

  NodeConnection connection;
  connection.SetStartPort(&start_node_port);
  connection.SetEndPort(&end_node_port);
  EXPECT_EQ(connection.GetStartPort(), &start_node_port);
  EXPECT_EQ(connection.GetEndPort(), &end_node_port);
}

}  // namespace mvvm::test
