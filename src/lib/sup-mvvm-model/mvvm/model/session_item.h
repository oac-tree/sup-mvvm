/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_SESSION_ITEM_H_
#define MVVM_MODEL_SESSION_ITEM_H_

#include <mvvm/core/exceptions.h>
#include <mvvm/core/variant.h>
#include <mvvm/model/mvvm_types.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/signals/signal_slot_fwd.h>
#include <mvvm/utils/container_utils.h>

#include <memory>
#include <vector>

namespace mvvm
{

class ISessionModel;
class TagInfo;
class SessionItemData;
class TaggedItems;

/**
 * @brief The SessionItem class is a base element to build a hierarchical structure representing all
 * the data of the application.
 *
 * It can contain an arbitrary amount of basic data types and can be a parent for other
 * SessionItems.
 */
class MVVM_MODEL_EXPORT SessionItem
{
public:
  static inline const std::string Type = "SessionItem";

  SessionItem();
  virtual ~SessionItem();

  SessionItem& operator=(const SessionItem&) = delete;

  /**
   * @brief Copy constructor.
   */
  SessionItem(const SessionItem& other);

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
  SessionItem& SetDisplayName(const std::string& name);

  /**
   * @brief Returns the model to which given item belongs to, or nullptr if item doesn't belong to a
   * model.
   */
  ISessionModel* GetModel() const;

  /**
   * @brief Returns item parent.
   */
  SessionItem* GetParent() const;

  /**
   * @brief  Returns TagIndex of this item under which it is accessible through its parent.
   */
  TagIndex GetTagIndex() const;

  /**
   * @brief Returns true if item has data on board with given role.
   *
   * @details
   */
  bool HasData(int role = DataRole::kData) const;

  /**
   * @brief Returns data for the given role.
   *
   * If given role doesn't exist, will return uninitialized variant_t{}. If role exists, will
   * perform convertion to the given T. Will throw, if conversion is not possible.
   *
   * @param role The role of the data.
   * @tparam T The type to convert the data.
   *
   * @return The variant with the data, or data itself if template parameter is specified.
   */
  template <typename T = variant_t>
  T Data(int role = DataRole::kData) const;

  /**
   * @brief Sets the data for the given role.
   *
   * When extra parameter direct is false (default case), will act through the model. This will
   * invoke a notification mechanism and provide the possibility for undo, if implemented. When
   * direct is true, will set the data via SessionItem API to bypass notifications.
   *
   * If the data is the same as before, will return false as a sign that no data was changed. It is
   * not possible to change the data type for a given role, once the role was set for the first
   * time. Thus, an attempt to set an integer to the role containing a string will lead to the
   * exception.
   *
   * @param value The value to set.
   * @param role The role of the data.
   * @param direct Set the data through the model, if set to false.
   *
   * @return Returns true, if the data was changed.
   */
  template <typename T>
  bool SetData(const T& value, int role = DataRole::kData, bool direct = false);

  /**
   * @brief Specialized version for const char to avoid its conversion to bool.
   */
  bool SetData(const char* value, int role = DataRole::kData, bool direct = false);

  /**
   * @brief Returns pointer to item's data container (non-const version).
   */
  SessionItemData* GetItemData();

  /**
   * @brief Returns pointer to item's data container (const version).
   */
  const SessionItemData* GetItemData() const;

  /**
   * @brief Returns total number of children in all tags.
   */
  int GetTotalItemCount() const;

  /**
   * @brief Returns vector of children formed from all children from all tags.
   */
  std::vector<SessionItem*> GetAllItems() const;

  /**
   * @brief Returns number of items in given tag.
   */
  int GetItemCount(const std::string& tag) const;

  /**
   * @brief Returns item located at given tag_index, or nullptr if an item doesn't exist.
   *
   * If the tag name is empty, the tag registered as default will be used. Will throw, when provided
   * non-empty tag wasn't registered.
   */
  SessionItem* GetItem(const TagIndex& tag_index) const;

