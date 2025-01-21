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

#include "item_pool.h"

#include <mvvm/core/exceptions.h>

namespace mvvm
{

std::size_t ItemPool::GetSize() const
{
  if (m_key_to_item.size() != m_item_to_key.size())
  {
    throw RuntimeException("Error in ItemPool::GetSize(): array size mismatch");
  }
  return m_key_to_item.size();
}

void ItemPool::RegisterItem(SessionItem* item, ItemPool::identifier_t key)

{
  if (m_item_to_key.find(item) != m_item_to_key.end())
  {
    throw RuntimeException(
        "Error in ItemPool::RegisterItem(): attempt to register already registered item.");
  }

  if (m_key_to_item.find(key) != m_key_to_item.end())
  {
    throw RuntimeException("Error in ItemPool::RegisterItem(): attempt to reuse existing key");
  }

  m_key_to_item.insert(std::make_pair(key, item));
  m_item_to_key.insert(std::make_pair(item, key));
}

void ItemPool::UnregisterItem(SessionItem* item)
{
  auto iter = m_item_to_key.find(item);
  if (iter == m_item_to_key.end())
  {
    throw RuntimeException(
        "Error in ItemPool::UnregisterItem: attempt to deregister non existing item.");
  }
  auto key = iter->second;
  m_item_to_key.erase(iter);

  auto iter2 = m_key_to_item.find(key);
  m_key_to_item.erase(iter2);
}

ItemPool::identifier_t ItemPool::KeyForItem(const SessionItem* item) const
{
  const auto iter = m_item_to_key.find(item);
  return iter == m_item_to_key.end() ? identifier_t() : iter->second;
}

SessionItem* ItemPool::ItemForKey(const ItemPool::identifier_t& key) const
{
  auto iter = m_key_to_item.find(key);
  return iter == m_key_to_item.end() ? nullptr : iter->second;
}

}  // namespace mvvm
