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
