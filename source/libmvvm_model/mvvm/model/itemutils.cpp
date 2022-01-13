/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/itemutils.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionitemcontainer.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/utils/containerutils.h"

#include <iterator>

namespace mvvm
{
void utils::iterate(SessionItem* item, const std::function<void(SessionItem*)>& fun)
{
  if (item)
  {
    fun(item);
  }
  else
  {
    return;
  }

  for (auto child : item->GetAllItems())
  {
    iterate(child, fun);
  }
}

void utils::iterate_if(const SessionItem* item, const std::function<bool(const SessionItem*)>& fun)
{
  bool proceed_with_children(true);

  if (item)
  {
    proceed_with_children = fun(item);
  }

  if (!item || !proceed_with_children)
  {
    return;
  }

  for (auto child : item->GetAllItems())
  {
    iterate_if(child, fun);
  }
}

int utils::CopyNumber(const SessionItem* item)
{
  int result(-1);

  if (!item)
  {
    return result;
  }

  int count(0);
  auto item_type = item->GetType();
  if (auto parent = item->GetParent())
  {
    for (auto child : parent->GetAllItems())
    {
      if (child == item)
      {
        result = count;
      }
      if (child->GetType() == item_type)
      {
        ++count;
      }
    }
  }

  return count > 1 ? result : -1;
}

SessionItem* utils::ChildAt(const SessionItem* parent, int index)
{
  if (!parent)
  {
    return nullptr;
  }

  auto container = parent->GetAllItems();
  return index >= 0 && static_cast<size_t>(index) < container.size()
             ? container[static_cast<size_t>(index)]
             : nullptr;
}

int utils::IndexOfChild(const SessionItem* parent, const SessionItem* child)
{
  return utils::IndexOfItem(parent->GetAllItems(), child);
}

bool utils::HasTag(const SessionItem& item, const std::string& tag)
{
  return item.GetTaggedItems()->HasTag(tag);
}

bool utils::IsSinglePropertyTag(const SessionItem& item, const std::string& tag)
{
  return item.GetTaggedItems()->IsSinglePropertyTag(tag);
}

std::vector<std::string> utils::RegisteredTags(const SessionItem& item)
{
  std::vector<std::string> result;
  for (const auto container : *item.GetTaggedItems())
  {
    result.push_back(container->GetName());
  }
  return result;
}

std::vector<std::string> utils::RegisteredUniversalTags(const SessionItem& item)
{
  std::vector<std::string> result;
  for (const auto& tag : RegisteredTags(item))
  {
    if (!IsSinglePropertyTag(item, tag))
    {
      result.push_back(tag);
    }
  }
  return result;
}

std::vector<SessionItem*> utils::TopLevelItems(const SessionItem& item)
{
  std::vector<SessionItem*> result;
  for (auto child : item.GetAllItems())
  {
    if (child->IsVisible() && !IsSinglePropertyTag(item, item.TagIndexOfItem(child).tag))
    {
      result.push_back(child);
    }
  }
  return result;
}

std::vector<SessionItem*> utils::SinglePropertyItems(const SessionItem& item)
{
  std::vector<SessionItem*> result;
  for (auto child : item.GetAllItems())
  {
    if (child->IsVisible() && IsSinglePropertyTag(item, item.TagIndexOfItem(child).tag))
    {
      result.push_back(child);
    }
  }
  return result;
}

SessionItem* utils::FindNextSibling(SessionItem* item)
{
  auto parent = item ? item->GetParent() : nullptr;
  if (!parent)
  {
    return nullptr;
  }
  auto tag_index = item->GetTagIndex();
  return parent->GetItem(tag_index.tag, tag_index.index + 1);
}

SessionItem* utils::FindPreviousSibling(SessionItem* item)
{
  auto parent = item ? item->GetParent() : nullptr;
  if (!parent)
  {
    return nullptr;
  }
  auto tag_index = parent->TagIndexOfItem(item);
  return parent->GetItem(tag_index.tag, tag_index.index - 1);
}

SessionItem* utils::FindNextItemToSelect(SessionItem* item)
{
  auto next = FindNextSibling(item);
  auto closest = next ? next : FindPreviousSibling(item);
  return closest ? closest : item->GetParent();
}

bool utils::IsItemAncestor(const SessionItem* item, const SessionItem* candidate)
{
  if (!item || !candidate)
  {
    return false;
  }
  const SessionItem* parent = item->GetParent();
  while (parent)
  {
    if (parent == candidate)
    {
      return true;
    }
    else
    {
      parent = parent->GetParent();
    }
  }
  return false;
}

std::vector<SessionItem*> utils::UniqueItems(const std::vector<SessionItem*>& items)
{
  auto filtered = utils::UniqueWithOrder(items);
  std::vector<SessionItem*> result;
  std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(result),
               [](auto x) { return x != nullptr; });
  return result;
}

}  // namespace mvvm
