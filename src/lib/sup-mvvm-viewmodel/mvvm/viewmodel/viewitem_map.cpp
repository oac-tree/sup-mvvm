/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewitem_map.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/core/exceptions.h>

namespace mvvm
{
ViewItemMap::ViewItemMap() = default;

void ViewItemMap::Insert(const SessionItem *const item, ViewItem *const view_item)
{
  const auto iter = m_item_to_view.find(item);
  if (iter != m_item_to_view.end())
  {
    throw RuntimeException("ViewItemMap: item is already registered");
  }
  m_item_to_view.insert(iter, {item, view_item});
}

ViewItem *ViewItemMap::FindView(const SessionItem *const item) const
{
  const auto iter = m_item_to_view.find(item);
  return iter == m_item_to_view.end() ? nullptr : iter->second;
}

void ViewItemMap::Remove(const SessionItem *const item)
{
  const auto iter = m_item_to_view.find(item);
  if (iter != m_item_to_view.end())
  {
    m_item_to_view.erase(iter);
  }
  else
  {
    throw RuntimeException("ViewItemMap: non-existing item");
  }
}

void ViewItemMap::OnItemRemove(const SessionItem *const item)
{
  const auto on_item = [this](auto item) -> bool
  {
    if (const auto iter = m_item_to_view.find(item); iter != m_item_to_view.end())
    {
      m_item_to_view.erase(iter);
    }
    return true;
  };
  utils::iterate_if(item, on_item);
}

void ViewItemMap::Clear()
{
  m_item_to_view.clear();
}

int ViewItemMap::GetSize() const
{
  return static_cast<int>(m_item_to_view.size());
}

}  // namespace mvvm
