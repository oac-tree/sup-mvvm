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

#ifndef MVVM_MODEL_ITEM_UTILS_H_
#define MVVM_MODEL_ITEM_UTILS_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model_export.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace mvvm::utils
{

/**
 * @brief Iterates through all items down through the whole item hierarchy and calls a user function
 * for every item.
 *
 * @param item Item to start iterating.
 * @param func User function to call.
 */
MVVM_MODEL_EXPORT void iterate(SessionItem* item, const std::function<void(SessionItem*)>& fun);

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
 * @details Method is used in the context of next item selection after given item was deleted. In
 * this implementation we suggest to select next item in the list, if possible.
 */
MVVM_MODEL_EXPORT SessionItem* FindNextItemToSelect(SessionItem* item);

/**
 * @brief Returns true if 'candidate' is one of ancestor (parent, grand-parent etc) of given item.
 */
MVVM_MODEL_EXPORT bool IsItemAncestor(const SessionItem* item, const SessionItem* candidate);

/**
 * @brief Returns vector with duplicates and 'nullptr' filtered out.
 */
MVVM_MODEL_EXPORT std::vector<SessionItem*> UniqueItems(const std::vector<SessionItem*>& items);

/**
 * @brief Returns nestling depth of 'item' with respect to the 'basis'.
 *
 * @param basis A reference item which we use to explore nestling depth.
 * @param item An item those nestling depth we are interesting in.
 *
 * @return Nestling depth
 *
 * @details Here are examples:
 * result = 0 if `item` == `basis`
 * result = 1 if `item` is a child of `basis`
 * result = 2 if `item` is a grandchild of `basis`
 * result = -1 if item is above `basis` or doesn't belong same branch
 */
MVVM_MODEL_EXPORT int GetNestlingDepth(const SessionItem* basis, const SessionItem* item);

//! Returns true if given item has appearance flag set.
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
 * @brief Finds first parent item of given type. Returns nullptr if no parent of given type found.
 */
template <typename T>
SessionItem* FindParent(const SessionItem* item)
{
  if (item == nullptr)
  {
    return nullptr;
  }

  auto parent = item->GetParent();
  while (parent != nullptr && dynamic_cast<T*>(parent) == nullptr)
  {
    parent = parent->GetParent();
  }

  return parent;
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

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_ITEM_UTILS_H_
