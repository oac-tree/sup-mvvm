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

#include "tagged_items.h"

#include "session_item.h"
#include "session_item_container.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <type_traits>
#include <utility>

namespace mvvm
{

namespace
{

/**
 * @brief Finds the container with the given name among the given containers.
 *
 * ContainerT selects the constness of the result.
 */
template <typename ContainerT, typename ContainersT>
ContainerT* FindContainerByName(ContainersT& containers, const std::string& name)
{
  for (const auto& container : containers)
  {
    if (container->GetName() == name)
    {
      return container.get();
    }
  }

  return nullptr;
}

/**
 * @brief Collects items of all given containers.
 *
 * ItemT selects the constness of the result.
 */
template <typename ItemT, typename ContainersT>
std::vector<ItemT*> CollectAllItems(ContainersT& containers)
{
  using ContainerT =
      std::conditional_t<std::is_const_v<ItemT>, const SessionItemContainer, SessionItemContainer>;

  std::vector<ItemT*> result;
  for (const auto& container : containers)
  {
    // unique_ptr doesn't propagate const, so it is this reference that picks the GetItems overload
    ContainerT& container_ref = *container;
    auto container_items = container_ref.GetItems();
    (void)result.insert(result.end(), container_items.begin(), container_items.end());
  }

  return result;
}

[[noreturn]] void ThrowNoSuchContainer(const std::string& tag)
{
  throw RuntimeException("Error in SessionItemTags: No such container [" + tag + "]");
}

[[noreturn]] void ThrowWrongContainerIndex()
{
  throw RuntimeException("Error it SessionItemTags: wrong container index");
}

}  // namespace

TaggedItems::TaggedItems() = default;

TaggedItems::~TaggedItems() = default;

void TaggedItems::RegisterTag(const TagInfo& tag_info, bool set_as_default)
{
  if (HasTag(tag_info.GetName()))
  {
    throw RuntimeException("Error in TaggedItems: existing name [" + tag_info.GetName() + "]");
  }

  (void)m_containers.emplace_back(std::make_unique<SessionItemContainer>(tag_info));
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

std::size_t TaggedItems::GetItemCount(const std::string& tag) const
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

const SessionItem* TaggedItems::FindItem(const TagIndex& tag_index) const
{
  // deliberately built on FindContainer (nullable) rather than GetContainer (throwing) to make
  // this method total
  const auto* container = FindContainer(tag_index.GetTag());
  return (container != nullptr) ? container->ItemAt(tag_index.GetIndex()) : nullptr;
}

SessionItem* TaggedItems::FindItem(const TagIndex& tag_index)
{
  auto* container = FindContainer(tag_index.GetTag());
  return (container != nullptr) ? container->ItemAt(tag_index.GetIndex()) : nullptr;
}

std::vector<const SessionItem*> TaggedItems::GetItems(const std::string& tag) const
{
  return GetContainer(tag)->GetItems();
}

std::vector<SessionItem*> TaggedItems::GetItems(const std::string& tag)
{
  return GetContainer(tag)->GetItems();
}

std::vector<const SessionItem*> TaggedItems::GetAllItems() const
{
  return CollectAllItems<const SessionItem>(m_containers);
}

std::vector<SessionItem*> TaggedItems::GetAllItems()
{
  return CollectAllItems<SessionItem>(m_containers);
}

std::size_t TaggedItems::GetTotalItemCount() const
{
  std::size_t result{0};
  for (const auto& container : m_containers)
  {
    result += container->GetItemCount();
  }
  return result;
}

TagIndex TaggedItems::TagIndexOfItem(const SessionItem* item) const
{
  for (auto& container : m_containers)
  {
    if (const auto index = container->IndexOfItem(item); index.has_value())
    {
      return {container->GetName(), index.value()};
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

std::size_t TaggedItems::GetTagCount() const
{
  return m_containers.size();
}

const SessionItemContainer& TaggedItems::ContainerAt(std::size_t index) const
{
  if (index >= GetTagCount())
  {
    ThrowWrongContainerIndex();
  }
  return *m_containers.at(index);
}

SessionItemContainer& TaggedItems::ContainerAt(std::size_t index)
{
  if (index >= GetTagCount())
  {
    ThrowWrongContainerIndex();
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

const SessionItemContainer* TaggedItems::GetContainer(const std::string& tag) const
{
  if (const auto* container = FindContainer(tag); container != nullptr)
  {
    return container;
  }

  ThrowNoSuchContainer(tag);
}

SessionItemContainer* TaggedItems::GetContainer(const std::string& tag)
{
  if (auto* container = FindContainer(tag); container != nullptr)
  {
    return container;
  }

  ThrowNoSuchContainer(tag);
}

const SessionItemContainer* TaggedItems::FindContainer(const std::string& tag) const
{
  return FindContainerByName<const SessionItemContainer>(m_containers,
                                                         tag.empty() ? GetDefaultTag() : tag);
}

SessionItemContainer* TaggedItems::FindContainer(const std::string& tag)
{
  return FindContainerByName<SessionItemContainer>(m_containers,
                                                   tag.empty() ? GetDefaultTag() : tag);
}

}  // namespace mvvm
