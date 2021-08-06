// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

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
