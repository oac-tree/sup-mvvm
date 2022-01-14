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

#include "mvvm/core/uniqueidgenerator.h"

#include <gtest/gtest.h>

#include <iostream>
#include <set>

using namespace mvvm;

//! Testing UniqueIDGenerator

class UniqueIDGeneratorTest : public ::testing::Test
{
};

TEST_F(UniqueIDGeneratorTest, TwoIdentifiers)
{
  auto id1 = UniqueIdGenerator::Generate();
  auto id2 = UniqueIdGenerator::Generate();
  EXPECT_NE(id1, id2);
  std::cout << id1 << " " << id2 << std::endl;
}

TEST_F(UniqueIDGeneratorTest, CheckAgainstPersistentID)
{
  // some arbitrary ID's generated on another machine
  std::vector<std::string> some_old_ids = {"{390e1599-aa5f-4b82-9929-99e7bc1c1b6d}",
                                           "{73ec2896-ac22-454a-a0f2-8db07a639ed2}"};
  for (const auto& id : some_old_ids)
  {
    EXPECT_NE(UniqueIdGenerator::Generate(), id);
  }
}

TEST_F(UniqueIDGeneratorTest, MultipleIdentifiers)
{
  std::set<std::string> collection;
  const int ntries = 1000;
  for (size_t i = 0; i < ntries; ++i)
  {
    collection.insert(UniqueIdGenerator::Generate());
  }

  EXPECT_EQ(collection.size(), ntries);
}
