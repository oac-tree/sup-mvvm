/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#include "mvvm/widgets/widget_utils.h"

#include <mvvm/core/platform.h>

#include <QDir>
#include <gtest/gtest.h>

namespace mvvm::test
{

//! Testing functions from utils.

class WidgetUtilsTest : public ::testing::Test
{
};

TEST_F(WidgetUtilsTest, CollapseHomePath)
{
  using utils::CollapseHomePath;

  if (IsWindowsHost())
  {
    GTEST_SKIP() << "Home path collapsing is a Unix-only feature";
  }

  const QString home_path = QDir::homePath();

  EXPECT_EQ(CollapseHomePath(home_path), QString("~"));
  EXPECT_EQ(CollapseHomePath(home_path + "/project"), QString("~/project"));

  // paths outside of the home directory are left as they are
  EXPECT_EQ(CollapseHomePath("/opt/codac"), QString("/opt/codac"));
}

TEST_F(WidgetUtilsTest, ExpandHomePath)
{
  using utils::ExpandHomePath;

  if (IsWindowsHost())
  {
    GTEST_SKIP() << "Home path expansion is a Unix-only feature";
  }

  const QString home_path = QDir::homePath();

  EXPECT_EQ(ExpandHomePath("~"), home_path);
  EXPECT_EQ(ExpandHomePath("~/project"), home_path + "/project");

  // paths without leading tilde are left as they are
  EXPECT_EQ(ExpandHomePath(""), QString(""));
  EXPECT_EQ(ExpandHomePath("/opt/codac"), QString("/opt/codac"));
  EXPECT_EQ(ExpandHomePath("~other/project"), QString("~other/project"));
  EXPECT_EQ(ExpandHomePath("/opt/~/codac"), QString("/opt/~/codac"));
}

TEST_F(WidgetUtilsTest, CollapseAndExpandHomePath)
{
  if (IsWindowsHost())
  {
    GTEST_SKIP() << "Home path collapsing is a Unix-only feature";
  }

  const QString path = QDir::homePath() + "/project/untitled.xml";
  EXPECT_EQ(utils::ExpandHomePath(utils::CollapseHomePath(path)), path);
}

TEST_F(WidgetUtilsTest, ClickableText)
{
  EXPECT_EQ(utils::ClickableText("abc", "site.com"), QString(R"(<a href="site.com">abc</a>)"));
}

TEST_F(WidgetUtilsTest, CreatePathPresentation)
{
  using utils::CreatePathPresentation;

  EXPECT_EQ(CreatePathPresentation(""), QString());

  EXPECT_EQ(CreatePathPresentation("/"), QString(R"(<a href="/">/</a>)"));

  // "/home" -> "/ home"
  EXPECT_EQ(CreatePathPresentation("/home"),
            QString(R"(<a href="/">/ </a><a href="/home">home</a>)"));

  // "/home/user" => "/ home / user"
  EXPECT_EQ(
      CreatePathPresentation("/home/user"),
      QString(R"(<a href="/">/ </a><a href="/home">home</a> / <a href="/home/user">user</a>)"));
}

}  // namespace mvvm::test
