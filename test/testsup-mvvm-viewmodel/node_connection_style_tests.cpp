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

#include "mvvm/nodeeditor/node_connection_style.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for NodeConnectionStyle class.
 */
class NodeConnectionStyleTest : public ::testing::Test
{
};

TEST_F(NodeConnectionStyleTest, CreateDefaultConnectableShapeStyle)
{
  auto dark_style = CreateDefaultNodeConnectionStyle(ColorFlavor::kForDarkThemes);
  auto light_style = CreateDefaultNodeConnectionStyle(ColorFlavor::kForLightThemes);

  // EXPECT_NE(dark_style.base_color, light_style.base_color);
}

}  // namespace mvvm::test