  /**
   * @brief Returns vector of items in the container with given tag name.
   *
   * If the tag name is empty, container registered as default will be used.
   */
  std::vector<SessionItem*> GetItems(const std::string& tag) const;

  /**
   * @brief Returns item under given tag and index casted to a specified type.
   *
   * Returns nullptr, if item doesn't exist. If item exists but can't be casted will throw.
   */
  template <typename T>
  T* GetItem(const TagIndex& tag_index) const;
  template <typename T = SessionItem>

  /**
   * @brief Returns all items under given tag casted to specific type.
   *
   * @param tag The name of container's tag name.
   * @return Vector of items.
   */
  std::vector<T*> GetItems(const std::string& tag) const;

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
   * @brief Returns pointer to internal collection of registered item containers (const version).
   */
  const TaggedItems* GetTaggedItems() const;

  /**
   * @brief Returns pointer to internal collection of registered item containers (non-const
   * version).
   */
  TaggedItems* GetTaggedItems();

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
  SessionItem& SetEditable(bool value);

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
  SessionItem& SetEnabled(bool value);

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
  SessionItem& SetVisible(bool value);

  /**
   * @brief Returns item tooltip, if exists.
   */
  std::string GetToolTip() const;

  /**
   * @brief Sets item's tooltip to given value.
   *
   * @return The reference to the same item (fluent interface).
   */
  SessionItem& SetToolTip(const std::string& tooltip);

  /**
   * @brief Returns editor type which is used to edit data role.
   */
  std::string GetEditorType() const;

  /**
   * @brief Sets editor type.
   *
   * @details This allows to define custom cell editors to edit data role of the item.
   */
  SessionItem& SetEditorType(const std::string& editor_type);

  /**
   * @brief Returns true if the item has given appearance flag set.
   */
  bool HasFlag(Appearance flag) const;

  /**
   * @brief Sets appearance flag to given value.
   */
  SessionItem& SetFlag(Appearance flag, bool value);

  /**
   * @brief Activates business logic.
   *
   * The method is called by the model when an item is inserted into the model. It allows to perform
   * some additional initialization on the item's board, if necessary. For example, it can provide
   * custom connections so the item reacts on its own properties change. See VectorItem example.
   */
  virtual void Activate();

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

protected:
  explicit SessionItem(const std::string& item_type);
  SessionItem(const std::string& item_type, std::unique_ptr<SessionItemData> data,
              std::unique_ptr<TaggedItems> tags);

  /**
   * @brief Sets the data for the given role.
   */
  bool SetDataInternal(const variant_t& value, int role, bool direct);

private:
  friend class TreeDataItemConverter;

  /**
   * @brief Returns the data stored for the given role.
   */
  variant_t DataInternal(int role) const;

  void SetParent(SessionItem* parent);

  void SetDataAndTags(std::unique_ptr<SessionItemData> data, std::unique_ptr<TaggedItems> tags);

  struct SessionItemImpl;  //!< implementation details
  std::unique_ptr<SessionItemImpl> p_impl;
};

template <typename T>
inline bool SessionItem::SetData(const T& value, int role, bool direct)
{
  return SetDataInternal(value, role, direct);
}

template <typename T>
inline T SessionItem::Data(int role) const
{
  if constexpr (std::is_same_v<T, variant_t>)
  {
    return DataInternal(role);  // if variant_t is required, simply return it
  }
  else
  {
    return std::get<T>(DataInternal(role));  // convert the data to required type
  }
}

template <typename T>
inline T* SessionItem::GetItem(const TagIndex& tag_index) const
{
  if (auto item = GetItem(tag_index); item)
  {
    T* tag_item = dynamic_cast<T*>(item);
    if (!tag_item)
    {
      throw RuntimeException("Error in SessionItem::GetItem(): can't cast an item to given type");
    }
    return tag_item;
  }
  return nullptr;
}

template <typename T>
std::vector<T*> SessionItem::GetItems(const std::string& tag) const
{
  return utils::CastItems<T>(GetItems(tag));
}

template <typename T>
inline T* SessionItem::InsertItem(const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), tag_index));
}

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_H_
