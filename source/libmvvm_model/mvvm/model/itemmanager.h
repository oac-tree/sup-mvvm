// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_ITEMMANAGER_H
#define MVVM_MODEL_ITEMMANAGER_H

#include <memory>
#include <string>

namespace ModelView
{
class SessionItem;
class ItemPool;
class ItemFactoryInterface;

//! Manages item creation/registration for SessionModel.

class ItemManager
{
public:
  ItemManager();
  ~ItemManager();

  void setItemFactory(std::unique_ptr<ItemFactoryInterface> factory);
  void setItemPool(std::shared_ptr<ItemPool> pool);

  std::unique_ptr<SessionItem> createItem(const std::string& modelType = {}) const;

  std::unique_ptr<SessionItem> createRootItem() const;

  SessionItem* findItem(const std::string& id) const;

  std::string findIdentifier(const SessionItem* item) const;

  const ItemPool* itemPool() const;
  ItemPool* itemPool();

  void registerInPool(SessionItem* item);
  void unregisterFromPool(SessionItem* item);

  const ItemFactoryInterface* factory() const;

  ItemFactoryInterface* factory();

private:
  std::shared_ptr<ItemPool> m_item_pool;
  std::unique_ptr<ItemFactoryInterface> m_item_factory;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMMANAGER_H
