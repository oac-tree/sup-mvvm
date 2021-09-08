/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/widgetutils.h"

#include "test_utils.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QDir>

using namespace ModelView;

//! Testing functions from utils.

class WidgetUtilsTest : public ::testing::Test
{
};

//! Test of WithTildeHomePath function.

TEST_F(WidgetUtilsTest, ProjectWindowTitle)
{
  // untitled and unmodified project
  EXPECT_EQ(Utils::ProjectWindowTitle(QString(""), false), "Untitled");

  // untitled and modified project
  EXPECT_EQ(Utils::ProjectWindowTitle(QString(""), true), "*Untitled");

  // unmodified project without projectDir
  EXPECT_EQ(Utils::ProjectWindowTitle(QString("Untitled"), false), "Untitled");

  // modified project without projectDir
  EXPECT_EQ(Utils::ProjectWindowTitle(QString("Untitled"), true), "*Untitled");

  // unmodified project with projectDir
  EXPECT_EQ(Utils::ProjectWindowTitle(QString("/home/user/project1"), false), "project1");

  // modified project with projectDir
  EXPECT_EQ(Utils::ProjectWindowTitle(QString("/home/user/project1"), true), "*project1");
}

TEST_F(WidgetUtilsTest, ClickableText)
{
  EXPECT_EQ(Utils::ClickableText("abc", "site.com"), QString("<a href=\"site.com\">abc</a>"));
}

TEST_F(WidgetUtilsTest, GetStringList)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(Utils::GetStringList(vec_t()), QStringList());
  EXPECT_EQ(Utils::GetStringList(vec_t({"abc", "cde"})), QStringList({"abc", "cde"}));
}

TEST_F(WidgetUtilsTest, GetStdStringVector)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(Utils::GetStdStringVector(QStringList()), vec_t());
  EXPECT_EQ(Utils::GetStdStringVector(QStringList({"abc", "cde"})), vec_t({"abc", "cde"}));
}

TEST_F(WidgetUtilsTest, GetByteArray)
{
  QStringList expected = QStringList() << "aaa"
                                       << "bbb"
                                       << "ccc";

  auto array = Utils::GetByteArray(expected);
  EXPECT_EQ(Utils::GetStringList(array), expected);
}
