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

#ifndef MVVM_INTERFACES_SESSIONMODEL_INTERFACE_H_
#define MVVM_INTERFACES_SESSIONMODEL_INTERFACE_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class ModelEventHandler;

/**
 * @brief The SessionModelInterface class is an interface for all application models holding
 * SessionItem hierarchy.
 */

class MVVM_MODEL_EXPORT SessionModelInterface
{
public:
  virtual ~SessionModelInterface() = default;

  /**
   * @brief Get model type.
   * @return Model type.
   */
  virtual std::string GetType() const = 0;

  /**
   * @brief Returns root item.
   * @return Root item.
   */
  virtual SessionItem* GetRootItem() const = 0;

  /**
   * @brief Get event handler.
   * @return Event handler.
   */
  virtual ModelEventHandler* GetEventHandler() const = 0;

  /**
   * @brief Inserts item into the given parent and take ownership of it.
   *
   * @param And item to insert.
   * @param parent The parent where to insert.
   * @param tag_index A tag_index pointing to the insert place.
   * @return The pointer to just inserted item.
   */
  virtual SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                  const TagIndex& tag_index) = 0;

  /**
   * @brief Creates and inserts item of given type into the given parent and take ownership of it.
   *
   * @tparam Type of item to create.
   * @param parent The parent where to insert.
   * @param tag_index A tag_index pointing to the insert place.
   * @return The pointer to just inserted item.
   */
  template <typename T>
  T* InsertItem(SessionItem* parent = nullptr, const TagIndex& tag_index = TagIndex::Append());

  /**
   * @brief Takes a child from parent and return it to the caller.
   *
   * @param parent A parent item from where take the item.
   * @param tag_index A tag_index pointing to the child.
   * @return Taken item.
   */
  virtual std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  /**
   * @brief Removes an item from the model and discards it.
   * @param item The item to remove.
   *
   * @details The item should belong to the model. It's exact parent and tag_index will be deduced
   * from the item itself.
   */
  virtual void RemoveItem(SessionItem* item) = 0;

  /**
   * @brief Moves an item from it's current parent to a new parent.
   *
   * @param item The item to move.
   * @param new_parent New parent where to insert.
   * @param tag_index A tag_index pointing to the insert place.
   *
   * @details Old and new parents should belong to the same model. Please note, that the current
   * implementation moves and item via combination of take/insert. This leads to corresponding
   * notifications: about-to-remove, item removed, about-to-insert, item inserted.
   */
  virtual void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) = 0;

  /**
   * @brief Sets the value to the given data role of the given item.
   *
   * @param item The item to set the data.
   * @param value The value.
   * @param role The data role.
   * @return Returns true, if the data was changed.
   *
   * @details If the data is the same as before, will return false and will suppress notifications.
   *
   * @details It is not possible to change the data type for a given role, once the role was set for
   * the first time. I.e. attempt to set an integer to the role containing a string will lead to
   * the exception.
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
   * @brief Removes all items from the model and recreates a new empty root item.
   *
   * @param root_item Optional new root item, possibly pre-filled with some content.
   */
  virtual void Clear(std::unique_ptr<SessionItem> root_item) = 0;

  /**
   * @brief A tech method to inform the model about new item.
   *
   * @details Internally it just registers an item in memory pool for faster search.
   */
  virtual void CheckIn(SessionItem* item) = 0;

  /**
   * @brief A tech method to inform the model that item is no longer belongs to the model.
   *
   * @details Internally it just un-registers an item from memory pool.
   */
  virtual void CheckOut(SessionItem* item) = 0;
};

template <typename T>
T* SessionModelInterface::InsertItem(SessionItem* parent, const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), parent, tag_index));
}

}  // namespace mvvm

#endif  // MVVM_INTERFACES_SESSIONMODEL_INTERFACE_H_
