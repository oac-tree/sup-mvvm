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

#include "tagged_items.h"

#include "session_item.h"
#include "session_item_container.h"

#include <mvvm/core/exceptions.h>

namespace mvvm
{

TaggedItems::TaggedItems() = default;

TaggedItems::~TaggedItems() = default;

void TaggedItems::RegisterTag(const TagInfo& tag_info, bool set_as_default)
{
  if (HasTag(tag_info.GetName()))
  {
    throw RuntimeException("Error in TaggedItems: existing name [" + tag_info.GetName() + "]");
  }

  m_containers.emplace_back(std::make_unique<SessionItemContainer>(tag_info));
  if (set_as_default)
  {
    m_default_tag = tag_info.GetName();
  }
}

bool TaggedItems::HasTag(const std::string& tag) const
{
  for (auto& container : m_containers)
  {
    if (container->GetName() == tag)
    {
      return true;
    }
  }
  return false;
}

std::string TaggedItems::GetDefaultTag() const
{
  return m_default_tag;
}

void TaggedItems::SetDefaultTag(const std::string& tag)
{
  m_default_tag = tag;
}

int TaggedItems::GetItemCount(const std::string& tag) const
{
  return GetContainer(tag)->GetItemCount();
}

bool TaggedItems::CanInsertItem(const SessionItem* item, const TagIndex& tag_index) const
{
  if (auto container = FindContainer(tag_index.GetTag()); container)
  {
    return container->CanInsertItem(item, tag_index.GetIndex());
  }
  return false;
}

bool TaggedItems::CanInsertType(const std::string& item_type, const TagIndex& tag_index) const
{
  if (auto container = FindContainer(tag_index.GetTag()); container)
  {
    return container->CanInsertType(item_type, tag_index.GetIndex());
  }
  return false;
}

SessionItem* TaggedItems::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  return GetContainer(tag_index.GetTag())->InsertItem(std::move(item), tag_index.GetIndex());
}

bool TaggedItems::CanTakeItem(const TagIndex& tag_index) const
{
  if (auto container = FindContainer(tag_index.GetTag()); container)
  {
    return container->CanTakeItem(tag_index.GetIndex());
  }
  return false;
}

std::unique_ptr<SessionItem> TaggedItems::TakeItem(const TagIndex& tag_index)
{
  return GetContainer(tag_index.GetTag())->TakeItem(tag_index.GetIndex());
}

bool TaggedItems::CanMoveItem(const SessionItem* item, const TagIndex& tag_index) const
{
  if (auto container = FindContainer(tag_index.GetTag()); container)
  {
    return container->CanMoveItem(item, tag_index.GetIndex());
  }
  return false;
}

SessionItem* TaggedItems::GetItem(const TagIndex& tag_index) const
{
  return GetContainer(tag_index.GetTag())->ItemAt(tag_index.GetIndex());
}

std::vector<SessionItem*> TaggedItems::GetItems(const std::string& tag) const
{
  return GetContainer(tag)->GetItems();
}

std::vector<SessionItem*> TaggedItems::GetAllItems() const
{
  std::vector<SessionItem*> result;
  for (auto& container : m_containers)
  {
    auto container_items = container->GetItems();
    result.insert(result.end(), container_items.begin(), container_items.end());
  }

  return result;
}

TagIndex TaggedItems::TagIndexOfItem(const SessionItem* item) const
{
  for (auto& container : m_containers)
  {
    if (const auto index = container->IndexOfItem(item); index.has_value())
    {
      return {container->GetName(), static_cast<int>(index.value())};
    }
  }

  return TagIndex::Invalid();
}

TaggedItems::const_iterator TaggedItems::begin() const
{
  return m_containers.begin();
}

TaggedItems::const_iterator TaggedItems::end() const
{
  return m_containers.end();
}

size_t TaggedItems::GetTagCount() const
{
  return m_containers.size();
}

SessionItemContainer& TaggedItems::ContainerAt(size_t index)
{
  if (index >= GetTagCount())
  {
    throw RuntimeException("Error it SessionItemTags: wrong container index");
  }
  return *m_containers.at(index);
}

void TaggedItems::AppendContainer(std::unique_ptr<SessionItemContainer> container)
{
  m_containers.push_back(std::move(container));
}

std::unique_ptr<TaggedItems> TaggedItems::Clone() const
{
  auto result = std::make_unique<TaggedItems>();
  result->m_default_tag = m_default_tag;
  for (const auto& container : m_containers)
  {
    result->m_containers.push_back(container->Clone());
  }
  return result;
}

SessionItemContainer* TaggedItems::GetContainer(const std::string& tag) const
{
  if (auto container = FindContainer(tag); container)
  {
    return container;
  }

  throw RuntimeException("Error in SessionItemTags: No such container [" + tag + "]");
}

SessionItemContainer* TaggedItems::FindContainer(const std::string& tag) const
{
  const std::string tag_name_to_use = tag.empty() ? GetDefaultTag() : tag;
  for (const auto& container : m_containers)
  {
    if (container->GetName() == tag_name_to_use)
    {
      return container.get();
    }
  }

  return nullptr;
}

TagIndex TaggedItems::GetInsertTagIndex(const TagIndex& tag_index) const
{
  if (const auto container = FindContainer(tag_index.GetTag()); container)
  {
    const int actual_index =
        tag_index.GetIndex() < 0 ? container->GetItemCount() : tag_index.GetIndex();
    return TagIndex{container->GetName(), actual_index};
  }

  return TagIndex::Invalid();
}
}  // namespace mvvm
