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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PROVIDERS_ITEM_VIEW_MAP_H_
#define MVVM_PROVIDERS_ITEM_VIEW_MAP_H_

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_utils.h>

#include <algorithm>
#include <map>

namespace mvvm
{

/**
 * @brief The ItemViewMap class is a map containing SessionItem objects and their corresponding
 * views.
 */
template <typename ItemT, typename ViewT>
class ItemViewMap
{
public:
  /**
   * @brief Insert item and its corresponding view to the map.
   */
  void Insert(ItemT item, ViewT view);

  /**
   * @brief  Insert item and corresponding view in a map.
   *
   * If such item already exists, will replace corresponding view with the given one.
   */
  void InsertOrAssign(ItemT item, ViewT view);

  /**
   * @brief Removes item and its corresponding view from the map (using key).
   */
  void Remove(ItemT item);

  /**
   * @brief Removes view and its corresponding view from the map (using value).
   */
  void Remove(ViewT view);

  /**
   * @brief Remove item, all its children and their corresponsding views from the map.
   *
   * Returns all removed views.
   */
  std::vector<ViewT> RemoveRecursively(ItemT item);

  /**
   * @brief Finds view for a given item.
   */
  ViewT FindView(ItemT item) const;

  /**
   * @brief Finds item for a given view.
   */
  ItemT FindItem(ViewT view) const;

  /**
   * @brief Clear the whole map.
   */
  void Clear();

  /**
   * @brief Returns number of entries in a map.
   */
  std::size_t GetSize() const;

private:
  std::map<ItemT, ViewT> m_item_to_view;
};

template <typename ItemT, typename ViewT>
inline void ItemViewMap<ItemT, ViewT>::Insert(ItemT item, ViewT view)
{
  const auto iter = m_item_to_view.find(item);
  if (iter != m_item_to_view.end())
  {
    throw RuntimeException("ItemViewMap: item is already registered");
  }
  (void)m_item_to_view.insert(iter, {item, view});
}

template <typename ItemT, typename ViewT>
inline void ItemViewMap<ItemT, ViewT>::InsertOrAssign(ItemT item, ViewT view)
{
  m_item_to_view.insert_or_assign(item, view);
}

template <typename ItemT, typename ViewT>
inline void ItemViewMap<ItemT, ViewT>::Remove(ItemT item)
{
  const auto iter = m_item_to_view.find(item);
  if (iter != m_item_to_view.end())
  {
    (void)m_item_to_view.erase(iter);
  }
  else
  {
    throw RuntimeException("ItemViewMap: non-existing item");
  }
}

template <typename ItemT, typename ViewT>
inline void ItemViewMap<ItemT, ViewT>::Remove(ViewT view)
{
  auto iter = std::find_if(m_item_to_view.begin(), m_item_to_view.end(),
                           [view](auto element) { return element.second == view; });

  if (iter != m_item_to_view.end())
  {
    (void)m_item_to_view.erase(iter);
  }
  else
  {
    throw RuntimeException("No connectable view found");
  }
}

template <typename ItemT, typename ViewT>
inline std::vector<ViewT> ItemViewMap<ItemT, ViewT>::RemoveRecursively(ItemT item)
{
  std::vector<ViewT> removed_views;
  const auto on_item = [this, &removed_views](auto item) -> bool
  {
    if (const auto iter = m_item_to_view.find(item); iter != m_item_to_view.end())
    {
      removed_views.push_back(iter->second);
      (void)m_item_to_view.erase(iter);
    }
    return true;
  };
  utils::iterate_if(item, on_item);
  return removed_views;
}

template <typename ItemT, typename ViewT>
inline ViewT ItemViewMap<ItemT, ViewT>::FindView(ItemT item) const
{
  const auto iter = m_item_to_view.find(item);
  return iter == m_item_to_view.end() ? nullptr : iter->second;
}

template <typename ItemT, typename ViewT>
inline ItemT ItemViewMap<ItemT, ViewT>::FindItem(ViewT view) const
{
  auto iter = std::find_if(m_item_to_view.begin(), m_item_to_view.end(),
                           [view](auto element) { return element.second == view; });
  return iter == m_item_to_view.end() ? nullptr : iter->first;
}

template <typename ItemT, typename ViewT>
inline void ItemViewMap<ItemT, ViewT>::Clear()
{
  m_item_to_view.clear();
}

template <typename ItemT, typename ViewT>
inline std::size_t ItemViewMap<ItemT, ViewT>::GetSize() const
{
  return m_item_to_view.size();
}

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_ITEM_VIEW_MAP_H_
