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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SERIALIZATION_SESSION_ITEM_COMPOSER_COMPONENTS_H_
#define MVVM_SERIALIZATION_SESSION_ITEM_COMPOSER_COMPONENTS_H_

#include <mvvm/serialization/converter_types.h>
#include <mvvm/serialization/tree_data.h>

#include <deque>

namespace mvvm
{

class SessionItem;
class IItemFactory;
class TaggedItems;

/**
 * @brief The ItemComposerComponent class helps constructing SessionItem from TreeData is
 * stack-based algorithms.
 */
class ItemComposerComponent
{
public:
  using component_t = std::unique_ptr<ItemComposerComponent>;
  using component_queue = std::deque<component_t>;

  ItemComposerComponent() = default;
  virtual ~ItemComposerComponent() = default;

  ItemComposerComponent(const ItemComposerComponent& other) = delete;
  ItemComposerComponent& operator=(const ItemComposerComponent& other) & = delete;
  ItemComposerComponent(ItemComposerComponent&&) = delete;
  ItemComposerComponent& operator=(ItemComposerComponent&&) = delete;

  /**
   * @brief Checks if there are other components in a queue.
   */
  bool HasNextComponent() const;

  /**
   * @brief Takes a component from a queue and returns to the user.
   */
  component_t TakeNextComponent();

  /**
   * @brief Returns number of components in a queue.
   */
  std::size_t GetComponentCount() const;

  /**
   * @brief Adds the next component to a queue.
   *
   * Our component is responsible for the queue of the next components to process. External
   * algorithm should take care of moving components from a queue to a stack.
   */
  void AddNextComponent(component_t component);

private:
  component_queue m_component_queue;
};

/**
 * @brief The ItemContainerFromTreeData class assists in conversion of TreeData to
 * SessionItemContainer.
 */
class ItemContainerFromTreeData : public ItemComposerComponent
{
public:
  ItemContainerFromTreeData(const ItemComposerContext& context, const tree_data_t& tree_data,
                            TaggedItems& tagged_items);
};

/**
 * @brief The TaggedItemsFromTreeData class assists in conversion of TreeData to TaggedItems.
 */
class TaggedItemsFromTreeData : public ItemComposerComponent
{
public:
  TaggedItemsFromTreeData(const ItemComposerContext& context, const tree_data_t& tree_data,
                          TaggedItems& tagged_items);
};

/**
 * @brief The SessionItemFromTreeData class assists in conversion of TreeData to SesisonItem.
 */
class SessionItemFromTreeData : public ItemComposerComponent
{
public:
  SessionItemFromTreeData(const ItemComposerContext& context, const tree_data_t& tree_data,
                          SessionItem& item);
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_SESSION_ITEM_COMPOSER_COMPONENTS_H_
