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

#ifndef MVVM_MODEL_SESSION_ITEM_H_
#define MVVM_MODEL_SESSION_ITEM_H_

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/core/variant.h>
#include <mvvm/model/item_traits.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/signals/signal_slot_fwd.h>
#include <mvvm/utils/container_utils.h>

#include <functional>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace mvvm
{

class ISessionModel;
class TagInfo;
class SessionItemData;
class TaggedItems;
class SessionItemImpl;

/**
 * @brief The SessionItem class is a base element to build a hierarchical structure representing all
 * the data of the application.
 *
 * It can contain an arbitrary amount of basic data types and can be a parent for other
 * SessionItems.
 */
class SessionItem
{
public:
  using data_strategy_t = std::function<bool(SessionItem*, const variant_t& value, role_t role)>;

  SessionItem();
  virtual ~SessionItem();

  SessionItem(const SessionItem& other) = delete;
  SessionItem(SessionItem&& other) = delete;
  SessionItem& operator=(const SessionItem& other) = delete;
  SessionItem& operator=(SessionItem&&) = delete;

  /**
   * @brief Creates a clone of the item.
   *
   * Creates an exact clone of the item together with all its children. All item identifiers will be
   * the same. Please note, that cloned items can't be used along with the original item in the same
   * model due to memory pool identifiers collision. Use utils::CopyItem to create deep copies that
   * have different item identifiers.
   */
  virtual std::unique_ptr<SessionItem> Clone() const;

  /**
   * @brief Returns the type of the item.
   */
  std::string GetType() const;

  /**
   * @brief Returns item unique identifier.
   */
  std::string GetIdentifier() const;

  /**
   * @brief Returns item's display name.
   */
  virtual std::string GetDisplayName() const;

  /**
   * @brief Sets the display name.
   */
  SessionItem& SetDisplayName(const std::string& name) &;

  /**
   * @brief Returns the model to which given item belongs to, or nullptr if item doesn't belong to a
   * model.
   */
  const ISessionModel* GetModel() const;
  ISessionModel* GetModel();

  /**
   * @brief Returns item parent.
   */
  const SessionItem* GetParent() const;
  SessionItem* GetParent();

  /**
   * @brief  Returns TagIndex of this item under which it is accessible through its parent.
   */
  TagIndex GetTagIndex() const;

  /**
   * @brief Checks if the item has data on board with the given role.
   */
  bool HasData(role_t role = DataRole::kData) const;

  /**
   * @brief Returns data for the given role.
   *
   * If given role doesn't exist, will return uninitialized variant_t{}. If role exists, will
   * perform conversion to the given T. Will throw, if conversion is not possible.
   *
   * @param role The role of the data.
   * @tparam T The type to convert the data.
   *
   * @return The variant with the data, or data itself if template parameter is specified.
   */
  template <typename T = variant_t>
  T Data(role_t role = DataRole::kData) const;

  /**
   * @brief Returns data stored for the given role on board of implementationm.
   *
   * Method invented to hide implementaiton details and avoid placing SessionItemData header into
   * SessionItem header.
   */
  variant_t GetDataImpl(role_t role) const;

  /**
   * @brief Sets the data for the given role.
   *
   * If the item belongs to the model, it will act through the model. This will invoke a
   * notification mechanism and provide the possibility for undo if implemented.
   *
   * If the data is the same as before, will return false as a sign that no data was changed. It is
   * not possible to change the data type for a given role, once the role was set for the first
   * time. Thus, an attempt to set an integer to the role containing a string will lead to the
   * exception.
   *
   * @param value The value to set.
   * @param role The role of the data.
   *
   * @return Returns true, if the data was changed.
   */
  template <typename T>
  bool SetData(const T& value, role_t role = DataRole::kData);

  /**
   * @brief Specialized version for const char to avoid its conversion to bool.
   */
  bool SetData(const char* value, role_t role = DataRole::kData);

  /**
   * @brief Sets the data for the given role.
   *
   * It is the main method that passes through the data strategy, command framework, and
   * notifications. It is called from all templated SessionItem::SetData methods.
   */
  bool SetDataInternal(const variant_t& value, role_t role);

  /**
   * @brief Returns total number of children in all tags.
   */
  std::size_t GetTotalItemCount() const;

  /**
   * @brief Returns vector of children formed from all children from all tags (const version).
   */
  std::vector<const SessionItem*> GetAllItems() const;

  /**
   * @brief Returns vector of children formed from all children from all tags (non-const version).
   */
  std::vector<SessionItem*> GetAllItems();

  /**
   * @brief Returns number of items in given tag.
   *
   * Will throw, when provided non-empty tag wasn't registered. If the tag name is empty, the tag
   * registered as default will be used.
   */
  std::size_t GetItemCount(std::string_view tag) const;

  /**
   * @brief Returns item located at given tag_index, or nullptr if an item doesn't exist (const
   * version).
   *
   * Never throws. Returns nullptr if the tag wasn't registered, or if no item exists at the given
   * index. If the tag name is empty, the tag registered as default will be used.
   */
  const SessionItem* FindItem(const TagIndex& tag_index) const;

  /**
   * @brief Returns item located at given tag_index, or nullptr if an item doesn't exist (non-const
   * version).
   */
  SessionItem* FindItem(const TagIndex& tag_index);

  /**
   * @brief Returns vector of items in the container with given tag name (const version).
   *
   * Will throw, when provided non-empty tag wasn't registered. If the tag name is empty, container
   * registered as default will be used.
   */
  std::vector<const SessionItem*> GetItems(std::string_view tag) const;

  /**
   * @brief Returns vector of items in the container with given tag name (non-const version).
   */
  std::vector<SessionItem*> GetItems(std::string_view tag);

  /**
   * @brief Returns a reference to the item under given tag and index casted to a specified type
   * (const version).
   *
   * Will throw, if the tag wasn't registered, if no item exists at the given index, or if the item
   * can't be casted to the given type.
   *
   * @see FindItem for a non-throwing, nullable alternative.
   */
  template <typename ItemT = SessionItem>
  const ItemT& GetItem(const TagIndex& tag_index) const;

  /**
   * @brief Returns a reference to the item under given tag and index casted to a specified type
   * (non-const version).
   */
  template <typename ItemT = SessionItem>
  ItemT& GetItem(const TagIndex& tag_index);

  /**
   * @brief Convenience overload addressing the first item in the given tag (const version).
   */
  template <typename ItemT = SessionItem>
  const ItemT& GetItem(std::string_view tag) const;

  /**
   * @brief Convenience overload addressing the first item in the given tag (non-const version).
   */
  template <typename ItemT = SessionItem>
  ItemT& GetItem(std::string_view tag);

  /**
   * @brief Returns item under given tag and index casted to a specified type, or nullptr (const
   * version).
   *
   * Never throws. Returns nullptr if the tag wasn't registered, if no item exists at the given
   * index, or if the item can't be casted to the given type.
   */
  template <typename ItemT = SessionItem>
  const ItemT* FindItem(const TagIndex& tag_index) const;

  /**
   * @brief Returns item under given tag and index casted to a specified type, or nullptr
   * (non-const version).
   */
  template <typename ItemT = SessionItem>
  ItemT* FindItem(const TagIndex& tag_index);

  /**
   * @brief Convenience overload addressing the first item in the given tag (const version).
   */
  template <typename ItemT = SessionItem>
  const ItemT* FindItem(std::string_view tag) const;

  /**
   * @brief Convenience overload addressing the first item in the given tag (non-const version).
   */
  template <typename ItemT = SessionItem>
  ItemT* FindItem(std::string_view tag);

  /**
   * @brief Returns all items under given tag which can be casted to a specific type (const
   * version).
   *
   * Never throws. Returns an empty vector if the tag wasn't registered. Items that can't be casted
   * to the given type are filtered out.
   *
   * @param tag The name of container's tag name.
   * @return Vector of items.
   */
  template <typename ItemT = SessionItem>
  std::vector<const ItemT*> FindItems(std::string_view tag) const;

  /**
   * @brief Returns all items under given tag which can be casted to a specific type (non-const
   * version).
   */
  template <typename ItemT = SessionItem>
  std::vector<ItemT*> FindItems(std::string_view tag);

  /**
   * @brief Returns pair of tag and index corresponding to given item.
   *
   * If given item is not direct child, will returns TagIndex::Invalid().
   *
   * @param item Possible child whose tag_index we want to find.
   * @return Items' tag_index.
   */
  TagIndex TagIndexOfItem(const SessionItem* item) const;

  /**
   * @brief Registers tag info to hold items under given name.
   *
   * @param tag_info A tag info object to register.
   * @param set_as_default Given tag will be used as default when empty tag name is used.
   */
  void RegisterTag(const TagInfo& tag_info, bool set_as_default = false);

  /**
   * @brief Inserts item into the given tag_index, ownership is taken.
   *
   * @param item Item to insert.
   * @param tag_index A tag_index pointing to the insert place.
   * @return Convenience pointer to just inserted item.
   */
  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index);

  /**
   * @brief Creates a new item of given type and insert it into the given tag_index.
   * @tparam Type of the item to create.
   * @param tag_index A tag_index pointing to the insert place.
   * @return Convenience pointer to just inserted item.
   */
  template <typename T = SessionItem>
  T* InsertItem(const TagIndex& tag_index);

  /**
   * @brief Removes an item from the given tag_index, returns it to the caller.
   */
  std::unique_ptr<SessionItem> TakeItem(const TagIndex& tag_index);

  /**
   * @brief Returns true if this item has editable flag set.
   *
   * The data role of editable items can be normally changed in Qt trees and tables by
   * double-clicking on the corresponding cell.
   */
  bool IsEditable() const;

  /**
   * @brief Sets item's editable flag to given value.
   *
   * @return The reference to the same item (fluent interface).
   */
  SessionItem& SetEditable(bool value) &;

  /**
   * @brief Returns true if this item has enabled flag set.
   *
   * Enabled items normally appear in default color in Qt trees and tables, while disabled items are
   * shown in gray.
   */
  bool IsEnabled() const;

  /**
   * @brief Sets item's enabled flag to given value.
   * @return The reference to the same item (fluent interface).
   */
  SessionItem& SetEnabled(bool value) &;

  /**
   * @brief Returns true if this item has visibility flag set.
   *
   * @details Visible items normally appear in Qt tree's and tables, while invisible items are
   * hidden from the user.
   */
  bool IsVisible() const;

  /**
   * @brief Sets item's visibility flag to given value.
   *
   * @return The reference to the same item (fluent interface).
   */
  SessionItem& SetVisible(bool value) &;

  /**
   * @brief Returns item tooltip, if exists.
   */
  std::string GetToolTip() const;

  /**
   * @brief Sets item's tooltip to given value.
   *
   * @return The reference to the same item (fluent interface).
   */
  SessionItem& SetToolTip(const std::string& tooltip) &;

  /**
   * @brief Returns editor type which is used to edit data role.
   */
  std::string GetEditorType() const;

  /**
   * @brief Sets editor type.
   *
   * @details This allows to define custom cell editors to edit data role of the item.
   */
  SessionItem& SetEditorType(const std::string_view& editor_type) &;

  /**
   * @brief Returns true if the item has given appearance flag set.
   */
  bool HasFlag(Appearance flag) const;

  /**
   * @brief Sets appearance flag to given value.
   */
  SessionItem& SetFlag(Appearance flag, bool value) &;

  /**
   * @brief Set item's model to the given value.
   */
  void SetModel(ISessionModel* model);

  /**
   * @brief Returns a slot for signal connection.
   *
   * Time of life of the slot is coupled with the item. After item destruction, all connections will
   * be safely cut.
   */
  Slot* GetSlot() const;

  /**
   * @brief Sets given item as parent.
   */
  void SetParent(SessionItem* parent);

  /**
   * @brief Returns pointer to item's data container (non-const version).
   */
  SessionItemData* GetItemData();

  /**
   * @brief Returns pointer to item's data container (const version).
   */
  const SessionItemData* GetItemData() const;

  /**
   * @brief Returns pointer to internal collection of registered item containers (const version).
   */
  const TaggedItems* GetTaggedItems() const;

  /**
   * @brief Returns pointer to internal collection of registered item containers (non-const
   * version).
   */
  TaggedItems* GetTaggedItems();

  /**
   * @brief Sets strategy which will be used to set the data.
   *
   * This method will be called on any call to SessionItem::SetData. It is responsibility of the
   * user to provide meaningfull behavior. @see VectorItem.
   */
  void SetStrategy(const data_strategy_t& data_strategy);

  /**
   * @brief Sets strategy which will be used to set the data (templated).
   *
   * @tparam ObjectT The type of the object.
   * @tparam MethodT The object's method.
   *
   * This method will be called on any call to SessionItem::SetData. It is responsibility of the
   * user to provide meaningfull behavior. @see VectorItem.
   */
  template <typename ObjectT, typename MethodT>
  void SetStrategy(ObjectT* object, const MethodT& method);

  /**
   * @brief Replace item internal structure and set the item type to the given value.
   */
  void SetDataAndTags(const std::string& item_type, std::unique_ptr<SessionItemData> data,
                      std::unique_ptr<TaggedItems> tags);

protected:
  explicit SessionItem(const std::string& item_type);
  SessionItem(const std::string& item_type, std::unique_ptr<SessionItemData> data,
              std::unique_ptr<TaggedItems> tags);

  /**
   * @brief A method to activate custom data strategies.
   */
  virtual void ActivateStrategies();

private:
  /**
   * @brief Returns all items in the given tag, or an empty vector if the tag wasn't registered.
   *
   * Total (non-throwing) counterpart of GetItems, used to implement FindItems.
   */
  std::vector<const SessionItem*> FindItemsImpl(std::string_view tag) const;
  std::vector<SessionItem*> FindItemsImpl(std::string_view tag);

  std::unique_ptr<SessionItemImpl> p_impl;
  data_strategy_t m_data_strategy;
};

