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

#include "mvvm/model/itemmanager.h"

#include "mvvm/model/itempool.h"

#include <gtest/gtest.h>

#include <memory>

using namespace ModelView;

//! Testing ItemFactory in the context of SessionModel and unique identifiers of SessionItem.

class ItemManagerTest : public ::testing::Test
{
};

TEST_F(ItemManagerTest, initialState)
{
  ItemManager manager;
  EXPECT_EQ(manager.GetItemPool(), nullptr);

  std::shared_ptr<ItemPool> pool(new ItemPool);
  manager.SetItemPool(pool);
  EXPECT_EQ(manager.GetItemPool(), pool.get());
  EXPECT_EQ(manager.GetItemPool()->size(), 0);
}
