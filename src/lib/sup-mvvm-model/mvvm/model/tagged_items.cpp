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

#include "tagged_items.h"

#include "sessionitem.h"
#include "sessionitem_container.h"

#include <mvvm/core/exceptions.h>

#include <stdexcept>

namespace mvvm
{

TaggedItems::TaggedItems() = default;

TaggedItems::~TaggedItems() = default;

void TaggedItems::RegisterTag(const TagInfo& tag_info, bool set_as_default)
{
  if (HasTag(tag_info.GetName()))
  {
    throw std::runtime_error("SessionItemTags::registerTag() -> Error. Existing name '"
                             + tag_info.GetName() + "'");
  }

  m_containers.emplace_back(std::make_unique<SessionItemContainer>(tag_info));
  if (set_as_default)
  {
    m_default_tag = tag_info.GetName();
  }
}

//! Returns true if container with such name exists.

bool TaggedItems::HasTag(const std::string& name) const
{
  for (auto& tag : m_containers)
  {
    if (tag->GetName() == name)
    {
      return true;
    }
  }
  return false;
}

//! Returns the name of the default tag.

std::string TaggedItems::GetDefaultTag() const
{
  return m_default_tag;
}

void TaggedItems::SetDefaultTag(const std::string& name)
{
  m_default_tag = name;
}

int TaggedItems::GetItemCount(const std::string& tag_name) const
{
  return GetContainer(tag_name)->GetItemCount();
}

//! Returns true if item can be inserted.

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

//! Inserts item in container with given tag name and at given index.
//! Returns pointer to item in the case of success. If tag name is empty, default tag will be used.

SessionItem* TaggedItems::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  auto tag_container = GetContainer(tag_index.tag);
  // negative index means appending to the vector
  auto index = tag_index.index < 0 ? tag_container->GetItemCount() : tag_index.index;
  return GetContainer(tag_index.tag)->InsertItem(std::move(item), index);
}

//! Returns true if item can be taken.

bool TaggedItems::CanTakeItem(const TagIndex& tag_index) const
{
  return GetContainer(tag_index.tag)->CanTakeItem(tag_index.index);
}

//! Removes item at given index and for given tag, returns it to the user.

std::unique_ptr<SessionItem> TaggedItems::TakeItem(const TagIndex& tag_index)
{
  return GetContainer(tag_index.tag)->TakeItem(tag_index.index);
}

//! Returns item at given index of given tag.

SessionItem* TaggedItems::GetItem(const TagIndex& tag_index) const
{
  return GetContainer(tag_index.tag)->ItemAt(tag_index.index);
}

//! Returns vector of items in the container with given name.
//! If tag name is empty, default tag will be used.

std::vector<SessionItem*> TaggedItems::GetItems(const std::string& tag) const
{
  return GetContainer(tag)->GetItems();
}

//! Returns vector of all items in all containers.

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

//! Returns tag name and index of item in container.

TagIndex TaggedItems::TagIndexOfItem(const SessionItem* item) const
{
  for (auto& cont : m_containers)
  {
    int index = cont->IndexOfItem(item);
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
    throw std::runtime_error("Error it SessionItemTags: wrong container index");
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

//! Returns container corresponding to given tag name. If name is empty,
//! default tag will be used. Exception is thrown if no such tag exists.

SessionItemContainer* TaggedItems::GetContainer(const std::string& tag_name) const
{
  std::string tagName = tag_name.empty() ? GetDefaultTag() : tag_name;
  auto container = FindContainer(tagName);
  if (!container)
  {
    throw std::runtime_error("SessionItemTags::container() -> Error. No such container '" + tagName
                             + "'");
  }

  return container;
}

//! Returns container corresponding to given tag name.

SessionItemContainer* TaggedItems::FindContainer(const std::string& tag_name) const
{
  for (auto& cont : m_containers)
  {
    if (cont->GetName() == tag_name)
    {
      return cont.get();
    }
  }

  return nullptr;
}

}  // namespace mvvm
