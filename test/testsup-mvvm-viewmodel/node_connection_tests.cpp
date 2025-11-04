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

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/nodeeditor/node_port.h>

#include <gmock/gmock.h>
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
  const NodeConnection connection;

  EXPECT_EQ(connection.GetStartPort(), nullptr);
  EXPECT_EQ(connection.GetEndPort(), nullptr);
  EXPECT_FALSE(connection.HasPort(nullptr));
}

TEST_F(NodeConnectionTest, SetPorts)
{
  NodePort start_port({PortDirection::kOutput, "abc"});
  NodePort end_port({PortDirection::kInput, "abc"});

  NodeConnection connection;
  connection.SetStartPort(&start_port);
  connection.SetEndPort(&end_port);
  EXPECT_EQ(connection.GetStartPort(), &start_port);
  EXPECT_EQ(connection.GetEndPort(), &end_port);
  EXPECT_TRUE(connection.HasPort(&start_port));
  EXPECT_TRUE(connection.HasPort(&end_port));
  EXPECT_FALSE(connection.HasPort(nullptr));
}

TEST_F(NodeConnectionTest, SetPortFailure)
{
  NodePort start_port({PortDirection::kOutput, "abc"});
  NodePort end_port({PortDirection::kInput, "abc"});

  NodeConnection connection;
  connection.SetStartPort(&start_port);
  EXPECT_THROW(connection.SetStartPort(&start_port), RuntimeException)
      << "Setting start port twice should throw";

  connection.SetEndPort(&end_port);
  EXPECT_THROW(connection.SetEndPort(&end_port), RuntimeException)
      << "Setting end port twice should throw";
}

TEST_F(NodeConnectionTest, TwoPortsConstruction)
{
  NodePort start_port({PortDirection::kOutput, "abc"});
  NodePort end_port({PortDirection::kInput, "abc"});

  const NodeConnection connection(&start_port, &end_port);
  EXPECT_EQ(connection.GetStartPort(), &start_port);
  EXPECT_EQ(connection.GetEndPort(), &end_port);
}

TEST_F(NodeConnectionTest, DisconnectionOnConnectionDestruction)
{
  NodePort start_port({PortDirection::kOutput, "abc"});
  NodePort end_port({PortDirection::kInput, "abc"});

  {
    const NodeConnection connection(&start_port, &end_port);
  }
  EXPECT_TRUE(start_port.GetConnections().empty());
  EXPECT_TRUE(end_port.GetConnections().empty());
}

TEST_F(NodeConnectionTest, ForgetPort)
{
  NodePort start_port({PortDirection::kOutput, "abc"});
  NodePort end_port({PortDirection::kInput, "abc"});

  NodeConnection connection(&start_port, &end_port);
  EXPECT_TRUE(connection.HasPort(&start_port));
  EXPECT_TRUE(connection.HasPort(&end_port));

  connection.ForgetPort(&start_port);
  EXPECT_FALSE(connection.HasPort(&start_port));
  EXPECT_TRUE(connection.HasPort(&end_port));
  start_port.RemoveConnection(&connection);  // to avoid destruction issues

  connection.ForgetPort(&end_port);
  EXPECT_FALSE(connection.HasPort(&start_port));
  EXPECT_FALSE(connection.HasPort(&end_port));
  end_port.RemoveConnection(&connection);  // to avoid destruction issues
}

TEST_F(NodeConnectionTest, NotifyShapeCallback)
{
  ::testing::MockFunction<void()> mock_callback;

  NodeConnection connection;
  connection.SetNotifyShapeCallback(mock_callback.AsStdFunction());
  EXPECT_CALL(mock_callback, Call()).Times(1);
  connection.UpdateConnectionShape();
}

TEST_F(NodeConnectionTest, NotifyShapeViaPorts)
{
  ::testing::MockFunction<void()> mock_callback;

  NodePort start_port({PortDirection::kOutput, "abc"});
  NodePort end_port({PortDirection::kInput, "abc"});

  NodeConnection connection(&start_port, &end_port);
  connection.SetNotifyShapeCallback(mock_callback.AsStdFunction());

  EXPECT_CALL(mock_callback, Call()).Times(1);
  start_port.SetScenePosition(1.0, 2.0);
}

}  // namespace mvvm::test
