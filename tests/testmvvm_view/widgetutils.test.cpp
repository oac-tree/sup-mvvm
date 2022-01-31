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

#include "mvvm/widgets/widgetutils.h"

#include "test_utils.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QDir>

using namespace mvvm;

//! Testing functions from utils.

class WidgetUtilsTest : public ::testing::Test
{
};

//! Test of WithTildeHomePath function.

TEST_F(WidgetUtilsTest, ProjectWindowTitle)
{
  // untitled and unmodified project
  EXPECT_EQ(utils::ProjectWindowTitle(QString(""), false), "Untitled");

  // untitled and modified project
  EXPECT_EQ(utils::ProjectWindowTitle(QString(""), true), "*Untitled");

  // unmodified project without projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(QString("Untitled"), false), "Untitled");

  // modified project without projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(QString("Untitled"), true), "*Untitled");

  // unmodified project with projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(QString("/home/user/project1"), false), "project1");

  // modified project with projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(QString("/home/user/project1"), true), "*project1");
}

TEST_F(WidgetUtilsTest, ClickableText)
{
  EXPECT_EQ(utils::ClickableText("abc", "site.com"), QString(R"(<a href="site.com">abc</a>)"));
}

TEST_F(WidgetUtilsTest, GetStringList)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(utils::GetStringList(vec_t()), QStringList());
  EXPECT_EQ(utils::GetStringList(vec_t({"abc", "cde"})), QStringList({"abc", "cde"}));
}

TEST_F(WidgetUtilsTest, GetStdStringVector)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(utils::GetStdStringVector(QStringList()), vec_t());
  EXPECT_EQ(utils::GetStdStringVector(QStringList({"abc", "cde"})), vec_t({"abc", "cde"}));
}

TEST_F(WidgetUtilsTest, GetByteArray)
{
  QStringList expected = QStringList() << "aaa"
                                       << "bbb"
                                       << "ccc";

  auto array = utils::GetByteArray(expected);
  EXPECT_EQ(utils::GetStringList(array), expected);
}

TEST_F(WidgetUtilsTest, CreatePathPresentation)
{
  using utils::CreatePathPresentation;

  EXPECT_EQ(CreatePathPresentation(""), QString());

  EXPECT_EQ(CreatePathPresentation("/"), QString(R"(<a href="/">/</a>)"));

  // "/home" -> "/ home"
  EXPECT_EQ(CreatePathPresentation("/home"), QString(R"(<a href="/">/ </a><a href="/home">home</a>)"));

  // "/home/user" => "/ home / user"
  EXPECT_EQ(CreatePathPresentation("/home/user"),
            QString(R"(<a href="/">/ </a><a href="/home">home</a> / <a href="/home/user">user</a>)"));
}
