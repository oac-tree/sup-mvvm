// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/model/itempool.h"

#include "mvvm/model/sessionitem.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

using namespace ModelView;

//! Tests of ItemPool and its abilities to register/deregister SessionItem.

class ItemPoolTest : public ::testing::Test
{
};

TEST_F(ItemPoolTest, initialState)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  EXPECT_EQ(pool->size(), 0U);
}

//! Explicit item registrations.

TEST_F(ItemPoolTest, registerItem)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  std::unique_ptr<SessionItem> item(new SessionItem);

  // registering item
  auto key = pool->RegisterItem(item.get());
  EXPECT_EQ(pool->size(), 1U);
  EXPECT_FALSE(key.empty());

  // checking registered key and item
  EXPECT_EQ(key, pool->KeyForItem(item.get()));
  EXPECT_EQ(item.get(), pool->ItemForKey(key));

  // checking unexisting registration
  std::unique_ptr<SessionItem> item2(new SessionItem);
  EXPECT_EQ(ItemPool::identifier_t(), pool->KeyForItem(item2.get()));
  EXPECT_EQ(nullptr, pool->ItemForKey("ABC"));

  // registering second item
  auto key2 = pool->RegisterItem(item2.get());
  EXPECT_EQ(pool->size(), 2U);
  EXPECT_EQ(key2, pool->KeyForItem(item2.get()));
  EXPECT_FALSE(key == key2);

  // attempt to register item twice
  EXPECT_THROW(pool->RegisterItem(item2.get()), std::runtime_error);
}

//! Explicit item de-registrations.

TEST_F(ItemPoolTest, deregisterItem)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  std::unique_ptr<SessionItem> item1(new SessionItem);
  std::unique_ptr<SessionItem> item2(new SessionItem);

  auto key1 = pool->RegisterItem(item1.get());
  auto key2 = pool->RegisterItem(item2.get());

  EXPECT_EQ(pool->size(), 2U);
  EXPECT_EQ(item1.get(), pool->ItemForKey(key1));
  EXPECT_EQ(item2.get(), pool->ItemForKey(key2));

  // deregistering item
  pool->UnregisterItem(item1.get());
  EXPECT_EQ(pool->size(), 1U);
  EXPECT_EQ(nullptr, pool->ItemForKey(key1));
  EXPECT_EQ(item2.get(), pool->ItemForKey(key2));

  // attempt to deregister twice
  EXPECT_THROW(pool->UnregisterItem(item1.get()), std::runtime_error);

  // deregistering last remaining item
  pool->UnregisterItem(item2.get());
  EXPECT_EQ(pool->size(), 0U);
}

//! Providing custom key.

TEST_F(ItemPoolTest, customKey)
{
  std::shared_ptr<ItemPool> pool(new ItemPool);
  EXPECT_EQ(pool.use_count(), 1L);

  // explicit item registration
  const ItemPool::identifier_t id("abc-cde-fgh");
  auto item = new SessionItem;
  pool->RegisterItem(item, id);

  // attempt to reuse key again
  std::unique_ptr<SessionItem> item2(new SessionItem);
  EXPECT_THROW(pool->RegisterItem(item2.get(), id), std::runtime_error);

  delete item;
}
