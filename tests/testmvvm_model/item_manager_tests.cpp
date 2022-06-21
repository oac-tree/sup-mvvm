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

#include "mvvm/model/item_manager.h"

#include <mvvm/factories/item_catalogue_factory.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/item_pool.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

//! Testing ItemFactory in the context of SessionModel and unique identifiers of SessionItem.

class ItemManagerTests : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    static inline const std::string Type = "TestItem";
    TestItem() : SessionItem(Type) {}
  };

  std::unique_ptr<ItemCatalogue<SessionItem>> CreateCatalogue()
  {
    auto result = std::make_unique<ItemCatalogue<SessionItem>>();
    result->RegisterItem<TestItem>();
    return result;
  }
};

//! Testing ItemManager constructed with pool and factory injected.

TEST_F(ItemManagerTests, Constructor)
{
  auto pool = std::make_shared<ItemPool>();
  auto factory = std::make_unique<mvvm::ItemFactory>(mvvm::CreateStandardItemCatalogue());
  auto factory_ptr = factory.get();

  ItemManager manager(std::move(factory), pool);
  EXPECT_EQ(manager.GetItemPool(), pool.get());
  EXPECT_EQ(manager.GetItemPool()->GetSize(), 0);
  EXPECT_EQ(manager.GetFactory(), factory_ptr);
}

//! Testing global function to consruct default ItemManager.

TEST_F(ItemManagerTests, CreateDefaultItemManager)
{
  auto manager = CreateDefaultItemManager();

  EXPECT_NE(manager->GetItemPool(), nullptr);
  EXPECT_NE(manager->GetFactory(), nullptr);

  // checking that at least one standard item is there
  auto item = manager->CreateItem(VectorItem::Type);
  EXPECT_EQ(item->GetType(), VectorItem::Type);
}

//! Testing global function to consruct default ItemManager, with external pool provided.

TEST_F(ItemManagerTests, CreateDefaultItemManagerFromPool)
{
  auto pool = std::make_shared<ItemPool>();
  auto manager = CreateDefaultItemManager(pool);

  EXPECT_EQ(manager->GetItemPool(), pool.get());
  EXPECT_NE(manager->GetFactory(), nullptr);

  // checking that at least one standard item is there
  auto item = manager->CreateItem(VectorItem::Type);
  EXPECT_EQ(item->GetType(), VectorItem::Type);
}

//! Testing global function to consruct default ItemManager, with user catalogue and external pool
//! provided.

TEST_F(ItemManagerTests, CreateDefaultItemManagerFromCatalogueAndPool)
{
  auto pool = std::make_shared<ItemPool>();
  auto manager = CreateDefaultItemManager(CreateCatalogue(), pool);

  EXPECT_EQ(manager->GetItemPool(), pool.get());
  EXPECT_NE(manager->GetFactory(), nullptr);

  // checking that at least one standard item is there
  auto item = manager->CreateItem(VectorItem::Type);
  EXPECT_EQ(item->GetType(), VectorItem::Type);

  // checking that test item sit ehre
  item = manager->CreateItem(TestItem::Type);
  EXPECT_EQ(item->GetType(), TestItem::Type);
}
