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

#include "tagged_items.h"

#include "sessionitem.h"
#include "sessionitem_container.h"

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
  if (tag_index.tag.empty() && GetDefaultTag().empty())
  {
    return false;
  }

  // FIXME (!) remove implicit convertion of invalid tag/index into something meaningful
  // simplify code below, remove check above

  auto tag_container = GetContainer(tag_index.tag);
  // negative index means appending to the vector
  auto index = tag_index.index < 0 ? tag_container->GetItemCount() : tag_index.index;
  return tag_container->CanInsertItem(item, index);
}

SessionItem* TaggedItems::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  auto tag_container = GetContainer(tag_index.tag);
  // negative index means appending to the vector
  auto index = tag_index.index < 0 ? tag_container->GetItemCount() : tag_index.index;
  return GetContainer(tag_index.tag)->InsertItem(std::move(item), index);
}

bool TaggedItems::CanTakeItem(const TagIndex& tag_index) const
{
  return GetContainer(tag_index.tag)->CanTakeItem(tag_index.index);
}

std::unique_ptr<SessionItem> TaggedItems::TakeItem(const TagIndex& tag_index)
{
  return GetContainer(tag_index.tag)->TakeItem(tag_index.index);
}

SessionItem* TaggedItems::GetItem(const TagIndex& tag_index) const
{
  return GetContainer(tag_index.tag)->ItemAt(tag_index.index);
}

std::vector<SessionItem*> TaggedItems::GetItems(const std::string& tag) const
{
  return GetContainer(tag)->GetItems();
}

std::vector<SessionItem*> TaggedItems::GetAllItems() const
{
  std::vector<SessionItem*> result;
  for (auto& cont : m_containers)
  {
    auto container_items = cont->GetItems();
    result.insert(result.end(), container_items.begin(), container_items.end());
  }

  return result;
}

TagIndex TaggedItems::TagIndexOfItem(const SessionItem* item) const
{
  for (auto& cont : m_containers)
  {
    const int index = cont->IndexOfItem(item);
    if (index != -1)
    {
      return {cont->GetName(), index};
    }
  }

  return {"", -1};
}

TaggedItems::const_iterator TaggedItems::begin() const
{
  return m_containers.begin();
}

TaggedItems::const_iterator TaggedItems::end() const
{
  return m_containers.end();
}

int TaggedItems::GetTagCount() const
{
  return static_cast<int>(m_containers.size());
}

SessionItemContainer& TaggedItems::ContainerAt(int index)
{
  if (index < 0 || index >= GetTagCount())
  {
    throw RuntimeException("Error it SessionItemTags: wrong container index");
  }
  return *m_containers.at(index);
}

void TaggedItems::AppendContainer(std::unique_ptr<SessionItemContainer> container)
{
  m_containers.push_back(std::move(container));
}

std::unique_ptr<TaggedItems> TaggedItems::Clone(bool make_unique_id) const
{
  auto result = std::make_unique<TaggedItems>();
  result->m_default_tag = m_default_tag;
  for (const auto& container : m_containers)
  {
    result->m_containers.push_back(container->Clone(make_unique_id));
  }
  return result;
}

SessionItemContainer* TaggedItems::GetContainer(const std::string& tag) const
{
  const std::string tag_name_to_use = tag.empty() ? GetDefaultTag() : tag;
  auto container = FindContainer(tag_name_to_use);
  if (!container)
  {
    throw RuntimeException("Error in SessionItemTags: No such container [" + tag_name_to_use + "]");
  }

  return container;
}

SessionItemContainer* TaggedItems::FindContainer(const std::string& tag) const
{
  for (auto& container : m_containers)
  {
    if (container->GetName() == tag)
    {
      return container.get();
    }
  }

  return nullptr;
}

}  // namespace mvvm
