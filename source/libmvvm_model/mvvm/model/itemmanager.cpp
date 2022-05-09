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

#include "mvvm/model/itemmanager.h"

#include "mvvm/model/itemfactory.h"
#include "mvvm/model/itempool.h"
#include "mvvm/model/sessionitem.h"

namespace mvvm
{

ItemManager::ItemManager(std::unique_ptr<ItemFactoryInterface> factory,
                         std::shared_ptr<ItemPool> pool)
    : m_item_factory(std::move(factory)), m_item_pool(std::move(pool))
{
}

void ItemManager::SetItemFactory(std::unique_ptr<ItemFactoryInterface> factory)
{
  m_item_factory = std::move(factory);
}

void ItemManager::SetItemPool(std::shared_ptr<ItemPool> pool)
{
  m_item_pool = std::move(pool);
}

ItemManager::~ItemManager() = default;

std::unique_ptr<SessionItem> ItemManager::CreateItem(const std::string& model_type) const
{
  return m_item_factory->CreateItem(model_type);
}

std::unique_ptr<SessionItem> ItemManager::CreateEmptyItem() const
{
  return std::make_unique<SessionItem>();
}

SessionItem* ItemManager::FindItem(const std::string& id) const
{
  return m_item_pool ? m_item_pool->ItemForKey(id) : nullptr;
}

std::string ItemManager::FindIdentifier(const SessionItem* item) const
{
  return m_item_pool ? m_item_pool->KeyForItem(item) : std::string();
}

const ItemPool* ItemManager::GetItemPool() const
{
  return m_item_pool.get();
}

ItemPool* ItemManager::GetItemPool()
{
  return m_item_pool.get();
}

void ItemManager::RegisterInPool(SessionItem* item)
{
  if (m_item_pool)
  {
    m_item_pool->RegisterItem(item, item->GetIdentifier());
  }
}

void ItemManager::UnregisterFromPool(SessionItem* item)
{
  if (m_item_pool)
  {
    m_item_pool->UnregisterItem(item);
  }
}

const ItemFactoryInterface* ItemManager::GetFactory() const
{
  return m_item_factory.get();
}

ItemFactoryInterface* ItemManager::GetFactory()
{
  return const_cast<ItemFactoryInterface*>(static_cast<const ItemManager*>(this)->GetFactory());
}

}  // namespace mvvm
