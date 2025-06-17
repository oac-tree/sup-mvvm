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

#ifndef MVVM_SERIALIZATION_TREE_DATA_COMPOSER_COMPONENT_H_
#define MVVM_SERIALIZATION_TREE_DATA_COMPOSER_COMPONENT_H_

#include <mvvm/serialization/tree_data.h>

#include <memory>
#include <deque>

namespace mvvm
{

/**
 * @brief The TreeDataComposerComponent class is a wrapper around TreeData to simplify
 * adding nested children in stack-based algorithms.
 *
 * It is intended to build a TreeData object from the SessionItem hierarchy. It contains a queue
 * of components which external algorithm should place in a stack.
 */
class TreeDataComposerComponent
{
public:
  using component_t = std::unique_ptr<TreeDataComposerComponent>;
  using component_queue = std::deque<component_t>;

  /**
   * @brief Main c-tor.
   */
  explicit TreeDataComposerComponent(const std::string& node_name);
  virtual ~TreeDataComposerComponent();

  TreeDataComposerComponent(const TreeDataComposerComponent& other) = delete;
  TreeDataComposerComponent& operator=(const TreeDataComposerComponent& other) & = delete;
  TreeDataComposerComponent(TreeDataComposerComponent&&) = delete;
  TreeDataComposerComponent& operator=(TreeDataComposerComponent&&) = delete;

  /**
   * @brief Move the resulting object.
   */
  tree_data_t MoveTreeData();

  /**
   * @brief Returns copy of underlying tree data.
   */
  tree_data_t GetTreeData() const;

  /**
   * @brief Adds child to underlying TreeData.
   */
  void AddChild(const tree_data_t& tree_data);

  /**
   * @brief Adds attribute to underlying TreeData.
   */
  void AddAttribute(const std::string& name, const std::string& value);

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
  std::size_t GetQueuedComponentsCount() const;

  /**
   * @brief Adds the next component to a queue.
   *
   * Our component is responsible for the queue of the next components to process. External
   * algorithm should take care of moving components from a queue to a stack.
   */
  void AddNextComponent(component_t component);

private:
  tree_data_t m_tree_data;
  component_queue m_component_queue;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_COMPOSER_COMPONENT_H_
