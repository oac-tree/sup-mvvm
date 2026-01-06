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

#include "mvvm/model/path.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

class PathTests : public ::testing::Test
{
};

TEST_F(PathTests, InitialState)
{
  const Path path;
  EXPECT_TRUE(path.GetString().empty());
  EXPECT_EQ(path.GetLength(), 0U);
}

TEST_F(PathTests, AppendPrepend)
{
  Path path;
  path.Append(1);
  EXPECT_EQ(path.GetString(), "1");
  EXPECT_EQ(path.GetLength(), 1U);

  path.Append(2);
  EXPECT_EQ(path.GetString(), "1,2");
  EXPECT_EQ(path.GetLength(), 2U);

  path.Prepend(3);
  EXPECT_EQ(path.GetString(), "3,1,2");
  EXPECT_EQ(path.GetLength(), 3U);
}

TEST_F(PathTests, CreateFromVector)
{
  const Path path = Path::CreateFromVector({1, 2, 3});
  EXPECT_EQ(path.GetString(), "1,2,3");
}

TEST_F(PathTests, CreateFromString)
{
  const Path path = Path::CreateFromString("3,2,3");
  EXPECT_EQ(path.GetString(), "3,2,3");
}

TEST_F(PathTests, BeginEnd)
{
  const Path path = Path::CreateFromVector({1, 2, 3});
  EXPECT_NE(path.begin(), path.end());

  std::vector<Path::PathElement> elements;
  for (auto it = path.begin(); it != path.end(); ++it)
  {
    elements.push_back(*it);
  }
  EXPECT_EQ(elements, std::vector<Path::PathElement>({1, 2, 3}));
}

}  // namespace mvvm::test
