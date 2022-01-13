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

#include "mvvm/model/path.h"

#include "mvvm/model/modelutils.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taginfo.h"

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

class PathTest : public ::testing::Test
{
};

TEST_F(PathTest, InitialState)
{
  Path path;
  EXPECT_TRUE(path.GetString().empty());
}

TEST_F(PathTest, AppendPrepend)
{
  Path path;
  path.Append(1);
  EXPECT_EQ(path.GetString(), "1");

  path.Append(2);
  EXPECT_EQ(path.GetString(), "1,2");

  path.Prepend(3);
  EXPECT_EQ(path.GetString(), "3,1,2");
}

TEST_F(PathTest, CreateFromVector)
{
  Path path = Path::CreateFromVector({1, 2, 3});
  EXPECT_EQ(path.GetString(), "1,2,3");
}

TEST_F(PathTest, CreateFromString)
{
  Path path = Path::CreateFromString("3,2,3");
  EXPECT_EQ(path.GetString(), "3,2,3");
}
