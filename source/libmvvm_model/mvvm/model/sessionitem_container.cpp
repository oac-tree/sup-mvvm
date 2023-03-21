/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/sessionitem_container.h"

#include <mvvm/model/sessionitem.h>
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

//! Returns number of items in given tag.

int SessionItemContainer::GetItemCount() const
{
  return static_cast<int>(m_items.size());
}

//! Returns vector of items in this container.

std::vector<SessionItem*> SessionItemContainer::GetItems() const
{
  std::vector<SessionItem*> result;
  std::transform(m_items.begin(), m_items.end(), std::back_inserter(result),
                 [](const auto& item) { return item.get(); });
  return result;
}

/*!
@brief Inserts item in a vector of children at given index, returns pointer to the item in the case
of success.
@param item Item to be inserted, ownership will be taken.
@param index Item insert index in a range [0, itemCount]

Insert index is an index which item will have after insertion. If item can't be inserted
(wrong model type, wrong index or maximum number of items reached), will return nullptr.
*/

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

//! Removes item at given index and returns it to the user.
//! If item can't be removed (item is a property item), will return nullptr.

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

//! Returns true if item can be taken.

bool SessionItemContainer::CanTakeItem(int index) const
{
  return ItemAt(index) && !IsMinimumReached();
}

//! Returns true if given item can be inserted under given index.

bool SessionItemContainer::CanInsertItem(const SessionItem* item, int index) const
{
  const bool valid_index = (index >= 0 && index <= GetItemCount());
  const bool enough_place = !IsMaximumReached();
  const bool valid_type = item && m_tag_info.IsValidType(item->GetType());
  return valid_index && enough_place && valid_type;
}

//! Returns index of item in vector of items.
//! Returns -1 if item doesn't belong to us.

int SessionItemContainer::IndexOfItem(const SessionItem* item) const
{
  return utils::IndexOfItem(m_items, item);
}

//! Returns item at given index. Returns nullptr if index is invalid.

SessionItem* SessionItemContainer::ItemAt(int index) const
{
  return index >= 0 && index < GetItemCount() ? m_items[static_cast<size_t>(index)].get() : nullptr;
}

//! Returns the name of the container.

std::string SessionItemContainer::GetName() const
{
  return m_tag_info.GetName();
}

//! Returns TagInfo of this container.

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

//! Provides container clone.

std::unique_ptr<SessionItemContainer> SessionItemContainer::Clone(bool make_unique_id) const
{
  auto result = std::make_unique<SessionItemContainer>(m_tag_info);
  for (const auto& item : m_items)
  {
    result->m_items.push_back(item->Clone(make_unique_id));
  }
  return result;
}

//! Returns true if no more items are allowed.

bool SessionItemContainer::IsMaximumReached() const
{
  return m_tag_info.GetMax() != -1 && m_tag_info.GetMax() == GetItemCount();
}

//! Returns true if less items than now is not allowed.

bool SessionItemContainer::IsMinimumReached() const
{
  return m_tag_info.GetMin() != -1 && m_tag_info.GetMin() == GetItemCount();
}

}  // namespace mvvm