template <typename T>
inline bool SessionItem::SetData(const T& value, role_t role)
{
  return SetDataInternal(value, role);
}

template <typename T>
inline T SessionItem::Data(role_t role) const
{
  if constexpr (std::is_same_v<T, variant_t>)
  {
    return GetDataImpl(role);  // if variant_t is required, simply return it
  }
  else
  {
    return std::get<T>(GetDataImpl(role));  // convert the data to required type
  }
}

template <typename ItemT>
inline const ItemT* SessionItem::FindItem(const TagIndex& tag_index) const
{
  if constexpr (std::is_same_v<ItemT, SessionItem>)
  {
    return FindItem(tag_index);  // non-template primitive, no RTTI involved
  }
  else
  {
    return dynamic_cast<const ItemT*>(FindItem(tag_index));
  }
}

template <typename ItemT>
inline ItemT* SessionItem::FindItem(const TagIndex& tag_index)
{
  return const_cast<ItemT*>(std::as_const(*this).FindItem<ItemT>(tag_index));
}

template <typename ItemT>
inline const ItemT* SessionItem::FindItem(std::string_view tag) const
{
  return FindItem<ItemT>(TagIndex(tag));
}

template <typename ItemT>
inline ItemT* SessionItem::FindItem(std::string_view tag)
{
  return FindItem<ItemT>(TagIndex(tag));
}

