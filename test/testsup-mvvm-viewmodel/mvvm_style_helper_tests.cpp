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

#include "mvvm/style/mvvm_style_helper.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

class MvvmStyleHelper : public ::testing::Test
{
};

TEST_F(MvvmStyleHelper, SystemPointSize)
{
  EXPECT_TRUE(style::SystemPointSize() > 0);
}

TEST_F(MvvmStyleHelper, ScaleToSystemFont)
{
  // Unit tests are in headless mode, QApplication is absent and "aplication" font size if always
  // the same.
  EXPECT_EQ(style::SystemPointSize(), style::SystemReferencePointSize());
  EXPECT_DOUBLE_EQ(style::ScaleToSystemFont(42.0), 42.0);
}

}  // namespace mvvm::test
