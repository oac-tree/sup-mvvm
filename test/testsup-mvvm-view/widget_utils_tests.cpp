/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/widgets/widget_utils.h"

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <QDebug>
#include <QDir>
#include <QSize>

using namespace mvvm;

//! Testing functions from utils.

class WidgetUtilsTest : public ::testing::Test
{
};

//! Test of system font (performed deliberately without QApplication running).
//! Checking that the method SystemPointSize() known hardcoded value.

TEST_F(WidgetUtilsTest, SystemPointSize)
{
  EXPECT_TRUE(utils::SystemPointSize() > 0);
}

TEST_F(WidgetUtilsTest, SizeOfLetterM)
{
  EXPECT_TRUE(utils::SizeOfLetterM().width() > 0);
  EXPECT_TRUE(utils::SizeOfLetterM().height() > 0);
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
  EXPECT_EQ(CreatePathPresentation("/home"),
            QString(R"(<a href="/">/ </a><a href="/home">home</a>)"));

  // "/home/user" => "/ home / user"
  EXPECT_EQ(
      CreatePathPresentation("/home/user"),
      QString(R"(<a href="/">/ </a><a href="/home">home</a> / <a href="/home/user">user</a>)"));
}
