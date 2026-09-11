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

#ifndef MVVM_MODEL_ITEM_UTILS_H_
#define MVVM_MODEL_ITEM_UTILS_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace mvvm::utils
{

/**
 * @brief Generates and sets new identifier for a given item.
 */
void SetIdentifier(SessionItem& item);

/**
 * @brief Sets item data for the given role.
 *
 * If the item belongs to the model, it will act through the model. This will invoke a notification
 * mechanism and provide the possibility for undo if implemented.
 *
 * @note This call bypasses possible data strategy on board of SessionItem. Thus, the method shall
 * be used within a custom data strategy implementation.
 *
 * @see SessionItem::SetDataStrategy
 */
bool SetData(SessionItem& item, const variant_t& value, role_t role);

/**
 * @brief Sets item data for the given role.
 *
 * This call bypasses possible data strategy, notifications, and command framework.
 */
bool SetDataImpl(SessionItem& item, const variant_t& value, role_t role);

/**
 * @brief Inserts item into the given tag_index, ownership is taken.
 *
 * This mehtod bypases the model (i.e. no notification and undo/redo).
 *
 * @param to_insert The item to insert.
 * @param item Item which will get new child.
 * @param tag_index A tag_index pointing to the insert place.
 * @return Convenience pointer to just inserted item.
 */
SessionItem* InsertItemImpl(std::unique_ptr<SessionItem> to_insert, SessionItem& item,
                            const TagIndex& tag_index);

/**
 * @brief Removes an item from the given tag_index, returns it to the caller.
 *
 * This mehtod bypases the model (i.e. no notification and undo/redo).
 */
std::unique_ptr<SessionItem> TakeItemImpl(SessionItem& item, const TagIndex& tag_index);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item (non-const version).
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 */
void iterate(SessionItem* item, const std::function<void(SessionItem*)>& func);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item (const version).
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 */
void iterate(const SessionItem* item, const std::function<void(const SessionItem*)>& func);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item (nullptr version).
 */
void iterate(std::nullptr_t, const std::function<void(const SessionItem*)>& func);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item.
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 *
 * @details Will stop iterating when 'func' returns false.
 */
void iterate_if(const SessionItem* item, const std::function<bool(const SessionItem*)>& func);

/**
 * @brief Returns copy number of a given item among all children of given parent.
 *
 * @details Takes into account only items with same type.
 */
std::size_t CopyNumber(const SessionItem* item);

/**
 * @brief Returns child at given index of a parent.
 *
 * #details No tags are involved, index is considered as global index in the combined array of all
 * children.
 */
SessionItem* ChildAt(const SessionItem* parent, std::size_t index);

/**
 * @brief Returns index in children array corresponding to given child.
 *
 * @details No tags are involved, index is considered as global index in the combined array of all
 * children.
 */
std::size_t IndexOfChild(const SessionItem* parent, const SessionItem* child);

/**
 * @brief Returns true if given item has registered tag with the given name.
 */
bool HasTag(const SessionItem& item, const std::string& tag_name);

/**
 * @brief Returns vector of strings containing all registered tags of the given item.
 */
std::vector<std::string> RegisteredTags(const SessionItem& item);

/**
 * @brief Returns vector of all visible children representing top level items.
 *
 * @details A top level item is any item not marked as a property item.
 */
std::vector<SessionItem*> TopLevelItems(const SessionItem& item);

/**
 * @brief Returns vector of all visible children representing property items.
 *
 * @details A property item is an item created with AddProperty method, it can be a leave with a
 * scalar data, moer complex VectorItem, etc. The main thing is that it is parked with kProperty
 * appearance flag.
 */
std::vector<SessionItem*> SinglePropertyItems(const SessionItem& item);

/**
 * @brief Returns next sibling within the same tag.
 */
const SessionItem* FindNextSibling(const SessionItem* item);

/**
 * @brief Returns previous sibling within the same tag.
 */
const SessionItem *FindPreviousSibling(const SessionItem *item);

/**
 * @brief Finds next item to select.
 *
 * Method is used in the context of the next item selection after the given item was deleted. This
 * implementation suggests first selecting the next item in the list. If it is not possible, the
 * previous item will be selected, if exist. As a last resort, the item's parent will be
 * returned.
 *
 * @param item Item which will be deleted soon.
 * @return An item which should be selected instead.
 */
const SessionItem* FindNextItemToSelect(const SessionItem* item);

/**
 * @brief Finds next sibling item to select.
 *
 * Method is used in the context of the next item selection after the given item was deleted. This
 * implementation suggests first selecting the next item in the list. If it is not possible, the
 * previous item will be selected, if exist.
 *
 * @param item Item which will be deleted soon.
 * @return An item which should be selected instead, or nullptr if no selection was found.
 */
const SessionItem* FindNextSiblingToSelect(const SessionItem* item);

/**
 * @brief Returns true if 'ancestor_candidate' is one of ancestor (parent, grand-parent etc) of
 * given item.
 */
bool IsItemAncestor(const SessionItem* item, const SessionItem* ancestor_candidate);

/**
 * @brief Returns vector with duplicates and 'nullptr' filtered out.
 */
std::vector<SessionItem*> UniqueItems(const std::vector<SessionItem*>& items);

/**
 * @brief Returns nesting depth of 'item' with respect to the 'basis'.
 *
 * @param basis A reference item which we use to explore nesting depth.
 * @param item An item those nesting depth we are interesting in.
 * @return Nesting depth.
 *
 * @details Here are examples:
 * result = 0 if `item` == `basis`
 * result = 1 if `item` is a child of `basis`
 * result = 2 if `item` is a grandchild of `basis`
 * result = constants::kInvalidIndex if item is above `basis` or doesn't belong same branch
 */
std::size_t GetNestingDepth(const SessionItem* basis, const SessionItem* item);

/**
 * @brief Returns nesting depth of 'item' with respect to the root item.
 *
 * A root item is any item which doesn't have a parent itself.
 *
 * @param item An item those nesting depth we are interesting in.
 * @return Nesting depth.
 *
 * @details Here are examples:
 * result = 0 if `item` doesn't have parent, or `item` is a root item itself
 * result = 1 if `item` is a child of root item
 */
std::size_t GetNestingDepth(const SessionItem* item);

/**
 * @brief Replace data of given item with provided value for given role, returns true in the case of
 * success.
 *
 * @details This method allow to set the data to the item, even if the type of the data doesn't
 * match the original. For example:
 * item.SetData(42.0, role);
 * item.SetData("abc", role); <-- will fail because we do not allow to switch data
 * ReplaceData(&item, "abc", role); <-- will succeed, new data will be std::string, instead of
 * double
 */
bool ReplaceData(SessionItem& item, const variant_t& value, role_t role);

/**
 * @brief Finds if given item or one of its parents up in the hierarchy can be casted to given type
 * (const version).
 *
 * @param item The item to investigate.
 * @param start_from_self Will start from item itself, if true, otherwise will start from its
 * parent.
 *
 * @return First found item casted to necessary type.
 */
template <typename T>
const T* FindItemUp(const SessionItem* item, bool start_from_self = true)
{
  if (!item)
  {
    return nullptr;
  }

  const SessionItem* current = start_from_self ? item : item->GetParent();

  while (current)
  {
    if (const T* result = dynamic_cast<const T*>(current); result)
    {
      return result;
    }
    current = current->GetParent();
  }
  return nullptr;
}

/**
 * @brief Finds if given item or one of its parents up in the hierarchy can be casted to given type
 * (non-const version).
 *
 * @param item The item to investigate.
 * @param start_from_self Will start from item itself, if true, otherwise will start from its
 * parent.
 *
 * @return First found item casted to necessary type.
 */
template <typename T>
T* FindItemUp(SessionItem* item, bool start_from_self = true)
{
  return const_cast<T*>(FindItemUp<T>(const_cast<const SessionItem*>(item), start_from_self));
}

/**
 * @brief Finds if given item or one of its parents up in the hierarchy can be casted to given type
 * (nullptr specialisation).
 */
template <typename T>
std::nullptr_t FindItemUp(std::nullptr_t, bool start_from_self = true)
{
  (void)start_from_self;
  return nullptr;
}

/**
 * @brief Finds if a given item or one of its children down in the hierarchy can be cast to given
 * type (const version).
 *
 * If a non-empty model_type is given, an additional check will be performed so that the result
 * contains only items with the given type.
 *
 * @tparam The concrete type of the item.
 * @param item The item to investigate.
 * @param model_type Required model type of all casted items.
 * @param start_from_self Will start from item itself, if true, otherwise will start from its
 * children.
 *
 * @return List of found items casted to necessary type.
 */
template <typename T = SessionItem>
std::vector<const T*> FindItemDown(const SessionItem* item, const std::string& model_type = {},
                                   bool start_from_self = true)
{
  if (!item)
  {
    return {};
  }

  std::vector<const T*> result;

  auto on_item = [item, start_from_self, &model_type, &result](const SessionItem* current)
  {
    if (!start_from_self && current == item)
    {
      return;
    }

    if (const T* casted = dynamic_cast<const T*>(current); casted)
    {
      const bool skip_check_for_type = model_type.empty();
      const bool type_is_ok = !model_type.empty() && casted->GetType() == model_type;
      if (skip_check_for_type || type_is_ok)
      {
        result.push_back(casted);
      }
    }
  };
  iterate(item, on_item);

  return result;
}

/**
 * @brief Returns clone of the item (identifiers are preserved).
 *
 * The purpose of this method is to change type of returned unique_ptr to the type of the original
 * item.
 */
template <typename T>
std::unique_ptr<T> CloneItem(const T& item)
{
  if constexpr (std::is_same_v<T, SessionItem>)
  {
    // return cloned object as it is (i.e. unique_ptr<SessionItem>)
    return item.Clone();
  }
  else
  {
    // Converting unique_ptr<SessionItem> to the correct type
    return std::unique_ptr<T>(static_cast<T*>(item.Clone().release()));
  }
}

/**
 * @brief Returns deep copy of the item (identifiers are regenerated).
 */
template <typename T>
std::unique_ptr<T> CopyItem(const T& item)
{
  auto result = CloneItem(item);
  iterate(result.get(), [](SessionItem* child) { SetIdentifier(*child); });
  return result;
}

/**
 * @brief Moves item up (decrements row of the item), returns true in the case of success.
 *
 * Works on children belonging to the same tag.
 */
bool MoveUp(const SessionItem& item);

/**
 * @brief Moves item down (increments row of the item), returns true in the case of success.
 *
 * Works on children belonging to the same tag.
 */
bool MoveDown(const SessionItem& item);

/**
 * @brief Removes item from its parent and destroys it.
 */
void RemoveItem(const SessionItem& item);

/**
 * @brief Removes an existing item at given 'tag_index' and insert a new one in its place.
 */
SessionItem* ReplaceItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                         const TagIndex& tag_index);

