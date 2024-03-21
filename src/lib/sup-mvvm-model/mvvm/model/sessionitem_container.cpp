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

#include "sessionitem_container.h"

#include "sessionitem.h"

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

int SessionItemContainer::GetItemCount() const
{
  return static_cast<int>(m_items.size());
}

std::vector<SessionItem*> SessionItemContainer::GetItems() const
{
  std::vector<SessionItem*> result;
  std::transform(m_items.begin(), m_items.end(), std::back_inserter(result),
                 [](const auto& item) { return item.get(); });
  return result;
}

bool SessionItemContainer::CanInsertItem(const SessionItem* item, int index) const
{
  // if item belongs to this container already, a request to insert an item will
  // be prepended by item removal. Should adjust max number of items accordingly.

  const int max_item_count = IndexOfItem(item) >= 0 ? GetItemCount() - 1 : GetItemCount();

  const bool valid_index = (index >= 0 && index <= max_item_count);
  const bool enough_place = !IsMaximumReached();
  const bool valid_type = item && m_tag_info.IsValidType(item->GetType());
  return valid_index && enough_place && valid_type;
}

bool SessionItemContainer::CanInsertType(const std::string& item_type, int index) const
{
  const bool valid_index = (index >= 0 && index <= GetItemCount());
  const bool enough_place = !IsMaximumReached();
  const bool valid_type = m_tag_info.IsValidType(item_type);
  return valid_index && enough_place && valid_type;
}

SessionItem* SessionItemContainer::InsertItem(std::unique_ptr<SessionItem> item, int index)
{
  if (!CanInsertItem(item.get(), index))
  {
    return nullptr;
  }

  auto result = item.get();
  m_items.insert(std::next(m_items.begin(), index), std::move(item));
  return result;
}

bool SessionItemContainer::CanTakeItem(int index) const
{
  return ItemAt(index) && !IsMinimumReached();
}

std::unique_ptr<SessionItem> SessionItemContainer::TakeItem(int index)
{
  if (IsMinimumReached())
  {
    return nullptr;
  }

  if (index >= 0 && index < GetItemCount())
  {
    auto item = std::move(m_items.at(index));
    m_items.erase(std::next(m_items.begin(), index));
    return std::move(item);
  }

  return nullptr;
}

int SessionItemContainer::IndexOfItem(const SessionItem* item) const
{
  return utils::IndexOfItem(m_items, item);
}

SessionItem* SessionItemContainer::ItemAt(int index) const
{
  return index >= 0 && index < GetItemCount() ? m_items[static_cast<size_t>(index)].get() : nullptr;
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

std::unique_ptr<SessionItemContainer> SessionItemContainer::Clone(bool make_unique_id) const
{
  auto result = std::make_unique<SessionItemContainer>(m_tag_info);
  for (const auto& item : m_items)
  {
    result->m_items.push_back(item->Clone(make_unique_id));
  }
  return result;
}

bool SessionItemContainer::IsMaximumReached() const
{
  return m_tag_info.GetMax() != -1 && m_tag_info.GetMax() == GetItemCount();
}

bool SessionItemContainer::IsMinimumReached() const
{
  return m_tag_info.GetMin() != -1 && m_tag_info.GetMin() == GetItemCount();
}

}  // namespace mvvm
