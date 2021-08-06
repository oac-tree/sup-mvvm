// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/itemmanager.h"

#include "mvvm/factories/itemcataloguefactory.h"
#include "mvvm/model/itemfactory.h"
#include "mvvm/model/itempool.h"
#include "mvvm/model/sessionitem.h"

namespace
{
std::unique_ptr<ModelView::ItemFactory> DefaultItemFactory()
{
  return std::make_unique<ModelView::ItemFactory>(ModelView::CreateStandardItemCatalogue());
}
}  // namespace

using namespace ModelView;

ItemManager::ItemManager() : m_item_factory(DefaultItemFactory()) {}

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
    m_item_pool->RegisterItem(item, item->identifier());
}

void ItemManager::UnregisterFromPool(SessionItem* item)
{
  if (m_item_pool)
    m_item_pool->UnregisterItem(item);
}

const ItemFactoryInterface* ItemManager::GetFactory() const
{
  return m_item_factory.get();
}

ItemFactoryInterface* ItemManager::GetFactory()
{
  return const_cast<ItemFactoryInterface*>(static_cast<const ItemManager*>(this)->GetFactory());
}
