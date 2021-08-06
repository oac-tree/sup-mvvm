// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_ITEMMANAGER_H
#define MVVM_MODEL_ITEMMANAGER_H

#include "mvvm/model_export.h"

#include <memory>
#include <string>

namespace ModelView
{
class SessionItem;
class ItemPool;
class ItemFactoryInterface;

//! Manages item creation/registration for SessionModel.

class MVVM_MODEL_EXPORT ItemManager
{
public:
  ItemManager();
  ~ItemManager();

  void SetItemFactory(std::unique_ptr<ItemFactoryInterface> factory);
  void SetItemPool(std::shared_ptr<ItemPool> pool);

  std::unique_ptr<SessionItem> CreateItem(const std::string& model_type) const;

  std::unique_ptr<SessionItem> CreateEmptyItem() const;

  SessionItem* FindItem(const std::string& id) const;

  std::string FindIdentifier(const SessionItem* item) const;

  const ItemPool* GetItemPool() const;
  ItemPool* GetItemPool();

  void RegisterInPool(SessionItem* item);
  void UnregisterFromPool(SessionItem* item);

  const ItemFactoryInterface* GetFactory() const;

  ItemFactoryInterface* GetFactory();

private:
  std::shared_ptr<ItemPool> m_item_pool;
  std::unique_ptr<ItemFactoryInterface> m_item_factory;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMMANAGER_H
