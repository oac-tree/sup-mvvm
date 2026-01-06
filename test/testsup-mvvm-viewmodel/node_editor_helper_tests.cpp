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

#include "mvvm/nodeeditor/node_editor_constants.h"
#include "mvvm/nodeeditor/node_editor_helper.h"
#include "mvvm/nodeeditor/node_port.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

namespace mvvm::test
{

/**
 * @brief Tests for helper methods in node_editor_helper.h
 */
class NodeHelperTest : public ::testing::Test
{
};

TEST_F(NodeHelperTest, GetPortLocation)
{
  EXPECT_EQ(GetPortLocation(NodeTreeLayout::kLeftToRight, PortDirection::kDefault),
            PortLocation::kUnknown);
  EXPECT_EQ(GetPortLocation(NodeTreeLayout::kLeftToRight, PortDirection::kInput),
            PortLocation::kLeft);
  EXPECT_EQ(GetPortLocation(NodeTreeLayout::kLeftToRight, PortDirection::kOutput),
            PortLocation::kRight);

  EXPECT_EQ(GetPortLocation(NodeTreeLayout::kTopToBottom, PortDirection::kDefault),
            PortLocation::kUnknown);
  EXPECT_EQ(GetPortLocation(NodeTreeLayout::kTopToBottom, PortDirection::kInput),
            PortLocation::kBottom);
  EXPECT_EQ(GetPortLocation(NodeTreeLayout::kTopToBottom, PortDirection::kOutput),
            PortLocation::kTop);
}

TEST_F(NodeHelperTest, GetDefaultPortAppearance)
{
  const auto default_port_color = GetDefaultNodePortStyle().base_color;

  const PortInfo input_info(PortDirection::kInput, "abc1", "label1", nullptr);
  const PortInfo output_info(PortDirection::kOutput, "abc2", "label2", nullptr);
  const PortInfo default_info(PortDirection::kDefault, "abc3", "label3", nullptr);

  {  // kLeftToRight
    auto input_appearance = GetDefaultPortAppearance(NodeTreeLayout::kLeftToRight, input_info);
    EXPECT_EQ(input_appearance.GetPortLocation(), PortLocation::kLeft);
    EXPECT_EQ(input_appearance.GetPortLabel(), "label1");
    EXPECT_EQ(input_appearance.GetPortColor(), default_port_color);

    auto output_appearance = GetDefaultPortAppearance(NodeTreeLayout::kLeftToRight, output_info);
    EXPECT_EQ(output_appearance.GetPortLocation(), PortLocation::kRight);
    EXPECT_EQ(output_appearance.GetPortLabel(), "label2");
    EXPECT_EQ(output_appearance.GetPortColor(), default_port_color);

    auto default_appearance = GetDefaultPortAppearance(NodeTreeLayout::kLeftToRight, default_info);
    EXPECT_EQ(default_appearance.GetPortLocation(), PortLocation::kUnknown);
    EXPECT_EQ(default_appearance.GetPortLabel(), "label3");
    EXPECT_EQ(default_appearance.GetPortColor(), default_port_color);
  }

  {  // kTopToBottom
    auto input_appearance = GetDefaultPortAppearance(NodeTreeLayout::kTopToBottom, input_info);
    EXPECT_EQ(input_appearance.GetPortLocation(), PortLocation::kBottom);
    EXPECT_EQ(input_appearance.GetPortLabel(), "label1");
    EXPECT_EQ(input_appearance.GetPortColor(), default_port_color);

    auto output_appearance = GetDefaultPortAppearance(NodeTreeLayout::kTopToBottom, output_info);
    EXPECT_EQ(output_appearance.GetPortLocation(), PortLocation::kTop);
    EXPECT_EQ(output_appearance.GetPortLabel(), "label2");
    EXPECT_EQ(output_appearance.GetPortColor(), default_port_color);

    auto default_appearance = GetDefaultPortAppearance(NodeTreeLayout::kTopToBottom, default_info);
    EXPECT_EQ(default_appearance.GetPortLocation(), PortLocation::kUnknown);
    EXPECT_EQ(default_appearance.GetPortLabel(), "label3");
    EXPECT_EQ(default_appearance.GetPortColor(), default_port_color);
  }
}

TEST_F(NodeHelperTest, GetFilteredPortInfo)
{
  ASSERT_TRUE(GetFilteredPortInfo({}, PortDirection::kDefault).empty());

  const std::vector<PortInfo> vec({PortInfo{PortDirection::kInput, "abc1"},
                                   PortInfo{PortDirection::kOutput, "abc3"},
                                   PortInfo{PortDirection::kInput, "abc2"}});

  auto input_ports = GetFilteredPortInfo(vec, PortDirection::kInput);
  ASSERT_EQ(input_ports.size(), 2);
  EXPECT_EQ(input_ports[0].GetPortType(), "abc1");
  EXPECT_EQ(input_ports[1].GetPortType(), "abc2");

  EXPECT_EQ(GetFilteredPortInfo(vec, PortDirection::kOutput).size(), 1);
  EXPECT_TRUE(GetFilteredPortInfo(vec, PortDirection::kDefault).empty());
}

TEST_F(NodeHelperTest, GetPortsByDirection)
{
  ASSERT_TRUE(GetPortsByDirection({}, PortDirection::kDefault).empty());

  NodePort port1({PortDirection::kInput, "abc1"});
  NodePort port2({PortDirection::kOutput, "abc3"});
  NodePort port3({PortDirection::kInput, "abc2"});

  const std::vector<INodePort*> vec({&port1, &port2, &port3});

  auto input_ports = GetPortsByDirection(vec, PortDirection::kInput);
  ASSERT_EQ(input_ports.size(), 2);
  EXPECT_EQ(input_ports[0], &port1);
  EXPECT_EQ(input_ports[1], &port3);

  auto output_ports = GetPortsByDirection(vec, PortDirection::kOutput);
  ASSERT_EQ(output_ports.size(), 1);
  EXPECT_EQ(output_ports[0], &port2);

  EXPECT_TRUE(GetPortsByDirection(vec, PortDirection::kDefault).empty());
}

TEST_F(NodeHelperTest, GetConnectableChild)
{
  toyitems::SequenceItem sequence;
  toyitems::WaitItem wait;
  NodePort input_port({PortDirection::kInput, "type1", "label", &sequence});
  NodePort output_port({PortDirection::kOutput, "type1", "label", &wait});

  EXPECT_EQ(GetConnectableChild(nullptr, nullptr), nullptr);
  EXPECT_EQ(GetConnectableChild(&input_port, nullptr), nullptr);
  EXPECT_EQ(GetConnectableChild(nullptr, &output_port), nullptr);
  EXPECT_EQ(GetConnectableChild(&input_port, &input_port), nullptr);
  EXPECT_EQ(GetConnectableChild(&output_port, &output_port), nullptr);
  EXPECT_EQ(GetConnectableChild(&input_port, &output_port), &wait);
  EXPECT_EQ(GetConnectableChild(&output_port, &input_port), &wait);
}

TEST_F(NodeHelperTest, GetConnectableParent)
{
  toyitems::SequenceItem sequence;
  toyitems::WaitItem wait;
  NodePort input_port({PortDirection::kInput, "type1", "label", &sequence});
  NodePort output_port({PortDirection::kOutput, "type1", "label", &wait});

  EXPECT_EQ(GetConnectableParent(nullptr, nullptr), nullptr);
  EXPECT_EQ(GetConnectableParent(&input_port, nullptr), nullptr);
  EXPECT_EQ(GetConnectableParent(nullptr, &output_port), nullptr);
  EXPECT_EQ(GetConnectableParent(&input_port, &input_port), nullptr);
  EXPECT_EQ(GetConnectableParent(&output_port, &output_port), nullptr);
  EXPECT_EQ(GetConnectableParent(&input_port, &output_port), &sequence);
  EXPECT_EQ(GetConnectableParent(&output_port, &input_port), &sequence);
}

TEST_F(NodeHelperTest, ConnectPorts)
{
  {  // connect items without the model
    toyitems::SequenceItem sequence;
    toyitems::WaitItem wait;

    NodePort input_port({PortDirection::kInput, "type1", "label", &sequence});
    NodePort output_port({PortDirection::kOutput, "type1", "label", &wait});

    EXPECT_FALSE(ConnectPorts(&input_port, &output_port));
  }

  {  // connect sequence to wait
    SessionModel model;
    auto sequence = model.InsertItem<toyitems::SequenceItem>();
    auto wait = model.InsertItem<toyitems::SequenceItem>();
    NodePort input_port({PortDirection::kInput, "type1", "label", sequence});
    NodePort output_port({PortDirection::kOutput, "type1", "label", wait});

    EXPECT_TRUE(ConnectPorts(&input_port, &output_port));
    EXPECT_EQ(wait->GetParent(), sequence);
  }

  {  // connect wait to sequence
    SessionModel model;
    auto sequence = model.InsertItem<toyitems::SequenceItem>();
    auto wait = model.InsertItem<toyitems::SequenceItem>();
    NodePort input_port({PortDirection::kInput, "type1", "label", sequence});
    NodePort output_port({PortDirection::kOutput, "type1", "label", wait});

    EXPECT_TRUE(ConnectPorts(&output_port, &input_port));
    EXPECT_EQ(wait->GetParent(), sequence);
  }
}

TEST_F(NodeHelperTest, ConnectPortsWhenItemChangeParent)
{
  SessionModel model;
  auto sequence0 = model.InsertItem<toyitems::SequenceItem>();
  auto wait0 = model.InsertItem<toyitems::SequenceItem>(sequence0);
  auto sequence1 = model.InsertItem<toyitems::SequenceItem>();

  NodePort input_port({PortDirection::kInput, "type1", "label", sequence1});
  NodePort output_port({PortDirection::kOutput, "type1", "label", wait0});

  EXPECT_TRUE(ConnectPorts(&input_port, &output_port));
  EXPECT_EQ(wait0->GetParent(), sequence1);
}

TEST_F(NodeHelperTest, DisconnectSequenceFromWait)
{
  SessionModel model;
  auto sequence0 = model.InsertItem<toyitems::SequenceItem>();
  auto wait0 = model.InsertItem<toyitems::SequenceItem>(sequence0);
  auto container = model.InsertItem<ContainerItem>();

  NodePort input_port({PortDirection::kInput, "type1", "label", sequence0});
  NodePort output_port({PortDirection::kOutput, "type1", "label", wait0});

  EXPECT_TRUE(DisconnectPortsAndMove(&input_port, &output_port, container));

  // after disconnection item shall be placed to the provided container
  EXPECT_EQ(wait0->GetParent(), container);
  EXPECT_EQ(container->GetSize(), 1);
}

TEST_F(NodeHelperTest, DisconnectWaitFromSequence)
{
  SessionModel model;
  auto sequence0 = model.InsertItem<toyitems::SequenceItem>();
  auto wait0 = model.InsertItem<toyitems::SequenceItem>(sequence0);
  auto container = model.InsertItem<ContainerItem>();

  NodePort input_port({PortDirection::kInput, "type1", "label", sequence0});
  NodePort output_port({PortDirection::kOutput, "type1", "label", wait0});

  EXPECT_TRUE(DisconnectPortsAndMove(&output_port, &input_port, container));

  // after disconnection item shall be placed to the provided container
  EXPECT_EQ(wait0->GetParent(), container);
  EXPECT_EQ(container->GetSize(), 1);
}

TEST_F(NodeHelperTest, GetPortOrigin)
{
  toyitems::SequenceItem sequence;
  toyitems::WaitItem wait;
  NodePort input_port({PortDirection::kInput, "type1", "label", &sequence});
  NodePort output_port({PortDirection::kOutput, "type1", "label", &wait});

  EXPECT_EQ(GetPortOrigin(nullptr), nullptr);

  EXPECT_EQ(GetPortOrigin(&input_port), &sequence);
  EXPECT_EQ(GetPortOrigin(&output_port), &wait);

  EXPECT_EQ(GetPortOrigin<toyitems::SequenceItem>(&input_port), &sequence);
  EXPECT_EQ(GetPortOrigin<toyitems::WaitItem>(&input_port), nullptr);
}

}  // namespace mvvm::test
