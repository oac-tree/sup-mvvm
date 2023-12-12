/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "item_manager.h"

#include "item_factory.h"
#include "item_pool.h"
#include "sessionitem.h"

namespace mvvm
{

ItemManager::ItemManager(std::shared_ptr<ItemPool> pool) : m_item_pool(std::move(pool)) {}

ItemManager::~ItemManager() = default;

std::unique_ptr<SessionItem> ItemManager::CreateItem(const std::string& model_type) const
{
  return GetGlobalItemFactory().CreateItem(model_type);
}

SessionItem* ItemManager::FindItem(const std::string& id) const
{
  return m_item_pool->ItemForKey(id);
}

std::string ItemManager::FindIdentifier(const SessionItem* item) const
{
  return m_item_pool->KeyForItem(item);
}

const ItemPool* ItemManager::GetItemPool() const
{
  return m_item_pool.get();
}

ItemPool* ItemManager::GetItemPool()
{
  return m_item_pool.get();
}

std::shared_ptr<ItemPool> ItemManager::GetSharedPool()
{
  return m_item_pool;
}

void ItemManager::RegisterInPool(SessionItem* item)
{
  m_item_pool->RegisterItem(item, item->GetIdentifier());
}

void ItemManager::UnregisterFromPool(SessionItem* item)
{
  m_item_pool->UnregisterItem(item);
}

std::unique_ptr<ItemManagerInterface> CreateDefaultItemManager(std::shared_ptr<ItemPool> pool)
{
  auto item_pool = pool ? pool : std::make_shared<ItemPool>();
  return std::make_unique<ItemManager>(std::move(item_pool));
}

}  // namespace mvvm
