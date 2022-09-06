/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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

#include <mvvm/model/mvvm_types.h>
#include <mvvm/model_export.h>
#include <mvvm/core/variant.h>

#include <functional>
#include <string>
#include <vector>

namespace mvvm
{
class SessionItem;
}

namespace mvvm::utils
{
//! Iterates through item and all its children.
MVVM_MODEL_EXPORT void iterate(SessionItem* item, const std::function<void(SessionItem*)>& fun);

//! Iterates through all model indices and calls user function.
//! If function returns false for given index, iteration will not go down to children.
MVVM_MODEL_EXPORT void iterate_if(const SessionItem* item,
                                  const std::function<bool(const SessionItem*)>& fun);

//! Returns copy number of a given item in its parent hierarchy. Takes into account only items with
//! same type.
MVVM_MODEL_EXPORT int CopyNumber(const SessionItem* item);

//! Returns child at given index of parent. No tags are involved, index is considered
//! as global index in the combined array of all children.
MVVM_MODEL_EXPORT SessionItem* ChildAt(const SessionItem* parent, int index);

//! Returns index in children array corresponding to given child. No tags are involved,
//! index is considered as global index in the combined array of all children.
MVVM_MODEL_EXPORT int IndexOfChild(const SessionItem* parent, const SessionItem* child);

//! Returns true if given item has registered tag.
MVVM_MODEL_EXPORT bool HasTag(const SessionItem& item, const std::string& tag);

//! Returns true if given item has registered `tag`, and it belongs to single property.
MVVM_MODEL_EXPORT bool IsSinglePropertyTag(const SessionItem& item, const std::string& tag);

//! Returns vector of strings containing all registered tags of the given item.
MVVM_MODEL_EXPORT std::vector<std::string> RegisteredTags(const SessionItem& item);

//! Returns vector of all visible children representing top level items.
MVVM_MODEL_EXPORT std::vector<SessionItem*> TopLevelItems(const SessionItem& item);

//! Returns vector of all visible children representing property items.
MVVM_MODEL_EXPORT std::vector<SessionItem*> SinglePropertyItems(const SessionItem& item);

//! Returns next sibling with same tag.
MVVM_MODEL_EXPORT SessionItem* FindNextSibling(SessionItem* item);

//! Returns previous sibling with same tag.
MVVM_MODEL_EXPORT SessionItem* FindPreviousSibling(SessionItem* item);

//! Finds next item to select
//! Method is used in the context of next item selection after given item was deleted.
MVVM_MODEL_EXPORT SessionItem* FindNextItemToSelect(SessionItem* item);

//! Returns true if 'candidate' is one of ancestor of given item.
MVVM_MODEL_EXPORT bool IsItemAncestor(const SessionItem* item, const SessionItem* candidate);

//! Returns vector with duplicates and 'nullptr' filtered out.
MVVM_MODEL_EXPORT std::vector<SessionItem*> UniqueItems(const std::vector<SessionItem*>& items);

//! Returns nestling depth of `item` with respect to the `basis`.
//! result = 0 if `item` == `basis`
//! result = 1 if `item` is a child of `basis`
//! result = 2 if `item` is a grandchild of `basis`
//! result = -1 if item is above `basis` or doesn't belong same branch
MVVM_MODEL_EXPORT int GetNestlingDepth(const SessionItem* basis, const SessionItem* item,
                                       int level = 0);

//! Returns true if given item has appearance flag set.
MVVM_MODEL_EXPORT bool HasAppearanceFlag(const SessionItem& item, Appearance flag);

//! Replace data of given item with provided value for given role.
//! This method allow to set the data to the item, even if the type of the data doesn't match
//! the original. For example
//! item.SetData(42.0, role);
//! item.SetData("abc", role); <-- will fail because we do not allow to switch data type
//! ReplaceData(&item, "abc", role) <-- will succeed, new data will be std::string, instead of double
MVVM_MODEL_EXPORT bool ReplaceData(SessionItem& item, const variant_t& value, int role);

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_ITEM_UTILS_H_