template <typename ItemT>
inline const ItemT& SessionItem::GetItem(const TagIndex& tag_index) const
{
  const auto* result = FindItem<ItemT>(tag_index);
  if (result == nullptr)
  {
    throw RuntimeException("Error in SessionItem::GetItem(): no item of the requested type at tag '"
                           + tag_index.GetTag() + "'");
  }
  return *result;
}

template <typename ItemT>
inline ItemT& SessionItem::GetItem(const TagIndex& tag_index)
{
  return const_cast<ItemT&>(std::as_const(*this).GetItem<ItemT>(tag_index));
}

template <typename ItemT>
inline const ItemT& SessionItem::GetItem(std::string_view tag) const
{
  return GetItem<ItemT>(TagIndex(tag));
}

template <typename ItemT>
inline ItemT& SessionItem::GetItem(std::string_view tag)
{
  return GetItem<ItemT>(TagIndex(tag));
}

template <typename ItemT>
inline std::vector<const ItemT*> SessionItem::FindItems(std::string_view tag) const
{
  return utils::CastItems<const ItemT>(FindItemsImpl(tag));
}

template <typename ItemT>
inline std::vector<ItemT*> SessionItem::FindItems(std::string_view tag)
{
  return utils::CastItems<ItemT>(FindItemsImpl(tag));
}

template <typename T>
inline T* SessionItem::InsertItem(const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), tag_index));
}

template <typename ObjectT, typename MethodT>
inline void SessionItem::SetStrategy(ObjectT* object, const MethodT& method)
{
  SetStrategy(std::bind(method, object, std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3));
}

template <>
struct item_traits<SessionItem>
{
  static constexpr std::string_view type_name() noexcept { return "SessionItem"; }
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_H_
