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

#include "item_utils.h"

#include "session_item.h"
#include "session_item_container.h"
#include "tagged_items.h"
#include "model_utils.h"
#include "i_session_model.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/serialization/treedata_item_converter.h>
#include <mvvm/serialization/xml_parse_utils.h>
#include <mvvm/serialization/xml_write_utils.h>
#include <mvvm/utils/container_utils.h>

#include <stack>
#include <sstream>

namespace
{
/**
 * @brief Move item to a new index within the same parent. No check is performed for index
 * validity.
 */
void MoveTo(mvvm::SessionItem& item, const mvvm::TagIndex& tag_index)
{
  if (auto model = item.GetModel(); model)
  {
    model->MoveItem(&item, item.GetParent(), tag_index);
  }
  else
  {
    auto parent = item.GetParent();
    auto current_tag_index = item.GetTagIndex();
    auto taken = parent->TakeItem(current_tag_index);
    parent->InsertItem(std::move(taken), tag_index);
  }
}

}  // namespace

namespace mvvm::utils
{

void iterate(SessionItem* item, const std::function<void(SessionItem*)>& func)
{
  auto adapter = [&func](const SessionItem* current) { func(const_cast<SessionItem*>(current)); };
  iterate(const_cast<const SessionItem*>(item), adapter);
}

void iterate(const SessionItem* item, const std::function<void(const SessionItem*)>& func)
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
    func(top_item);

    stack.pop();

    auto children = top_item->GetAllItems();
    // push in reverse order to provide correct child order processing
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
      stack.push(*it);
    }
  }
}

void iterate(std::nullptr_t, const std::function<void(const SessionItem*)>& func)
{
  (void)func;
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
  auto closest = FindNextSiblingToSelect(item);
  return closest ? closest : item->GetParent();
}

SessionItem* FindNextSiblingToSelect(SessionItem* item)
{
  auto next = FindNextSibling(item);
  auto closest = next ? next : FindPreviousSibling(item);
  return closest ? closest : nullptr;
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

int GetNestingDepth(const SessionItem* basis, const SessionItem* item)
{
  if (!basis || !item)
  {
    return -1;
  }

  SessionItem* root_item = basis->GetModel() ? basis->GetModel()->GetRootItem() : nullptr;

  int level = 0;
  auto current_item = item;

  while (current_item != nullptr)
  {
    if (current_item == basis)
    {
      return level;
    }

    level += 1;
    current_item = current_item->GetParent();
  }

  return -1;
}

bool HasAppearanceFlag(const SessionItem& item, Appearance flag)
{
  return item.HasData(DataRole::kAppearance) ? item.Data<int>(DataRole::kAppearance) & flag : false;
}

bool ReplaceData(SessionItem& item, const variant_t& value, int role)
{
  item.SetData(variant_t(), role);   // will remove old variant for given role
  return item.SetData(value, role);  // will succeed
}

bool MoveUp(SessionItem& item)
{
  auto tag_index = item.GetTagIndex();
  if (tag_index.index == 0)
  {
    return false;  // item already at the top
  }

  MoveTo(item, tag_index.Prev());
  return true;
}

bool MoveDown(SessionItem& item)
{
  auto tag_index = item.GetTagIndex();
  if (tag_index.index == item.GetParent()->GetItemCount(tag_index.tag) - 1)
  {
    return false;  // item already at the buttom
  }

  MoveTo(item, tag_index.Next());
  return true;
}

void RemoveItem(SessionItem& item)
{
  if (!item.GetParent())
  {
    throw LogicErrorException("Item doesn't have a parent");
  }

  if (auto model = item.GetModel(); model)
  {
    model->RemoveItem(&item);
  }
  else
  {
    item.GetParent()->TakeItem(item.GetParent()->TagIndexOfItem(&item));
  }
}

SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                        const TagIndex& tag_index)
{
  if (auto model = parent->GetModel(); model)
  {
    return model->InsertItem(std::move(item), parent, tag_index);
  }
  return parent->InsertItem(std::move(item), tag_index);
}

SessionItem* ReplaceItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                         const TagIndex& tag_index)
{
  if (parent == nullptr)
  {
    throw LogicErrorException("No parent item provided");
  }

  if (auto model = parent->GetModel(); model)
  {
    model->TakeItem(parent, tag_index);
    return model->InsertItem(std::move(item), parent, tag_index);
  }

  parent->TakeItem(tag_index);
  return parent->InsertItem(std::move(item), tag_index);
}

std::string ToXMLString(const SessionItem& item)
{
  TreeDataItemConverter converter(&GetGlobalItemFactory(), ConverterMode::kClone);
  auto tree_data = converter.ToTreeData(item);
  return GetXMLString(*tree_data);
}

std::unique_ptr<SessionItem> SessionItemFromXMLString(const std::string& str, bool make_unique_id)
{
  TreeDataItemConverter converter(&GetGlobalItemFactory(),
                                  make_unique_id ? ConverterMode::kCopy : ConverterMode::kClone);
  auto tree_data = ParseXMLDataString(str);
  return converter.ToSessionItem(*tree_data);
}

void BeginMacro(const SessionItem &item, const std::string &macro_name)
{
  if (const auto model = item.GetModel(); model)
  {
    BeginMacro(*model, macro_name);
  }
}

void EndMacro(const SessionItem &item)
{
  if (const auto model = item.GetModel(); model)
  {
    EndMacro(*model);
  }
}

std::string ItemToDebugString(const SessionItem *item)
{
  std::ostringstream ostr;
  if (item)
  {
    ostr << (item);
    ostr << " " << "[" + item->GetDisplayName() + "]";
    ostr << " value [" + utils::ValueToString(item->Data()) + "]";
  }
  else
  {
    ostr << "nullptr";
  }
  return ostr.str();
}

}  // namespace mvvm::utils
