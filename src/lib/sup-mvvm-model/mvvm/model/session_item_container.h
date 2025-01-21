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

#ifndef MVVM_MODEL_SESSION_ITEM_CONTAINER_H_
#define MVVM_MODEL_SESSION_ITEM_CONTAINER_H_

#include <mvvm/model/taginfo.h>

#include <memory>
#include <vector>

namespace mvvm
{

class SessionItem;

/**
 * @brief The SessionItemContainer class holds a collection of SessionItem objects related to the
 * same tag.
 */
class MVVM_MODEL_EXPORT SessionItemContainer
{
public:
  using container_t = std::vector<std::unique_ptr<SessionItem>>;
  using const_iterator = container_t::const_iterator;

  explicit SessionItemContainer(TagInfo tag_info);
  SessionItemContainer(const SessionItemContainer& other) = delete;
  SessionItemContainer& operator=(const SessionItemContainer&) = delete;

  ~SessionItemContainer();

  /**
   * @brief Checks if the container is empty.
   */
  bool IsEmpty() const;

  /**
   * @brief Returns the number of all items in the container.
   */
  std::size_t GetItemCount() const;

  /**
   * @brief Returns all items in this container.
   */
  std::vector<SessionItem*> GetItems() const;

  /**
   * @brief Checks if the item can be inserted into the given index.
   */
  bool CanInsertItem(const SessionItem* item, std::size_t index) const;

  /**
   * @brief Inserts item in a vector of children at given index, returns pointer to the item in the
   * case of success.
   *
   * @param item Item to be inserted, ownership will be taken.
   * @param index Insert index in a range [0, itemCount]
   * @return A pointer to just inserted item.
   *
   * Insert index is an index which item will have after insertion. If item can't be inserted (wrong
   * model type, wrong index or maximum number of items reached), will return nullptr.
   *
   */
  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, std::size_t index);

  /**
   * @brief Checks if a new item with the given type can be inserted into the given index.
   */
  bool CanInsertType(const std::string& item_type, std::size_t new_index) const;

  /**
   * @brief Checks if the item can be moved into the given index.
   *
   * The behavior depends on if the item belongs already to the container, or not.
   */
  bool CanMoveItem(const SessionItem* item, std::size_t index) const;

  /**
   * @brief Checks if the item can be removed from the given index.
   */
  bool CanTakeItem(std::size_t index) const;

  /**
   * @brief Removes an item at given index and returns it to the user.
   *
   * If item can't be removed (item is a property item), will return nullptr.
   */
  std::unique_ptr<SessionItem> TakeItem(std::size_t index);

  /**
   * @brief Returns index of item in a vector of items.
   *
   * Will return empty optional, if item doesn't belong to this container.
   */
  std::optional<std::size_t> IndexOfItem(const SessionItem* item) const;

  /**
   * @brief Returns item at given index, or nullptr of index is invalid.
   */
  SessionItem* ItemAt(std::size_t index) const;

  /**
   * @brief Returns the name of the container.
   */
  std::string GetName() const;

  /**
   * @brief Returns TagInfo describing this container.
   */
  TagInfo GetTagInfo() const;

  /**
   * @brief The begin of the vector with items.
   */
  const_iterator begin() const;

  /**
   * @brief The end of the vector with items.
   */
  const_iterator end() const;

  /**
   * @brief Creates clone.
   */
  std::unique_ptr<SessionItemContainer> Clone() const;

  /**
   * @brief Checks if no more items are allowed.
   */
  bool IsMaximumReached() const;

  /**
   * @brief Checks if less items than now is not allowed.
   */
  bool IsMinimumReached() const;

private:
  TagInfo m_tag_info;
  container_t m_items;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_CONTAINER_H_
