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

#ifndef MVVM_MODEL_I_SESSION_MODEL_H_
#define MVVM_MODEL_I_SESSION_MODEL_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class ModelEventHandler;
class ICommandStack;

/**
 * @brief The ISessionModel class is an interface for all application models holding
 * SessionItem hierarchy.
 */
class MVVM_MODEL_EXPORT ISessionModel
{
public:
  virtual ~ISessionModel() = default;

  /**
   * @brief Returns model type.
   */
  virtual std::string GetType() const = 0;

  /**
   * @brief Returns root item.
   */
  virtual SessionItem* GetRootItem() const = 0;

  /**
   * @brief Returns event handler.
   * @return Event handler.
   */
  virtual ModelEventHandler* GetEventHandler() const = 0;

  /**
   * @brief Returns command stack.
   */
  virtual ICommandStack* GetCommandStack() const = 0;

  /**
   * @brief Inserts an item into the given parent and takes ownership of it.
   *
   * @param An item to insert.
   * @param parent The parent where to insert.
   * @param tag_index A tag_index pointing to the inserted place.
   * @return The pointer to the inserted item.
   */
  virtual SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                  const TagIndex& tag_index) = 0;

  /**
   * @brief Creates and inserts an item of a given type into the given parent and takes ownership of
   * it.
   *
   * @tparam Type of item to create.
   * @param parent The parent where to insert.
   * @param tag_index A tag_index pointing to the place to insert.
   * @return The pointer to the inserted item.
   */
  template <typename T>
  T* InsertItem(SessionItem* parent = nullptr, const TagIndex& tag_index = TagIndex::Append());

  /**
   * @brief Takes a child from a parent and returns it to the caller.
   *
   * @param parent A parent item from where take the item.
   * @param tag_index A tag_index pointing to the child.
   * @return Taken item.
   */
  virtual std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  /**
   * @brief Removes an item from the model and discards it.
   *
   * The item should belong to the model. Its exact parent and tag_index will be deduced from the
   * item itself.
   *
   * @param item The item to remove.
   */
  virtual void RemoveItem(SessionItem* item) = 0;

  /**
   * @brief Moves an item from it's current parent to a new parent.
   *
   * Old and new parents should belong to the same model. Please note, that the current
   * implementation moves and item via combination of take/insert. This leads to corresponding
   * notifications: about-to-remove, item removed, about-to-insert, item inserted.
   *
   * @param item The item to move.
   * @param new_parent New parent where to insert.
   * @param tag_index A tag_index pointing to the insert place.
   */
  virtual void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) = 0;

  /**
   * @brief Sets the value to the given data role of the given item.
   *
   * If the data is the same as before, will return false and will suppress notifications. It is not
   * possible to change the data type for a given role, once the role is set for the first time.
   * I.e. an attempt to set an integer to the role containing a string will lead to the exception.
   * User utils::ReplaceData if you need to change a data type.
   *
   * @param item The item to set the data.
   * @param value The value.
   * @param role The data role.
   * @return Returns true, if the data was changed.
   */
  virtual bool SetData(SessionItem* item, const variant_t& value, int role) = 0;

  /**
   * @brief Finds an item with the given identifier among all model's items.
   *
   * @param id A unique item identifier.
   * @return  A pointer to the item.
   */
  virtual SessionItem* FindItem(const std::string& id) const = 0;

  /**
   * @brief Clears the model.
   *
   * Internally replaces root item with new empty root item.
   */
  virtual void Clear() = 0;

  /**
   * @brief Replaces existing root item with new root item.
   *
   * This method is used in serialization to restore the model from persistent content. New root
   * item must be initialized (unique_ptr shall not be empty).
   *
   * @param root_item New root item, possibly pre-filled with some content.
   */
  virtual void ReplaceRootItem(std::unique_ptr<SessionItem> root_item) = 0;

  /**
   * @brief A tech method to inform the model about new item.
   *
   * Internally it just registers an item in memory pool for faster search.
   */
  virtual void CheckIn(SessionItem* item) = 0;

  /**
   * @brief A tech method to inform the model that item is no longer belongs to the model.
   *
   * Internally it just un-registers an item from memory pool.
   */
  virtual void CheckOut(SessionItem* item) = 0;
};

template <typename T>
T* ISessionModel::InsertItem(SessionItem* parent, const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), parent, tag_index));
}

}  // namespace mvvm

#endif  // MVVM_MODEL_I_SESSION_MODEL_H_