/**
 * @brief Converts given item to XML string.
 *
 * @param item The item to copy.
 * @param filter_func If function is defined and returns true will filter out child.
 */
std::string ToXMLString(const SessionItem& item,
                        const std::function<bool(const SessionItem&)>& filter_func = {});

/**
 * @brief Returns SessionItem from its XML representation.
 *
 * @param str A string containing XML representaion.
 * @param make_unique_id Generates new ID during item creation.
 */
std::unique_ptr<SessionItem> SessionItemFromXMLString(const std::string& str,
                                                      bool make_unique_id = true);

/**
 * @brief Begin command macros with the given name.
 *
 * Works only if the item belongs to the model with undo/redo enabled. Otherwise, do nothing.
 */
void BeginMacro(const SessionItem& item, const std::string& macro_name);

/**
 * @brief Finished recording of the command macro.
 *
 * Works only if the item belongs to the model with undo/redo enabled. Otherwise, do nothing.
 */
void EndMacro(const SessionItem& item);

/**
 * @brief Returns string representing item.
 */
std::string ItemToDebugString(const SessionItem* item);

/**
 * @brief Collect all items up in hierarchy that can be casted to a given type.
 *
 * It is used in the context when the user selects the part of the tree and wants to find all
 * parents of certain type. The resulting list will be filtered for duplicates, the order of parents
 * in the list will be preserved.
 *
 * @param item The items to investigate.
 * @param start_from_self Will start from items itself, if true, otherwise will start from its
 * parent.
 *
 * @return All found items casted to necessary type.
 */
