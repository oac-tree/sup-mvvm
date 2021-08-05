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

void ItemManager::setItemFactory(std::unique_ptr<ItemFactoryInterface> factory)
{
  m_item_factory = std::move(factory);
}

void ItemManager::setItemPool(std::shared_ptr<ItemPool> pool)
{
  m_item_pool = std::move(pool);
}

ItemManager::~ItemManager() = default;

std::unique_ptr<SessionItem> ItemManager::createItem(const std::string& modelType) const
{
  return m_item_factory->createItem(modelType);
}

std::unique_ptr<SessionItem> ItemManager::createRootItem() const
{
  return std::make_unique<SessionItem>();
}

SessionItem* ItemManager::findItem(const std::string& id) const
{
  return m_item_pool ? m_item_pool->ItemForKey(id) : nullptr;
}

std::string ItemManager::findIdentifier(const SessionItem* item) const
{
  return m_item_pool ? m_item_pool->KeyForItem(item) : std::string();
}

const ItemPool* ItemManager::itemPool() const
{
  return m_item_pool.get();
}

ItemPool* ItemManager::itemPool()
{
  return m_item_pool.get();
}

void ItemManager::registerInPool(SessionItem* item)
{
  if (m_item_pool)
    m_item_pool->RegisterItem(item, item->identifier());
}

void ItemManager::unregisterFromPool(SessionItem* item)
{
  if (m_item_pool)
    m_item_pool->UnregisterItem(item);
}

const ItemFactoryInterface* ItemManager::factory() const
{
  return m_item_factory.get();
}

ItemFactoryInterface* ItemManager::factory()
{
  return const_cast<ItemFactoryInterface*>(static_cast<const ItemManager*>(this)->factory());
}
