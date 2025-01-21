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

#include "session_item_container.h"

#include "session_item.h"

#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace mvvm
{

SessionItemContainer::SessionItemContainer(mvvm::TagInfo tag_info) : m_tag_info(std::move(tag_info))
{
}

SessionItemContainer::~SessionItemContainer() = default;

bool SessionItemContainer::IsEmpty() const
{
  return m_items.empty();
}

std::size_t SessionItemContainer::GetItemCount() const
{
  return m_items.size();
}

std::vector<SessionItem*> SessionItemContainer::GetItems() const
{
  std::vector<SessionItem*> result;
  std::transform(m_items.begin(), m_items.end(), std::back_inserter(result),
                 [](const auto& item) { return item.get(); });
  return result;
}

bool SessionItemContainer::CanInsertItem(const SessionItem* item, std::size_t index) const
{
  if (!item || IndexOfItem(item).has_value())
  {
    // item belongs to this container already
    return false;
  }

  return CanInsertType(item->GetType(), index);
}

bool SessionItemContainer::CanInsertType(const std::string& item_type, std::size_t index) const
{
  const bool valid_index = index <= GetItemCount();
  const bool enough_place = !IsMaximumReached();
  const bool valid_type = m_tag_info.IsValidType(item_type);
  return valid_index && enough_place && valid_type;
}

bool SessionItemContainer::CanMoveItem(const SessionItem* item, std::size_t index) const
{
  if (!item || !m_tag_info.IsValidType(item->GetType()))
  {
    return false;
  }

  auto current_item_index = IndexOfItem(item);

  // item belongs to another container
  if (!current_item_index.has_value())
  {
    return CanInsertItem(item, index);
  }

  // item is already in proper place
  if (current_item_index == index)
  {
    return false;
  }

  // item belongs already to this container, and we can't remove it
  if (!CanTakeItem(current_item_index.value()))
  {
    return false;
  }

  return index < GetItemCount();
}

SessionItem* SessionItemContainer::InsertItem(std::unique_ptr<SessionItem> item, std::size_t index)
{
  if (!CanInsertItem(item.get(), index))
  {
    return nullptr;
  }

  auto result = item.get();
  m_items.insert(std::next(m_items.begin(), index), std::move(item));
  return result;
}

bool SessionItemContainer::CanTakeItem(std::size_t index) const
{
  return ItemAt(index) && !IsMinimumReached();
}

std::unique_ptr<SessionItem> SessionItemContainer::TakeItem(std::size_t index)
{
  if (IsMinimumReached())
  {
    return nullptr;
  }

  if (index < GetItemCount())
  {
    auto item = std::move(m_items.at(index));
    m_items.erase(std::next(m_items.begin(), index));
    return item;
  }

  return nullptr;
}

std::optional<std::size_t> SessionItemContainer::IndexOfItem(const SessionItem* item) const
{
  auto pos = std::find_if(m_items.begin(), m_items.end(),
                          [item](const auto& element) { return element.get() == item; });
  return pos == m_items.end() ? std::optional<std::size_t>() : std::distance(m_items.begin(), pos);
}

SessionItem* SessionItemContainer::ItemAt(std::size_t index) const
{
  return index < GetItemCount() ? m_items[index].get() : nullptr;
}

std::string SessionItemContainer::GetName() const
{
  return m_tag_info.GetName();
}

TagInfo SessionItemContainer::GetTagInfo() const
{
  return m_tag_info;
}

SessionItemContainer::const_iterator SessionItemContainer::begin() const
{
  return m_items.begin();
}

SessionItemContainer::const_iterator SessionItemContainer::end() const
{
  return m_items.end();
}

std::unique_ptr<SessionItemContainer> SessionItemContainer::Clone() const
{
  auto result = std::make_unique<SessionItemContainer>(m_tag_info);
  for (const auto& item : m_items)
  {
    result->m_items.push_back(item->Clone());
  }
  return result;
}

bool SessionItemContainer::IsMaximumReached() const
{
  return m_tag_info.GetMax() == GetItemCount();
}

bool SessionItemContainer::IsMinimumReached() const
{
  return m_tag_info.GetMin() == GetItemCount();
}

}  // namespace mvvm
