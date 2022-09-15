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

#ifndef MVVM_MODEL_SESSIONITEM_H_
#define MVVM_MODEL_SESSIONITEM_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/mvvm_types.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/utils/container_utils.h>

#include <memory>
#include <stdexcept>
#include <vector>

namespace mvvm
{
class SessionModelInterface;
class TagInfo;
class SessionItemData;
class TaggedItems;

//! A base element to build a hierarchical structure representing all the data of the application
//! running. SessionItem can contain an arbitrary amount of basic data types, and can be a parent
//! for other SessionItems.

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

  SessionModelInterface* GetModel() const;

  SessionItem* GetParent() const;

  TagIndex GetTagIndex() const;

  // methods to deal with item data

  bool HasData(int role = DataRole::kData) const;

  template <typename T = variant_t>
  T Data(int role = DataRole::kData) const;

  template <typename T>
  bool SetData(const T& value, int role = DataRole::kData, bool direct = false);
  bool SetData(const char* value, int role = DataRole::kData, bool direct = false);

  SessionItemData* GetItemData();
  const SessionItemData* GetItemData() const;

  // children access

  int GetTotalItemCount() const;

  std::vector<SessionItem*> GetAllItems() const;

  int GetItemCount(const std::string& tag) const;

  SessionItem* GetItem(const std::string& tag, int index = 0) const;

  std::vector<SessionItem*> GetItems(const std::string& tag) const;

  template <typename T>
  T* GetItem(const std::string& tag, int index = 0) const;
  template <typename T = SessionItem>
  std::vector<T*> GetItems(const std::string& tag) const;

  TagIndex TagIndexOfItem(const SessionItem* item) const;

  void RegisterTag(const TagInfo& tagInfo, bool set_as_default = false);

  TaggedItems* GetTaggedItems();
  const TaggedItems* GetTaggedItems() const;

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

  std::string GetEditorType() const;
  SessionItem* SetEditorType(const std::string& editor_type);

  bool HasFlag(Appearance flag) const;
  SessionItem*  SetFlag(Appearance flag, bool value);

  virtual void Activate();

  void SetModel(SessionModelInterface* model);

protected:
  explicit SessionItem(const std::string& item_type);

private:
  friend class TreeDataItemConverter;

  bool SetDataInternal(const variant_t& value, int role, bool direct);
  variant_t DataInternal(int role) const;

  void SetParent(SessionItem* parent);

  void SetDataAndTags(std::unique_ptr<SessionItemData> data, std::unique_ptr<TaggedItems> tags);

  struct SessionItemImpl;
  std::unique_ptr<SessionItemImpl> p_impl;
};

//! Sets data for a given role. When extra parameter `direct` is false (default case), will act
//! through the model to invoke notifications/undo-redo (if implemented).

template <typename T>
inline bool SessionItem::SetData(const T& value, int role, bool direct)
{
  return SetDataInternal(value, role, direct);
}

//! Returns data of given type T for given role.

template <typename T>
inline T SessionItem::Data(int role) const
{
  if constexpr (std::is_same_v<T, variant_t>)
  {
    return DataInternal(role);  // if variant_it is required, simply return it
  }
  else
  {
    return std::get<T>(DataInternal(role));
  }
}

//! Returns item under given tag and index casted to a specified type.
//! Returns nullptr, if item doesn't exist. If item exists but can't be casted will throw.

template <typename T>
inline T* SessionItem::GetItem(const std::string& tag, int index) const
{
  if (auto item = GetItem(tag, index); item)
  {
    T* tag_item = dynamic_cast<T*>(item);
    if (!tag_item)
    {
      throw std::runtime_error("Can't cast an item to given type");
    }
    return tag_item;
  }
  return nullptr;
}

//! Returns all items under given tag casted to specific type.

template <typename T>
std::vector<T*> SessionItem::GetItems(const std::string& tag) const
{
  return utils::CastItems<T>(GetItems(tag));
}

//! Creates a new item and insert it into given tag under the given row.
//! Returns pointer to inserted item to the user.

template <typename T>
inline T* SessionItem::InsertItem(const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), tag_index));
}

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSIONITEM_H_
