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

#include "item_utils.h"

#include "i_session_model.h"
#include "item_factory.h"
#include "model_utils.h"
#include "process_command.h"
#include "session_item.h"
#include "session_item_container.h"
#include "session_item_data.h"
#include "tagged_items.h"

#include <mvvm/commands/set_value_command.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/core/unique_id_generator.h>
#include <mvvm/serialization/tree_data_item_converter.h>
#include <mvvm/utils/container_utils.h>

#include <sup/xml/tree_data_parser.h>
#include <sup/xml/tree_data_serialize.h>

#include <sstream>
#include <stack>
#include <utility>

namespace mvvm::utils
{

namespace
{
/**
 * @brief Move item to a new index within the same parent. No check is performed for index
 * validity.
 */
void MoveTo(const SessionItem& item, const TagIndex& tag_index)
{
  if (auto model = item.GetModel(); model)
  {
    // FIXME: modify MoveItem API so it accepts const item to move
    model->MoveItem(const_cast<SessionItem*>(&item), item.GetParent(), tag_index);
  }
  else
  {
    auto parent = item.GetParent();
    auto current_tag_index = item.GetTagIndex();
    const TagIndex target_index =
        (tag_index.GetIndex() > current_tag_index.GetIndex()) ? tag_index.Prev() : tag_index;
    auto taken = parent->TakeItem(current_tag_index);
    (void)parent->InsertItem(std::move(taken), target_index);
  }
}

}  // namespace

void SetIdentifier(SessionItem& item)
{
  // we want to bypass any data strategies
  (void)SetDataImpl(item, UniqueIdGenerator::Generate(), DataRole::kIdentifier);
}

bool SetData(SessionItem& item, const variant_t& value, role_t role)
{
  if (item.GetModel() != nullptr)
  {
    return ProcessCommand<SetValueCommand>(*item.GetModel(), &item, value, role);
  }

  return utils::SetDataImpl(item, value, role);
}

bool SetDataImpl(SessionItem& item, const variant_t& value, role_t role)
{
  return item.GetItemData()->SetData(value, role);
}

SessionItem* InsertItemImpl(std::unique_ptr<SessionItem> to_insert, SessionItem& item,
                            const TagIndex& tag_index)
{
  auto result = item.GetTaggedItems()->InsertItem(std::move(to_insert), tag_index);
  result->SetParent(&item);
  result->SetModel(item.GetModel());
  return result;
}

std::unique_ptr<SessionItem> TakeItemImpl(SessionItem& item, const TagIndex& tag_index)
{
  auto taken = item.GetTaggedItems()->TakeItem(tag_index);
  taken->SetParent(nullptr);
  taken->SetModel(nullptr);
  return taken;
}

namespace
{
// Depth-first pre-order traversal shared by the const and non-const iterate() overloads. The
// constness of ItemT propagates through GetAllItems(), so the same body serves both.
template <typename ItemT>
void IterateImpl(ItemT* item, const std::function<void(ItemT*)>& func)
{
  if (item == nullptr)
  {
    return;
  }

  std::stack<ItemT*> stack;
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
}  // namespace

void iterate(SessionItem* item, const std::function<void(SessionItem*)>& func)
{
  IterateImpl(item, func);
}

void iterate(const SessionItem* item, const std::function<void(const SessionItem*)>& func)
{
  IterateImpl(item, func);
}

void iterate(std::nullptr_t, const std::function<void(const SessionItem*)>& func)
{
  (void)func;
}

void iterate_if(const SessionItem* item, const std::function<bool(const SessionItem*)>& func)
{
  if (item == nullptr)
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

std::size_t CopyNumber(const SessionItem* item)
{
  std::size_t result = constants::kInvalidIndex;

  if (item == nullptr)
  {
    return result;
  }

  std::size_t count{0};
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

  return (count > 1) ? result : constants::kInvalidIndex;
}

const SessionItem* ChildAt(const SessionItem* parent, std::size_t index)
{
  if (parent == nullptr)
  {
    return nullptr;
  }

  auto container = parent->GetAllItems();
  return (index < container.size()) ? container[index] : nullptr;
}

SessionItem* ChildAt(SessionItem* parent, std::size_t index)
{
  return const_cast<SessionItem*>(ChildAt(static_cast<const SessionItem*>(parent), index));
}

std::size_t IndexOfChild(const SessionItem* parent, const SessionItem* child)
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

std::vector<const SessionItem*> TopLevelItems(const SessionItem& item)
{
  std::vector<const SessionItem*> result;
  for (const auto* child : item.GetAllItems())
  {
    if (child->IsVisible() && !child->HasFlag(kProperty))
    {
      result.push_back(child);
    }
  }
  return result;
}

std::vector<SessionItem*> TopLevelItems(SessionItem& item)
{
  return RemoveConst(TopLevelItems(std::as_const(item)));
}

std::vector<const SessionItem*> SinglePropertyItems(const SessionItem& item)
{
  std::vector<const SessionItem*> result;
  for (const auto* child : item.GetAllItems())
  {
    if (child->IsVisible() && child->HasFlag(kProperty))
    {
      result.push_back(child);
    }
  }
  return result;
}

std::vector<SessionItem*> SinglePropertyItems(SessionItem& item)
{
  return RemoveConst(SinglePropertyItems(std::as_const(item)));
}

const SessionItem *FindNextSibling(const SessionItem *item)
{
  auto parent = (item != nullptr) ? item->GetParent() : nullptr;
  if (parent == nullptr)
  {
    return nullptr;
  }
  return parent->FindItem(item->GetTagIndex().Next());
}

const SessionItem* FindPreviousSibling(const SessionItem* item)
{
  auto parent = (item != nullptr) ? item->GetParent() : nullptr;
  if (parent == nullptr)
  {
    return nullptr;
  }
  return parent->FindItem(item->GetTagIndex().Prev());
}

const SessionItem *FindNextItemToSelect(const SessionItem *item)
{
  auto closest = FindNextSiblingToSelect(item);
  return (closest != nullptr) ? closest : item->GetParent();
}

const SessionItem *FindNextSiblingToSelect(const SessionItem *item)
{
  auto next = FindNextSibling(item);
  auto closest = (next != nullptr) ? next : FindPreviousSibling(item);
  return (closest != nullptr) ? closest : nullptr;
}

bool IsItemAncestor(const SessionItem* item, const SessionItem* ancestor_candidate)
{
  if ((item == nullptr) || (ancestor_candidate == nullptr))
  {
    return false;
  }
  const SessionItem* parent = item->GetParent();
  while (parent != nullptr)
  {
    if (parent == ancestor_candidate)
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
  (void)std::copy_if(filtered.begin(), filtered.end(), std::back_inserter(result),
                     [](auto item) { return item != nullptr; });
  return result;
}

std::size_t GetNestingDepth(const SessionItem* basis, const SessionItem* item)
{
  if ((basis == nullptr) || (item == nullptr))
  {
    return constants::kInvalidIndex;
  }

  std::size_t level = 0;
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

  return constants::kInvalidIndex;
}

std::size_t GetNestingDepth(const SessionItem* item)
{
  std::size_t level{constants::kInvalidIndex};

  if (item == nullptr)
  {
    return level;
  }

  auto current_item = item;

  while (current_item != nullptr)
  {
    level += 1;
    current_item = current_item->GetParent();
  }

  return level;
}

bool ReplaceData(SessionItem& item, const variant_t& value, role_t role)
{
  if (item.HasData(role))
  {
    (void)item.SetData(variant_t(), role);  // will remove old variant for given role
  }
  return item.SetData(value, role);  // will succeed
}

bool MoveUp(const SessionItem &item)
{
  auto tag_index = item.GetTagIndex();
  if (tag_index.GetIndex() == 0)
  {
    return false;  // item already at the top
  }

  MoveTo(item, tag_index.Prev());
  return true;
}

bool MoveDown(const SessionItem &item)
{
  auto tag_index = item.GetTagIndex();
  if (tag_index.GetIndex() == (item.GetParent()->GetItemCount(tag_index.GetTag()) - 1))
  {
    return false;  // item already at the bottom
  }

  // [a b c d e f]
  // [0 1 2 3 4 5]
  // to shift item "c" to the right we have to use index "4" as move index
  MoveTo(item, tag_index.Next().Next());  // moving index twice
  return true;
}

void RemoveItem(const SessionItem &item)
{
  if (item.GetParent() == nullptr)
  {
    throw LogicErrorException("Item doesn't have a parent");
  }

  (void)item.GetParent()->TakeItem(item.GetTagIndex());
}

SessionItem* ReplaceItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                         const TagIndex& tag_index)
{
  if (parent == nullptr)
  {
    throw LogicErrorException("No parent item provided");
  }

  (void)parent->TakeItem(tag_index);
  return parent->InsertItem(std::move(item), tag_index);
}

std::string ToXMLString(const SessionItem& item,
                        const std::function<bool(const SessionItem&)>& filter_func)
{
  const TreeDataItemConverter converter(&GetGlobalItemFactory(), ConverterMode::kClone,
                                        filter_func);
  auto tree_data = converter.ToTreeData(item);
  return xml::TreeDataToString(*tree_data);
}

std::unique_ptr<SessionItem> SessionItemFromXMLString(const std::string& str, bool make_unique_id)
{
  const TreeDataItemConverter converter(
      &GetGlobalItemFactory(), make_unique_id ? ConverterMode::kCopy : ConverterMode::kClone);
  auto tree_data = xml::TreeDataFromString(str);
  return converter.ToSessionItem(*tree_data);
}

void BeginMacro(const SessionItem& item, const std::string& macro_name)
{
  if (const auto model = item.GetModel(); model)
  {
    BeginMacro(*model, macro_name);
  }
}

void EndMacro(const SessionItem& item)
{
  if (const auto model = item.GetModel(); model)
  {
    EndMacro(*model);
  }
}

std::string ItemToDebugString(const SessionItem* item)
{
  std::ostringstream ostr;
  if (item != nullptr)
  {
    ostr << (item);
    ostr << " " << "[" + item->GetDisplayName() + "]";
    ostr << " " << item->GetIdentifier();
    ostr << " value [" + utils::ValueToString(item->Data()) + "]";
  }
  else
  {
    ostr << "nullptr";
  }
  return ostr.str();
}

bool HasNotifications(const SessionItem* item)
{
  if ((item == nullptr) || (item->GetModel() == nullptr))
  {
    return false;
  }
  return item->GetModel()->GetEventHandler() != nullptr;
}

}  // namespace mvvm::utils
