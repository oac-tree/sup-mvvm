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

#include "mvvm/nodeeditor/port_info.h"

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
  PortInfo info("abc", "def");
  EXPECT_EQ(info.GetPortType(), std::string("abc"));
  EXPECT_EQ(info.GetPortColor(), std::string("def"));
}

TEST_F(PortInfoTest, GetDefaultPort)
{
  EXPECT_EQ(GetDefaultPortInfo().GetPortType(), GetRootPortInfo().GetPortType());
  EXPECT_NE(GetDefaultPortInfo().GetPortColor(), GetRootPortInfo().GetPortColor());
}

}  // namespace mvvm::test