template <typename T>
std::vector<T*> FindItemsUp(const std::vector<SessionItem*>& items, bool start_from_self = true)
{
  std::vector<T*> result;
  for (auto item : items)
  {
    if (auto casted_item = FindItemUp<T>(item, start_from_self); casted_item)
    {
      result.push_back(casted_item);
    }
  }
  return UniqueWithOrder(result);
}

/**
 * @brief Checks if given item has notifications.
 */
bool HasNotifications(const SessionItem* item);

/**
 * @brief Returns true when the given item tree contains more than max_item_count items of the
 * given type.
 *
 * The given item itself is taken into account. Items which can't be cast to the given type are not
 * counted.
 *
 * @tparam ItemT The concrete type of items to count.
 * @param item The root of the tree to examine.
 * @param max_item_count The maximum number of items allowed in the tree.
 */
template <typename ItemT = SessionItem>
bool HasMoreItemsThan(const SessionItem& item, std::size_t max_item_count)
{
  std::size_t count{0};

  auto on_item = [&count, max_item_count](const SessionItem* visited)
  {
    if (dynamic_cast<const ItemT*>(visited) != nullptr)
    {
      ++count;
    }
    return count <= max_item_count;  // count only until max_item_count is exceeded
  };

  iterate_if(&item, on_item);

  return count > max_item_count;
}

/**
 * @brief Returns the number of items of the given type in the given item tree.
 *
 * The given item itself is taken into account. Items which can't be cast to the given type are not
 * counted.
 *
 * @tparam ItemT The concrete type of items to count.
 * @param item The root of the tree to examine.
 */
template <typename ItemT = SessionItem>
std::size_t GetItemCount(const SessionItem& item)
{
  std::size_t result{0};

  auto on_item = [&result](const SessionItem* visited)
  {
    if (dynamic_cast<const ItemT*>(visited) != nullptr)
    {
      ++result;
    }
  };

  iterate(&item, on_item);

  return result;
}

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_ITEM_UTILS_H_
