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

#include "mvvm/model/item_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/factories/item_copy_strategy_factory.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionitem_container.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/utils/container_utils.h>

#include <stack>

namespace mvvm::utils
{

void iterate(SessionItem* item, const std::function<void(SessionItem*)>& fun)
{
  if (!item)
  {
    return;
  }

  std::stack<SessionItem*> stack;
  stack.push(item);

  while (!stack.empty())
  {
    auto top_item = stack.top();
    fun(top_item);

    stack.pop();

    auto children = top_item->GetAllItems();
    // push in reverse order to provide correct child order processing
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push(*it);
    }
  }
}

void iterate_if(const SessionItem* item, const std::function<bool(const SessionItem*)>& func)
{
  if (!item)
  {
    return;
  }

  std::stack<const SessionItem*> stack;
  stack.push(item);

  while (!stack.empty())
  {
    auto top_item = stack.top();
    if (!func(top_item))
    {
      break;
    }

    stack.pop();

    auto children = top_item->GetAllItems();
    // push in reverse order to provide correct child order processing
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push(*it);
    }
  }
}

int CopyNumber(const SessionItem* item)
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

SessionItem* ChildAt(const SessionItem* parent, int index)
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

int IndexOfChild(const SessionItem* parent, const SessionItem* child)
{
  return IndexOfItem(parent->GetAllItems(), child);
}

bool HasTag(const SessionItem& item, const std::string& tag_name)
{
  return item.GetTaggedItems()->HasTag(tag_name);
}

std::vector<std::string> RegisteredTags(const SessionItem& item)
{
  std::vector<std::string> result;
  for (const auto& container : *item.GetTaggedItems())
  {
    result.push_back(container->GetName());
  }
  return result;
}

std::vector<SessionItem*> TopLevelItems(const SessionItem& item)
{
  std::vector<SessionItem*> result;
  for (auto child : item.GetAllItems())
  {
    if (child->IsVisible() && !HasAppearanceFlag(*child, kProperty))
    {
      result.push_back(child);
    }
  }
  return result;
}

std::vector<SessionItem*> SinglePropertyItems(const SessionItem& item)
{
  std::vector<SessionItem*> result;
  for (auto child : item.GetAllItems())
  {
    if (child->IsVisible() && HasAppearanceFlag(*child, kProperty))
    {
      result.push_back(child);
    }
  }
  return result;
}

SessionItem* FindNextSibling(SessionItem* item)
{
  auto parent = item ? item->GetParent() : nullptr;
  if (!parent)
  {
    return nullptr;
  }
  return parent->GetItem(item->GetTagIndex().Next());
}

SessionItem* FindPreviousSibling(SessionItem* item)
{
  auto parent = item ? item->GetParent() : nullptr;
  if (!parent)
  {
    return nullptr;
  }
  return parent->GetItem(item->GetTagIndex().Prev());
}

SessionItem* FindNextItemToSelect(SessionItem* item)
{
  auto next = FindNextSibling(item);
  auto closest = next ? next : FindPreviousSibling(item);
  return closest ? closest : item->GetParent();
}

bool IsItemAncestor(const SessionItem* item, const SessionItem* candidate)
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
    parent = parent->GetParent();
  }
  return false;
}

std::vector<SessionItem*> UniqueItems(const std::vector<SessionItem*>& items)
{
  auto filtered = utils::UniqueWithOrder(items);
  std::vector<SessionItem*> result;
  std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(result),
               [](auto item) { return item != nullptr; });
  return result;
}

int GetNestlingDepth(const SessionItem* basis, const SessionItem* item, int level)
{
  if (!basis || !item)
  {
    return -1;
  }

  SessionItem* root_item = basis->GetModel() ? basis->GetModel()->GetRootItem() : nullptr;

  if (item == basis)
  {
    return level;
  }

  if (item == nullptr || item == root_item)
  {
    return -1;
  }
  return GetNestlingDepth(basis, item->GetParent(), level + 1);
}

bool HasAppearanceFlag(const SessionItem& item, Appearance flag)
{
  return item.HasData(mvvm::DataRole::kAppearance)
             ? item.Data<int>(mvvm::DataRole::kAppearance) & flag
             : false;
}

bool ReplaceData(SessionItem& item, const variant_t& value, int role)
{
  item.SetData(variant_t(), role);   // will remove old variant for given role
  return item.SetData(value, role);  // will succeed
}

}  // namespace mvvm::utils
