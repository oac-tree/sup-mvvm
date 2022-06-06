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

#include "mvvm/model/item_pool.h"

#include <stdexcept>

namespace mvvm
{

size_t ItemPool::GetSize() const
{
  if (m_key_to_item.size() != m_item_to_key.size())
  {
    throw std::runtime_error("Error in ItemPool: array size mismatch");
  }
  return m_key_to_item.size();
}

void ItemPool::RegisterItem(SessionItem* item, ItemPool::identifier_t key)

{
  if (m_item_to_key.find(item) != m_item_to_key.end())
  {
    throw std::runtime_error("Attempt to register already registered item.");
  }

  if (m_key_to_item.find(key) != m_key_to_item.end())
  {
    throw std::runtime_error("Attempt to reuse existing key");
  }

  m_key_to_item.insert(std::make_pair(key, item));
  m_item_to_key.insert(std::make_pair(item, key));
}

void ItemPool::UnregisterItem(SessionItem* item)
{
  auto it = m_item_to_key.find(item);
  if (it == m_item_to_key.end())
  {
    throw std::runtime_error("Attempt to deregister non existing item.");
  }
  auto key = it->second;
  m_item_to_key.erase(it);

  auto it2 = m_key_to_item.find(key);
  m_key_to_item.erase(it2);
}

ItemPool::identifier_t ItemPool::KeyForItem(const SessionItem* item) const
{
  const auto it = m_item_to_key.find(item);
  return it == m_item_to_key.end() ? identifier_t() : it->second;
}

SessionItem* ItemPool::ItemForKey(const ItemPool::identifier_t& key) const
{
  auto it = m_key_to_item.find(key);
  return it == m_key_to_item.end() ? nullptr : it->second;
}

}  // namespace mvvm
