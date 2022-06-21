/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/core/unique_id_generator.h"

#include <gtest/gtest.h>

#include <iostream>
#include <set>

using namespace mvvm;

//! Testing UniqueIDGenerator

class UniqueIDGeneratorTests : public ::testing::Test
{
};

TEST_F(UniqueIDGeneratorTests, TwoIdentifiers)
{
  auto id1 = UniqueIdGenerator::Generate();
  auto id2 = UniqueIdGenerator::Generate();
  EXPECT_NE(id1, id2);
}

TEST_F(UniqueIDGeneratorTests, CheckAgainstPersistentID)
{
  // some arbitrary ID's generated on another machine
  std::vector<std::string> some_old_ids = {"82c1862f-6c06-416f-9290-0f4b868ca2bf",
                                           "cb7e6e4e-bf82-402d-88c4-06dea0cb7a91"};
  for (const auto& id : some_old_ids)
  {
    EXPECT_NE(UniqueIdGenerator::Generate(), id);
  }
}

TEST_F(UniqueIDGeneratorTests, MultipleIdentifiers)
{
  std::set<std::string> collection;
  const int ntries = 1000;
  for (size_t i = 0; i < ntries; ++i)
  {
    collection.insert(UniqueIdGenerator::Generate());
  }

  EXPECT_EQ(collection.size(), ntries);
}
