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

TEST_F(ItemPoolTest, InitialState)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  EXPECT_EQ(pool->GetSize(), 0U);
}

//! Explicit item registrations.

TEST_F(ItemPoolTest, RegisterItem)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  std::unique_ptr<SessionItem> item(new SessionItem);

  // registering item
  auto key = pool->RegisterItem(item.get());
  EXPECT_EQ(pool->GetSize(), 1U);
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
  EXPECT_EQ(pool->GetSize(), 2U);
  EXPECT_EQ(key2, pool->KeyForItem(item2.get()));
  EXPECT_FALSE(key == key2);

  // attempt to register item twice
  EXPECT_THROW(pool->RegisterItem(item2.get()), std::runtime_error);
}

//! Explicit item de-registrations.

TEST_F(ItemPoolTest, DeregisterItem)
{
  std::unique_ptr<ItemPool> pool(new ItemPool);
  std::unique_ptr<SessionItem> item1(new SessionItem);
  std::unique_ptr<SessionItem> item2(new SessionItem);

  auto key1 = pool->RegisterItem(item1.get());
  auto key2 = pool->RegisterItem(item2.get());

  EXPECT_EQ(pool->GetSize(), 2U);
  EXPECT_EQ(item1.get(), pool->ItemForKey(key1));
  EXPECT_EQ(item2.get(), pool->ItemForKey(key2));

  // deregistering item
  pool->UnregisterItem(item1.get());
  EXPECT_EQ(pool->GetSize(), 1U);
  EXPECT_EQ(nullptr, pool->ItemForKey(key1));
  EXPECT_EQ(item2.get(), pool->ItemForKey(key2));

  // attempt to deregister twice
  EXPECT_THROW(pool->UnregisterItem(item1.get()), std::runtime_error);

  // deregistering last remaining item
  pool->UnregisterItem(item2.get());
  EXPECT_EQ(pool->GetSize(), 0U);
}

//! Providing custom key.

TEST_F(ItemPoolTest, CustomKey)
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
