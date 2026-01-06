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

#include "mvvm/nodeeditor/port_info.h"

#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodeInfo class.
 */
class PortInfoTest : public ::testing::Test
{
};

TEST_F(PortInfoTest, InitialState)
{
  {
    const PortInfo info(PortDirection::kDefault, "abc");
    EXPECT_EQ(info.GetPortType(), std::string("abc"));
    EXPECT_EQ(info.GetPortDirection(), PortDirection::kDefault);
    EXPECT_TRUE(info.GetPortLabel().empty());
    EXPECT_EQ(info.GetOrigin(), nullptr);
  }

  {
    const PortInfo info(PortDirection::kDefault, "abc", "label", nullptr);
    EXPECT_EQ(info.GetPortType(), std::string("abc"));
    EXPECT_EQ(info.GetPortDirection(), PortDirection::kDefault);
    EXPECT_EQ(info.GetPortLabel(), std::string("label"));
    EXPECT_EQ(info.GetOrigin(), nullptr);
  }

  {
    SessionItem item;
    const PortInfo info(PortDirection::kDefault, "abc", "label", &item);
    EXPECT_EQ(info.GetPortType(), std::string("abc"));
    EXPECT_EQ(info.GetPortDirection(), PortDirection::kDefault);
    EXPECT_EQ(info.GetPortLabel(), std::string("label"));
    EXPECT_EQ(info.GetOrigin(), &item);
  }
}

TEST_F(PortInfoTest, ComparisonOperators)
{
  {
    const PortInfo info1(PortDirection::kDefault, "abc");
    const PortInfo info2(PortDirection::kDefault, "abc");
    EXPECT_TRUE(info1 == info2);
    EXPECT_FALSE(info1 != info2);
  }

  {
    const PortInfo info1(PortDirection::kInput, "abc");
    const PortInfo info2(PortDirection::kOutput, "abc");
    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }

  {
    const PortInfo info1(PortDirection::kDefault, "abc");
    const PortInfo info2(PortDirection::kDefault, "def");
    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }
}

}  // namespace mvvm::test
