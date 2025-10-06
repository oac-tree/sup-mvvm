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

#include <gmock/gmock.h>
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
  const PortInfo port_info{PortDirection::kDefault, "type_abc"};

  const NodePort node_port(port_info);

  EXPECT_EQ(node_port.GetPortDirection(), PortDirection::kDefault);
  EXPECT_EQ(node_port.GetPortType(), std::string("type_abc"));

  EXPECT_EQ(node_port.GetPortInfo(), port_info);
  EXPECT_TRUE(node_port.GetConnections().empty());

  EXPECT_FALSE(node_port.IsConnected(nullptr));
  EXPECT_FALSE(node_port.IsConnectable(nullptr));
}

TEST_F(NodePortTest, AppendRemoveConnection)
{
  NodeConnection connection;

  NodePort node_port({PortDirection::kDefault, "type_abc"});
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

  NodePort node_port({PortDirection::kDefault, "type_abc"});
  node_port.AppendConnection(&connection1);
  node_port.AppendConnection(&connection2);
  node_port.AppendConnection(&connection3);

  const std::vector<const INodeConnection*> expected{&connection1, &connection2, &connection3};
  EXPECT_EQ(node_port.GetConnections(), expected);

  node_port.RemoveConnection(&connection2);
  const std::vector<const INodeConnection*> expected_after_removal{&connection1, &connection3};
  EXPECT_EQ(node_port.GetConnections(), expected_after_removal);
}

TEST_F(NodePortTest, IsConnected)
{
  NodePort port1({PortDirection::kInput, "type_abc"});
  NodePort port2({PortDirection::kOutput, "type_abc"});
  NodePort port3({PortDirection::kOutput, "type_abc"});

  NodeConnection connection1;
  connection1.SetStartPort(&port1);
  connection1.SetEndPort(&port2);
  EXPECT_TRUE(port1.IsConnected(&port2));
  EXPECT_TRUE(port2.IsConnected(&port1));
  EXPECT_FALSE(port1.IsConnected(&port3));
  EXPECT_FALSE(port2.IsConnected(&port3));
  EXPECT_FALSE(port1.IsConnected(nullptr));
  EXPECT_FALSE(port2.IsConnected(nullptr));
  EXPECT_FALSE(port3.IsConnected(nullptr));
}

TEST_F(NodePortTest, IsConnectable)
{
  {
    NodePort port1({PortDirection::kInput, "type_abc"});
    NodePort port2({PortDirection::kOutput, "type_abc"});
    EXPECT_TRUE(port1.IsConnectable(&port2));
    EXPECT_TRUE(port2.IsConnectable(&port1));
  }

  {
    NodePort port1({PortDirection::kOutput, "type_abc"});
    NodePort port2({PortDirection::kInput, "type_abc"});
    EXPECT_TRUE(port1.IsConnectable(&port2));
    EXPECT_TRUE(port2.IsConnectable(&port1));
  }

  {
    NodePort port1({PortDirection::kInput, "type_abc"});
    NodePort port2({PortDirection::kOutput, "type_xyz"});
    EXPECT_FALSE(port1.IsConnectable(&port2));
    EXPECT_FALSE(port2.IsConnectable(&port1));
  }

  {
    NodePort port1({PortDirection::kDefault, "type_abc"});
    NodePort port2({PortDirection::kDefault, "type_abc"});
    EXPECT_FALSE(port1.IsConnectable(&port2));
    EXPECT_FALSE(port2.IsConnectable(&port1));
  }

  {
    NodePort port1({PortDirection::kInput, "type_abc"});
    NodePort port2({PortDirection::kInput, "type_abc"});
    EXPECT_FALSE(port1.IsConnectable(&port2));
    EXPECT_FALSE(port2.IsConnectable(&port1));
  }

  {
    NodePort port1({PortDirection::kOutput, "type_abc"});
    NodePort port2({PortDirection::kOutput, "type_abc"});
    EXPECT_FALSE(port1.IsConnectable(&port2));
    EXPECT_FALSE(port2.IsConnectable(&port1));
  }
}

TEST_F(NodePortTest, NodePortsDeletedBeforeConnection)
{
  NodeConnection connection;

  {
    NodePort port1({PortDirection::kInput, "type_abc"});
    NodePort port2({PortDirection::kOutput, "type_abc"});
    connection.SetStartPort(&port1);
    connection.SetEndPort(&port2);
  }
  // Ports are deleted, connection should not try to remove itself from ports
  // in its destructor.
  EXPECT_EQ(connection.GetStartPort(), nullptr);
  EXPECT_EQ(connection.GetEndPort(), nullptr);
}

TEST_F(NodePortTest, GetAndSetScenePosition)
{
  NodePort port({PortDirection::kInput, "type_abc"});

  EXPECT_EQ(port.GetScenePosition(), (std::pair<double, double>{0.0, 0.0}));

  port.SetScenePosition(1.0, 2.0);
  EXPECT_EQ(port.GetScenePosition(), (std::pair<double, double>{1.0, 2.0}));
}

TEST_F(NodePortTest, NotifyShapeCallback)
{
  ::testing::MockFunction<void(const NodeOperationStates&)> mock_callback;

  NodePort port({PortDirection::kInput, "type_abc"});
  port.SetNotifyShapeCallback(mock_callback.AsStdFunction());
  const NodeOperationStates operation_states({NodeOperationState::kCollapsedChildren});
  EXPECT_CALL(mock_callback, Call(operation_states)).Times(1);
  port.UpdateShape(operation_states);
}

TEST_F(NodePortTest, ProcessEvent)
{
  ::testing::MockFunction<NodePort::process_event_t> mock_callback;

  NodePort port({PortDirection::kInput, "type_abc"}, mock_callback.AsStdFunction());
  DoubleClickEvent event{&port};
  const node_event_t event_variant = event;
  EXPECT_CALL(mock_callback, Call(event_variant)).Times(1);
  port.ProcessEvent(event_variant);
}

}  // namespace mvvm::test
