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

#include "mvvm/model/item_pool.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

//! Tests of ItemPool and its abilities to register/deregister SessionItem.

class ItemPoolTests : public ::testing::Test
{
};

TEST_F(ItemPoolTests, InitialState)
{
  ItemPool pool;
  EXPECT_EQ(pool.GetSize(), 0);
}

//! Explicit item registrations.

TEST_F(ItemPoolTests, RegisterItem)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  std::unique_ptr<SessionItem> item(new SessionItem);

  // registering item
  const std::string key("abc");
  pool->RegisterItem(item.get(), key);
  EXPECT_EQ(pool->GetSize(), 1);

  // checking registered key and item
  EXPECT_EQ(key, pool->KeyForItem(item.get()));
  EXPECT_EQ(item.get(), pool->ItemForKey(key));

  // checking unexisting registration
  std::unique_ptr<SessionItem> item2(new SessionItem);
  EXPECT_EQ(ItemPool::identifier_t(), pool->KeyForItem(item2.get()));
  EXPECT_EQ(nullptr, pool->ItemForKey("ABC"));

  // registering second item
  const std::string key2("abc2");
  pool->RegisterItem(item2.get(), key2);
  EXPECT_EQ(pool->GetSize(), 2);
  EXPECT_EQ(key2, pool->KeyForItem(item2.get()));
  EXPECT_NE(key, key2);

  // attempt to register item twice
  const std::string key3("abc3");
  EXPECT_THROW(pool->RegisterItem(item2.get(), key3), RuntimeException);
}

//! Explicit item de-registrations.

TEST_F(ItemPoolTests, DeregisterItem)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  std::unique_ptr<SessionItem> item1(new SessionItem);
  std::unique_ptr<SessionItem> item2(new SessionItem);

  const std::string key1("key1");
  const std::string key2("key2");
  pool->RegisterItem(item1.get(), key1);
  pool->RegisterItem(item2.get(), key2);

  EXPECT_EQ(pool->GetSize(), 2U);
  EXPECT_EQ(item1.get(), pool->ItemForKey(key1));
  EXPECT_EQ(item2.get(), pool->ItemForKey(key2));

  // deregistering item
  pool->UnregisterItem(item1.get());
  EXPECT_EQ(pool->GetSize(), 1U);
  EXPECT_EQ(nullptr, pool->ItemForKey(key1));
  EXPECT_EQ(item2.get(), pool->ItemForKey(key2));

  // attempt to deregister twice
  EXPECT_THROW(pool->UnregisterItem(item1.get()), RuntimeException);

  // deregistering last remaining item
  pool->UnregisterItem(item2.get());
  EXPECT_EQ(pool->GetSize(), 0U);
}

//! Providing custom key.

TEST_F(ItemPoolTests, CustomKey)
{
  auto pool = std::make_shared<ItemPool>();
  EXPECT_EQ(pool.use_count(), 1L);

  // explicit item registration
  const ItemPool::identifier_t id("abc-cde-fgh");
  auto item = std::make_unique<SessionItem>();
  pool->RegisterItem(item.get(), id);

  // attempt to reuse key again
  auto item2 = std::make_unique<SessionItem>();
  EXPECT_THROW(pool->RegisterItem(item2.get(), id), RuntimeException);
}
