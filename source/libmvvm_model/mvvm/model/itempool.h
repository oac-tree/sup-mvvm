// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_ITEMPOOL_H
#define MVVM_MODEL_ITEMPOOL_H

#include "mvvm/model_export.h"

#include <map>
#include <string>

namespace ModelView
{
class SessionItem;

//! Provides registration of SessionItem pointers and their unique identifiers
//! in global memory pool.

class MVVM_MODEL_EXPORT ItemPool
{
public:
  using identifier_t = std::string;

  ItemPool() = default;
  ItemPool(const ItemPool&) = delete;
  ItemPool(ItemPool&&) = delete;
  ItemPool& operator=(const ItemPool&) = delete;
  ItemPool& operator=(ItemPool&&) = delete;

  size_t size() const;

  identifier_t RegisterItem(SessionItem* item, identifier_t key = {});
  void UnregisterItem(SessionItem* item);

  identifier_t KeyForItem(const SessionItem* item) const;

  SessionItem* ItemForKey(const identifier_t& key) const;

private:
  std::map<identifier_t, SessionItem*> m_key_to_item;
  std::map<const SessionItem*, identifier_t> m_item_to_key;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMPOOL_H
