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

#ifndef MVVM_MODEL_SESSIONITEM_H
#define MVVM_MODEL_SESSIONITEM_H

#include "mvvm/core/variant.h"
#include "mvvm/model/mvvm_types.h"
#include "mvvm/model/tagindex.h"

#include <memory>
#include <stdexcept>
#include <vector>

namespace ModelView
{
class SessionModel;
class TagInfo;
class SessionItemData;
class TaggedItems;

//! The main object representing an editable/displayable/serializable entity. Serves as a
//! construction element (node) of SessionModel to represent all the data of GUI application.

class MVVM_MODEL_EXPORT SessionItem
{
public:
  static inline const std::string Type = "SessionItem";

  SessionItem();
  virtual ~SessionItem();

  SessionItem(const SessionItem&) = delete;
  SessionItem& operator=(const SessionItem&) = delete;

  // basic item properties

  std::string GetType() const;

  std::string GetIdentifier() const;

  virtual std::string GetDisplayName() const;

  virtual SessionItem* SetDisplayName(const std::string& name);

  SessionModel* GetModel() const;

  SessionItem* GetParent() const;

  TagIndex GetTagIndex() const;

  // methods to deal with item data

  bool HasData(int role = DataRole::kData) const;

  template <typename T = variant_t>
  T Data(int role = DataRole::kData) const;

  template <typename T>
  bool SetData(const T& value, int role = DataRole::kData);

  SessionItemData* itemData();
  const SessionItemData* itemData() const;

  // children access

  int childrenCount() const;

  std::vector<SessionItem*> children() const;

  int GetItemCount(const std::string& tag) const;

  SessionItem* getItem(const std::string& tag, int index = 0) const;

  std::vector<SessionItem*> getItems(const std::string& tag) const;

  template <typename T>
  T* item(const std::string& tag) const;
  template <typename T = SessionItem>
  std::vector<T*> items(const std::string& tag) const;

  TagIndex TagIndexOfItem(const SessionItem* item) const;

  void RegisterTag(const TagInfo& tagInfo, bool set_as_default = false);

  TaggedItems* itemTags();
  const TaggedItems* itemTags() const;

  // item manipulation

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index);
  template <typename T = SessionItem>
  T* InsertItem(const TagIndex& tag_index);

  std::unique_ptr<SessionItem> TakeItem(const TagIndex& tag_index);

  // more convenience methods

  bool IsEditable() const;
  SessionItem* SetEditable(bool value);

  bool IsEnabled() const;
  SessionItem* SetEnabled(bool value);

  bool IsVisible() const;
  SessionItem* SetVisible(bool value);

  std::string GetToolTip() const;
  SessionItem* SetToolTip(const std::string& tooltip);

protected:
  explicit SessionItem(const std::string& item_type);

private:
  friend class SessionModel;
  friend class TreeDataItemConverter;
  bool set_data_internal(const variant_t& value, int role);
  variant_t data_internal(int role) const;
  void setParent(SessionItem* parent);
  void setModel(SessionModel* model);
  void setAppearanceFlag(int flag, bool value);

  void setDataAndTags(std::unique_ptr<SessionItemData> data, std::unique_ptr<TaggedItems> tags);

  struct SessionItemImpl;
  std::unique_ptr<SessionItemImpl> p_impl;
};

//! Sets data for a given role.

template <typename T>
inline bool SessionItem::SetData(const T& value, int role)
{
  return set_data_internal(value, role);
}

//! Returns data of given type T for given role.

template <typename T>
inline T SessionItem::Data(int role) const
{
  if constexpr (std::is_same_v<T, variant_t>)
    return data_internal(role);
  else
    return std::get<T>(data_internal(role));
}

//! Returns first item under given tag casted to a specified type.
//! Returns nullptr, if item doesn't exist. If item exists but can't be casted will throw.

template <typename T>
inline T* SessionItem::item(const std::string& tag) const
{
  if (auto item = getItem(tag); item)
  {
    T* tag_item = dynamic_cast<T*>(item);
    if (!tag_item)
      throw std::runtime_error("Can't cast an item to given type");
    return tag_item;
  }
  return nullptr;
}

//! Returns all items under given tag casted to specific type.

template <typename T>
std::vector<T*> SessionItem::items(const std::string& tag) const
{
  std::vector<T*> result;
  for (auto item : getItems(tag))
    if (auto casted = dynamic_cast<T*>(item); casted)
      result.push_back(casted);
  return result;
}

//! Creates a new item and insert it into given tag under the given row.
//! Returns pointer to inserted item to the user.

template <typename T>
inline T* SessionItem::InsertItem(const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), tag_index));
}

}  // namespace ModelView

#endif  // MVVM_MODEL_SESSIONITEM_H
