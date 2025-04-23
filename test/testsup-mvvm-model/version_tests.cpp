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

#include "mvvm/core/version.h"
#include "version_constants.h"  // from <build>/autogen

#include <gtest/gtest.h>

using namespace mvvm;

class VersionTest : public ::testing::Test
{
};

TEST_F(VersionTest, ParseVersionString)
{
  EXPECT_EQ(kProjectVersionMajor, ProjectVersionMajor());
  EXPECT_EQ(kProjectVersionMinor, ProjectVersionMinor());
  EXPECT_EQ(kProjectVersionPatch, ProjectVersionPatch());
  EXPECT_EQ(kProjectVersionString, ProjectVersion());
}
