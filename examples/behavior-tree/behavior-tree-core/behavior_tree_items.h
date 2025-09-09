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

#ifndef BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_BEHAVIOR_TREE_ITEMS_H_
#define BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_BEHAVIOR_TREE_ITEMS_H_

#include <mvvm/standarditems/node_item.h>

namespace behaviortree
{

class SequenceItem : public mvvm::NodeItem
{
public:
  SequenceItem();
};

class WaitItem : public mvvm::NodeItem
{
public:
  WaitItem();
};

class MessageItem : public mvvm::NodeItem
{
public:
  MessageItem();
};

class RepeatItem : public mvvm::NodeItem
{
public:
  RepeatItem();
};

}  // namespace behaviortree

namespace mvvm
{

template <>
struct item_traits<behaviortree::SequenceItem>
{
  static constexpr std::string_view type_name() noexcept { return "SequenceItem"; }
};

template <>
struct item_traits<behaviortree::WaitItem>
{
  static constexpr std::string_view type_name() noexcept { return "WaitItem"; }
};

template <>
struct item_traits<behaviortree::MessageItem>
{
  static constexpr std::string_view type_name() noexcept { return "MessageItem"; }
};

template <>
struct item_traits<behaviortree::RepeatItem>
{
  static constexpr std::string_view type_name() noexcept { return "RepeatItem"; }
};

}  // namespace mvvm

#endif  // BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_BEHAVIOR_TREE_ITEMS_H_
