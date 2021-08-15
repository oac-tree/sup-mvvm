/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/itempool.h"

#include "mvvm/core/uniqueidgenerator.h"

#include <stdexcept>

using namespace ModelView;

size_t ItemPool::size() const
{
  if (m_key_to_item.size() != m_item_to_key.size())
    throw std::runtime_error("Error in ItemPool: array size mismatch");
  return m_key_to_item.size();
}

ItemPool::identifier_t ItemPool::RegisterItem(SessionItem* item, ItemPool::identifier_t key)

{
  if (m_item_to_key.find(item) != m_item_to_key.end())
    throw std::runtime_error(
        "ItemPool::register_item() -> Attempt to register already "
        "registered item.");

  if (key.empty())
  {
    key = UniqueIdGenerator::Generate();
    while (m_key_to_item.find(key) != m_key_to_item.end())
      key = UniqueIdGenerator::Generate();  // preventing improbable duplicates
  }
  else
  {
    if (m_key_to_item.find(key) != m_key_to_item.end())
      throw std::runtime_error(" ItemPool::register_item() -> Attempt to reuse existing key");
  }

  m_key_to_item.insert(std::make_pair(key, item));
  m_item_to_key.insert(std::make_pair(item, key));

  return key;
}

void ItemPool::UnregisterItem(SessionItem* item)
{
  auto it = m_item_to_key.find(item);
  if (it == m_item_to_key.end())
    throw std::runtime_error(
        "ItemPool::deregister_item() -> Attempt to deregister "
        "non existing item.");
  auto key = it->second;
  m_item_to_key.erase(it);

  auto it2 = m_key_to_item.find(key);
  m_key_to_item.erase(it2);
}

ItemPool::identifier_t ItemPool::KeyForItem(const SessionItem* item) const
{
  const auto it = m_item_to_key.find(item);
  if (it != m_item_to_key.end())
    return it->second;

  return {};
}

SessionItem* ItemPool::ItemForKey(const ItemPool::identifier_t& key) const
{
  auto it = m_key_to_item.find(key);
  if (it != m_key_to_item.end())
    return it->second;

  return nullptr;
}
