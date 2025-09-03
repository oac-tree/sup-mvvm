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
}

}  // namespace mvvm::test
