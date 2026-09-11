/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "session_item_container.h"

#include "session_item.h"

#include <mvvm/utils/container_utils.h>

#include <algorithm>
#include <sstream>
#include <utility>

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

std::vector<const SessionItem*> SessionItemContainer::GetItems() const
{
  std::vector<const SessionItem*> result;
  (void)std::transform(m_items.begin(), m_items.end(), std::back_inserter(result),
                       [](const auto& item) { return item.get(); });
  return result;
}

std::vector<SessionItem*> SessionItemContainer::GetItems()
{
  return utils::RemoveConst(std::as_const(*this).GetItems());
}

bool SessionItemContainer::CanInsertItem(const SessionItem* item, std::size_t index) const
{
  if ((item == nullptr) || Contains(item))
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

SessionItem* SessionItemContainer::InsertItem(std::unique_ptr<SessionItem> item, std::size_t index)
{
  if (!item)
  {
    throw InvalidOperationException("Attempt to insert uninitialized item");
  }

  if (!CanInsertType(item->GetType(), index))
  {
    std::ostringstream ostr;
    ostr << "Can't insert item of type [" << item->GetType() << "] into tag ["
         << m_tag_info.GetName() << "] at index " << index;
    ;
    throw InvalidOperationException(ostr.str());
  }

  auto result = item.get();
  (void)m_items.insert(std::next(m_items.begin(), static_cast<std::ptrdiff_t>(index)),
                       std::move(item));
  return result;
}

bool SessionItemContainer::CanTakeItem(std::size_t index) const
{
  return (index < GetItemCount()) && !IsMinimumReached();
}

std::unique_ptr<SessionItem> SessionItemContainer::TakeItem(std::size_t index)
{
  if ((index < GetItemCount()) && !IsMinimumReached())
  {
    auto item = std::move(m_items.at(index));
    (void)m_items.erase(std::next(m_items.begin(), static_cast<std::ptrdiff_t>(index)));
    return item;
  }

  throw InvalidOperationException("Can't take item from container");
}

std::optional<std::size_t> SessionItemContainer::IndexOfItem(const SessionItem* item) const
{
  auto pos = std::find_if(m_items.begin(), m_items.end(),
                          [item](const auto& element) { return element.get() == item; });
  return pos == m_items.end() ? std::optional<std::size_t>() : std::distance(m_items.begin(), pos);
}

const SessionItem* SessionItemContainer::ItemAt(std::size_t index) const
{
  return (index < GetItemCount()) ? m_items[index].get() : nullptr;
}

SessionItem* SessionItemContainer::ItemAt(std::size_t index)
{
  return const_cast<SessionItem*>(std::as_const(*this).ItemAt(index));
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

bool SessionItemContainer::Contains(const SessionItem* item) const
{
  auto pos = std::find_if(m_items.begin(), m_items.end(),
                          [item](const auto& element) { return element.get() == item; });
  return pos != m_items.end();
}

}  // namespace mvvm
