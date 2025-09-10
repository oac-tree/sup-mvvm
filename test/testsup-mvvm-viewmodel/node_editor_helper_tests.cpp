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

#include "mvvm/nodeeditor/node_editor_constants.h"
#include "mvvm/nodeeditor/node_editor_helper.h"

#include <gtest/gtest.h>

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
  const PortInfo input_info(PortDirection::kInput, "abc1", "label1");
  const PortInfo output_info(PortDirection::kOutput, "abc2", "label2");
  const PortInfo default_info(PortDirection::kDefault, "abc3", "label3");

  {  // kLeftToRight
    auto input_appearance = GetDefaultPortAppearance(NodeTreeLayout::kLeftToRight, input_info);
    EXPECT_EQ(input_appearance.GetPortLocation(), PortLocation::kLeft);
    EXPECT_EQ(input_appearance.GetPortLabel(), "label1");
    EXPECT_EQ(input_appearance.GetPortColor(), constants::DefaultPortColor());

    auto output_appearance = GetDefaultPortAppearance(NodeTreeLayout::kLeftToRight, output_info);
    EXPECT_EQ(output_appearance.GetPortLocation(), PortLocation::kRight);
    EXPECT_EQ(output_appearance.GetPortLabel(), "label2");
    EXPECT_EQ(output_appearance.GetPortColor(), constants::DefaultPortColor());

    auto default_appearance = GetDefaultPortAppearance(NodeTreeLayout::kLeftToRight, default_info);
    EXPECT_EQ(default_appearance.GetPortLocation(), PortLocation::kUnknown);
    EXPECT_EQ(default_appearance.GetPortLabel(), "label3");
    EXPECT_EQ(default_appearance.GetPortColor(), constants::DefaultPortColor());
  }

  {  // kTopToBottom
    auto input_appearance = GetDefaultPortAppearance(NodeTreeLayout::kTopToBottom, input_info);
    EXPECT_EQ(input_appearance.GetPortLocation(), PortLocation::kBottom);
    EXPECT_EQ(input_appearance.GetPortLabel(), "label1");
    EXPECT_EQ(input_appearance.GetPortColor(), constants::DefaultPortColor());

    auto output_appearance = GetDefaultPortAppearance(NodeTreeLayout::kTopToBottom, output_info);
    EXPECT_EQ(output_appearance.GetPortLocation(), PortLocation::kTop);
    EXPECT_EQ(output_appearance.GetPortLabel(), "label2");
    EXPECT_EQ(output_appearance.GetPortColor(), constants::DefaultPortColor());

    auto default_appearance = GetDefaultPortAppearance(NodeTreeLayout::kTopToBottom, default_info);
    EXPECT_EQ(default_appearance.GetPortLocation(), PortLocation::kUnknown);
    EXPECT_EQ(default_appearance.GetPortLabel(), "label3");
    EXPECT_EQ(default_appearance.GetPortColor(), constants::DefaultPortColor());
  }
}

TEST_F(NodeHelperTest, GetPortInfo)
{
  ASSERT_TRUE(GetPortInfo<PortDirection::kDefault>({}).empty());

  const std::vector<PortInfo> vec({PortInfo{PortDirection::kInput, "abc1"},
                                   PortInfo{PortDirection::kOutput, "abc3"},
                                   PortInfo{PortDirection::kInput, "abc2"}});

  auto input_ports = GetPortInfo<PortDirection::kInput>(vec);
  ASSERT_EQ(input_ports.size(), 2);
  EXPECT_EQ(input_ports[0].GetPortType(), "abc1");
  EXPECT_EQ(input_ports[1].GetPortType(), "abc2");

  EXPECT_EQ(GetPortInfo<PortDirection::kOutput>(vec).size(), 1);
  EXPECT_TRUE(GetPortInfo<PortDirection::kDefault>(vec).empty());
}

}  // namespace mvvm::test
