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

#ifndef MVVM_MODEL_TAGGED_ITEMS_H_
#define MVVM_MODEL_TAGGED_ITEMS_H_

#include <mvvm/model/tagindex.h>
#include <mvvm/model_export.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace mvvm
{

class SessionItemContainer;
class TagInfo;
class SessionItem;

/**
 * @brief The TaggedItems class holds a collection of SessionItems named containers.
 *
 * Container properties are described with the help of the TagInfo class. The container is
 * accessible via its name (we use the words "name" and "tag" interchangeably). One of the
 * container can be marked as default, it will be used when no name is provided.
 *
 * For container manipulation (insert, take, get) one have to know the position of the item, given
 * by TagIndex class.
 */
class MVVM_MODEL_EXPORT TaggedItems
{
public:
  using container_t = std::vector<std::unique_ptr<SessionItemContainer>>;
  using const_iterator = container_t::const_iterator;

  TaggedItems();
  ~TaggedItems();
  TaggedItems(const TaggedItems&) = delete;
  TaggedItems& operator=(const TaggedItems&) = delete;

  /**
   * @brief Registers tag.
   *
   * Creates new container intended for storage of certain item types.
   *
   * @param tag_info A tag information describing properties of the container.
   * @param set_as_default A flag to mark a container as default.
   */
  void RegisterTag(const TagInfo& tag_info, bool set_as_default = false);

  /**
   * @brief Checks if container with such name exists.
   */
  bool HasTag(const std::string& tag) const;

  /**
   * @brief Returns the name of the default tag.
   */
  std::string GetDefaultTag() const;

  /**
   * @brief Sets the default tag name.
   */
  void SetDefaultTag(const std::string& tag);

  /**
   * @brief Returns number of items in the container with given name.
   */
  int GetItemCount(const std::string& tag) const;

  /**
   * @brief Checks if the given item can be inserted into the given place.
   *
   * @param item Item to insert.
   * @param tag_index A TagIndex pointing to the insert place.
   * @return True if insertion is possible.
   */
  bool CanInsertItem(const SessionItem* item, const TagIndex& tag_index) const;

  /**
   * @brief Checks if item type can be inserted into the given place.
   */
  bool CanInsertType(const std::string& item_type, const TagIndex& tag_index) const;

  /**
   * @brief Inserts item into the given tag_index, ownership is taken.
   *
   * @param item Item to insert.
   * @param tag_index A tag_index pointing to the insert place.
   *
   * @return Convenience pointer to just inserted item.
   */
  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index);

  /**
   * @brief Checks if item can be removed from given place.
   */
  bool CanTakeItem(const TagIndex& tag_index) const;

  /**
   * @brief Removes an item from the given tag_index, returns it to the caller.
   */
  std::unique_ptr<SessionItem> TakeItem(const TagIndex& tag_index);

  /**
   * @brief Checks if the item can be moved into the given index.
   *
   * @param item Item to insert.
   * @param tag_index A TagIndex pointing to the insert place.
   */
  bool CanMoveItem(const SessionItem* item, const TagIndex& tag_index) const;

  /**
   * @brief Returns item located at given tag_index, or nullptr if an item doesn't exist.
   *
   * If the tag name is empty, the tag registered as default will be used. Will throw, when
   * provided non-empty tag wasn't registered. The tag index shouldn't exceed the number of items in
   * the corresponding container.
   */
  SessionItem* GetItem(const TagIndex& tag_index) const;

  /**
   * @brief Returns all items from the container with the given name.
   *
   * If tag name is empty, default tag will be used.
   */
  std::vector<SessionItem*> GetItems(const std::string& tag = {}) const;

  /**
   * @brief Returns vector of items formed from all children of all named container.
   */
  std::vector<SessionItem*> GetAllItems() const;

  /**
   * @brief Returns pair of tag and index corresponding to given item.
   *
   * @param item Possible child whose tag_index we want to find.
   * @return Items' tag_index.
   *
   * @details If given item is not direct child, will returns {"", -1}.
   */
  TagIndex TagIndexOfItem(const SessionItem* item) const;

  /**
   * @brief The begin of the vector with containers.
   */
  const_iterator begin() const;

  /**
   * @brief The end of the vector with containers.
   */
  const_iterator end() const;

  /**
   * @brief Returns total number of all registered containers.
   */
  int GetTagCount() const;

  /**
   * @brief Index operator to access containers.
   */
  SessionItemContainer& ContainerAt(int index);

  /**
   * @brief Appends given container at the end.
   */
  void AppendContainer(std::unique_ptr<SessionItemContainer> container);

  /**
   * @brief Creates clones.
   *
   * @param make_unique_id Regenerates unique identifiers of the item and all its children, when
   * true.
   * @return Full clone of this.
   */
  std::unique_ptr<TaggedItems> Clone(bool make_unique_id) const;

  /**
   * @brief Returns container corresponding to the given name, or throw if container doesn't exist.
   *
   * If the provided tag name is empty, will try to find a container registered by default.
   */
  SessionItemContainer* GetContainer(const std::string& tag) const;

  /**
   * @brief Returns container corresponding to the given tag name, or nullptr if container doesn't
   * exist.
   *
   * If the provided tag name is empty, will try to find a container registered by default.
   */
  SessionItemContainer* FindContainer(const std::string& tag) const;

  /**
   * @brief Converts insert index into an actual TagIndex.
   *
   * The method is mainly used to convert TagIndex::Append() to the actual insert index. Also
   * performs validation if index is valid and tag name exists. Will return an empty result if it is
   * not the case.
   *
   * @param index A tag index for item insert.
   * @return An optional
   */
  std::optional<TagIndex> GetInsertTagIndex(const TagIndex& index) const;

private:
  container_t m_containers;
  std::string m_default_tag;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_TAGGED_ITEMS_H_
