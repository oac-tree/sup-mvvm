// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_ITEMUTILS_H
#define MVVM_MODEL_ITEMUTILS_H

#include <functional>
#include <string>
#include <vector>

namespace ModelView
{
class SessionItem;

namespace Utils
{
//! Iterates through item and all its children.
void iterate(SessionItem* item, const std::function<void(SessionItem*)>& fun);

//! Iterates through all model indices and calls user function.
//! If function returns false for given index, iteration will not go down to children.
void iterate_if(const SessionItem* item, const std::function<bool(const SessionItem*)>& fun);

//! Returns copy number of given item in it's parent hierarchy. Takes into account only items with
//! same modelType.
int CopyNumber(const SessionItem* item);

//! Returns child at given index of parent. No tags are involved, index is considered
//! as global index in the combined array of all children.
SessionItem* ChildAt(const SessionItem* parent, int index);

//! Returns index in children array corresponding to given child. No tags are involved,
//! index is considered as global index in the combined array of all children.
int IndexOfChild(const SessionItem* parent, const SessionItem* child);

//! Returns true if given item has registered tag.
bool HasTag(const SessionItem& item, const std::string& tag);

//! Returns true if given item has registered `tag`, and it belongs to single property.
bool IsSinglePropertyTag(const SessionItem& item, const std::string& tag);

//! Returns vector of strings containing all registered tags of the given item.
std::vector<std::string> RegisteredTags(const SessionItem& item);

//! Returns vector of strings containing all registered universal tags of the given item.
//! A universal tag is a tag that is usually empty after item construction and serves for later
//! children's insertion.
std::vector<std::string> RegisteredUniversalTags(const SessionItem& item);

//! Returns vector of all visible children representing top level items.
std::vector<SessionItem*> TopLevelItems(const SessionItem& item);

//! Returns vector of all visible children representing property items.
std::vector<SessionItem*> SinglePropertyItems(const SessionItem& item);

//! Returns next sibling with same tag.
SessionItem* FindNextSibling(SessionItem* item);

//! Returns previous sibling with same tag.
SessionItem* FindPreviousSibling(SessionItem* item);

//! Finds next item to select
//! Method is used in the context of next item selection after given item was deleted.
SessionItem* FindNextItemToSelect(SessionItem* item);

//! Returns true if 'candidate' is one of ancestor of given item.
bool IsItemAncestor(const SessionItem* item, const SessionItem* candidate);

//! Returns vector with duplicates and 'nullptr' filtered out.
std::vector<SessionItem*> UniqueItems(const std::vector<SessionItem*>& items);

//! Returns vector of items casted to given type.
template <typename T>
std::vector<T*> CastedItems(const std::vector<SessionItem*>& items)
{
  std::vector<T*> result;
  for (auto item : items)
    if (auto casted_item = dynamic_cast<T*>(item); casted_item)
      result.push_back(casted_item);

  return result;
}

}  // namespace Utils

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMUTILS_H
