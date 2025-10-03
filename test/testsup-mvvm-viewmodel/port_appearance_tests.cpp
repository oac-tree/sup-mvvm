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

#include "mvvm/nodeeditor/port_appearance.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for PortAppearance class.
 */
class PortAppearanceTest : public ::testing::Test
{
};

TEST_F(PortAppearanceTest, InitialState)
{
  const PortStyle port_style{Qt::red, Qt::red, 42.0};
  const PortAppearance info(PortLocation::kTop, "abc", port_style);
  EXPECT_EQ(info.GetPortLabel(), QString("abc"));
  EXPECT_EQ(info.GetPortColor(), QColor("red"));
  EXPECT_EQ(info.GetPortLocation(), PortLocation::kTop);
  EXPECT_EQ(info.GetPortRadius(), 42.0);
}

TEST_F(PortAppearanceTest, ComparisonOperators)
{
  {
    const PortAppearance info1(PortLocation::kTop, "abc");
    const PortAppearance info2(PortLocation::kTop, "abc");
    EXPECT_TRUE(info1 == info2);
    EXPECT_FALSE(info1 != info2);
  }

  {
    const PortAppearance info1(PortLocation::kTop, "abc");
    const PortAppearance info2(PortLocation::kBottom, "abc");
    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }

  {
    const PortAppearance info1(PortLocation::kTop, "abc");
    const PortAppearance info2(PortLocation::kTop, "def");
    EXPECT_FALSE(info1 == info2);
    EXPECT_TRUE(info1 != info2);
  }
}

}  // namespace mvvm::test
