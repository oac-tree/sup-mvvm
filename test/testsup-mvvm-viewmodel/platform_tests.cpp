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

#include "mvvm/core/platform.h"

#include <gtest/gtest.h>

#include <QtGlobal>

using namespace mvvm;

/**
 * @brief Tests for functions defined in platform.h header.
 */
class PlatformTest : public ::testing::Test
{
};

//! Validating our own functions using Qt macros, in assumption, that QT knows the truth.
TEST_F(PlatformTest, CheckPlatform)
{
#if defined(Q_OS_WIN)
  EXPECT_TRUE(IsWindowsHost());
  EXPECT_FALSE(IsLinuxHost());
  EXPECT_FALSE(IsMacHost());
#endif

#if defined(Q_OS_MAC)
  EXPECT_FALSE(IsWindowsHost());
  EXPECT_FALSE(IsLinuxHost());
  EXPECT_TRUE(IsMacHost());
#endif

#if defined(Q_OS_LINUX)
  EXPECT_FALSE(IsWindowsHost());
  EXPECT_TRUE(IsLinuxHost());
  EXPECT_FALSE(IsMacHost());
#endif
}
