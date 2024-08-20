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

#ifndef MVVM_MODEL_ITEM_UTILS_H_
#define MVVM_MODEL_ITEM_UTILS_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model_export.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace mvvm::utils
{

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item (non-const version).
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 */
MVVM_MODEL_EXPORT void iterate(SessionItem* item, const std::function<void(SessionItem*)>& func);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item (const version).
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 */
MVVM_MODEL_EXPORT void iterate(const SessionItem* item,
                               const std::function<void(const SessionItem*)>& func);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item (nullptr version).
 */
MVVM_MODEL_EXPORT void iterate(std::nullptr_t, const std::function<void(const SessionItem*)>& func);

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item.
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 *
 * @details Will stop iterating when 'func' returns false.
 */
MVVM_MODEL_EXPORT void iterate_if(const SessionItem* item,
                                  const std::function<bool(const SessionItem*)>& func);

/**
 * @brief Returns copy number of a given item among all children of given parent.
 *
 * @details Takes into account only items with same type.
 */
MVVM_MODEL_EXPORT int CopyNumber(const SessionItem* item);

/**
 * @brief Returns child at given index of a parent.
 *
 * #details No tags are involved, index is considered as global index in the combined array of all
 * children.
 */
MVVM_MODEL_EXPORT SessionItem* ChildAt(const SessionItem* parent, int index);

/**
 * @brief Returns index in children array corresponding to given child.
 *
 * @details No tags are involved, index is considered as global index in the combined array of all
 * children.
 */
MVVM_MODEL_EXPORT int IndexOfChild(const SessionItem* parent, const SessionItem* child);

/**
 * @brief Returns true if given item has registered tag with the given name.
 */
MVVM_MODEL_EXPORT bool HasTag(const SessionItem& item, const std::string& tag_name);

/**
 * @brief Returns vector of strings containing all registered tags of the given item.
 */
MVVM_MODEL_EXPORT std::vector<std::string> RegisteredTags(const SessionItem& item);

/**
 * @brief Returns vector of all visible children representing top level items.
 *
 * @details A top level item is any item not marked as a property item.
 */
MVVM_MODEL_EXPORT std::vector<SessionItem*> TopLevelItems(const SessionItem& item);

/**
 * @brief Returns vector of all visible children representing property items.
 *
 * @details A property item is an item created with AddProperty method, it can be a leave with a
 * scalar data, moer complex VectorItem, etc. The main thing is that it is parked with kProperty
 * appearance flag.
 */
MVVM_MODEL_EXPORT std::vector<SessionItem*> SinglePropertyItems(const SessionItem& item);

/**
 * @brief Returns next sibling within the same tag.
 */
MVVM_MODEL_EXPORT SessionItem* FindNextSibling(SessionItem* item);

/**
 * @brief Returns previous sibling within the same tag.
 */
MVVM_MODEL_EXPORT SessionItem* FindPreviousSibling(SessionItem* item);

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
MVVM_MODEL_EXPORT SessionItem* FindNextItemToSelect(SessionItem* item);

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
MVVM_MODEL_EXPORT SessionItem* FindNextSiblingToSelect(SessionItem* item);

/**
 * @brief Returns true if 'candidate' is one of ancestor (parent, grand-parent etc) of given item.
 */
MVVM_MODEL_EXPORT bool IsItemAncestor(const SessionItem* item, const SessionItem* candidate);

/**
 * @brief Returns vector with duplicates and 'nullptr' filtered out.
 */
MVVM_MODEL_EXPORT std::vector<SessionItem*> UniqueItems(const std::vector<SessionItem*>& items);

/**
 * @brief Returns nesting depth of 'item' with respect to the 'basis'.
 *
 * @param basis A reference item which we use to explore nesting depth.
 * @param item An item those nesting depth we are interesting in.
 *
 * @return Nesting depth
 *
 * @details Here are examples:
 * result = 0 if `item` == `basis`
 * result = 1 if `item` is a child of `basis`
 * result = 2 if `item` is a grandchild of `basis`
 * result = -1 if item is above `basis` or doesn't belong same branch
 */
MVVM_MODEL_EXPORT int GetNestingDepth(const SessionItem* basis, const SessionItem* item);

/**
 * @brief Checks if given item has appearance flag set.
 */
MVVM_MODEL_EXPORT bool HasAppearanceFlag(const SessionItem& item, Appearance flag);

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
MVVM_MODEL_EXPORT bool ReplaceData(SessionItem& item, const variant_t& value, int role);

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
 * @brief Returns deep copy or clone of the item.
 *
 * @param item Item to clone
 * @param make_unique_id Flag requesting identifier regeneration
 *
 * @return New item casted to necessary type.
 *
 * @details If 'make_unique_id' is true, identifiers of the item and all its children will be
 * regenerated. The item will be unique it and will allow its usage (serialization, memory pool)
 * along with the original. If 'make_unique_id' is false, the result will be an exact clone of the
 * original.
 */
template <typename T>
std::unique_ptr<T> CreateDeepCopy(const T& item, bool make_unique_id)
{
  if constexpr (std::is_same_v<T, SessionItem>)
  {
    // return cloned object as it is (i.e. unique_ptr<SessionItem>)
    return item.Clone(make_unique_id);
  }
  else
  {
    // Converting unique_ptr<SessionItem> to the correct type
    return std::unique_ptr<T>(static_cast<T*>(item.Clone(make_unique_id).release()));
  }
}

/**
 * @brief Returns clone of the item (identifiers are preserved).
 */
template <typename T>
std::unique_ptr<T> CloneItem(const T& item)
{
  return CreateDeepCopy(item, /* make_unique_id */ false);
}

/**
 * @brief Returns deep copy of the item (identifiers are regenerated).
 */
template <typename T>
std::unique_ptr<T> CopyItem(const T& item)
{
  return CreateDeepCopy(item, /* make_unique_id */ true);
}

/**
 * @brief Moves item up (decrements row of the item), returns true in the case of success.
 *
 * Works on children belonging to the same tag.
 */
MVVM_MODEL_EXPORT bool MoveUp(SessionItem& item);

/**
 * @brief Moves item down (increments row of the item), returns true in the case of success.
 *
 * Works on children belonging to the same tag.
 */
MVVM_MODEL_EXPORT bool MoveDown(SessionItem& item);

/**
 * @brief Removes item from its parent and destroys it.
 *
 * @details If item belongs to the model, will remove an item using the model API. This will
 * invoke notification mechanism, and undo/redo, if the model is equipped with it. If item doesn't
 * belong to the model, will act via SessionItem API.
 */
void RemoveItem(SessionItem& item);

/**
 * @brief Insert item into a parent at given 'tag_index'
 *
 * @details If parent belongs to the model, will insert an item using the model API. This will
 * invoke notification mechanism, and undo/redo, if the model is equipped with it. If parent doesn't
 * belong to the model, will act via SessionItem API.
 */
SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                        const TagIndex& tag_index);

/**
 * @brief remove an existing item at given 'tag_index' and insert a new one in its place
 *
 * @details If parent belongs to a model, will insert an item using the model API. This will
 * invoke notification mechanism, if the model is equipped with it. If parent doesn't
 * belong to a model, will act via SessionItem API.
 */
SessionItem* ReplaceItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                         const TagIndex& tag_index);

/**
 * @brief Converts given item to XML string.
 */
std::string ToXMLString(const SessionItem& item);

/**
 * @brief Returns SessionItem from its XML representation.
 *
 * @param str A string containing XML representaion.
 * @param make_unique_id Generates new ID during item creation.
 */
std::unique_ptr<SessionItem> SessionItemFromXMLString(const std::string& str,
                                                      bool make_unique_id = true);

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_ITEM_UTILS_H_
