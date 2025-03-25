/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/qtcore_helper.h"

#include <gtest/gtest.h>

using namespace mvvm;

class QtCoreHelperTest : public ::testing::Test
{
};

TEST_F(QtCoreHelperTest, GetStringList)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(utils::GetStringList(vec_t()), QStringList());
  EXPECT_EQ(utils::GetStringList(vec_t({"abc", "cde"})), QStringList({"abc", "cde"}));
}

TEST_F(QtCoreHelperTest, GetStdStringVector)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(utils::GetStdStringVector(QStringList()), vec_t());
  EXPECT_EQ(utils::GetStdStringVector(QStringList({"abc", "cde"})), vec_t({"abc", "cde"}));
}

TEST_F(QtCoreHelperTest, GetByteArray)
{
  QStringList expected = QStringList() << "aaa"
                                       << "bbb"
                                       << "ccc";

  auto array = utils::GetByteArray(expected);
  EXPECT_EQ(utils::GetStringList(array), expected);
}
