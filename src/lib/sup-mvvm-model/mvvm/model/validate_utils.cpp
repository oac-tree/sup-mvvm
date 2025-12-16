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

#include "validate_utils.h"

#include "item_utils.h"
#include "session_item.h"
#include "tagged_items.h"
#include "tagindex.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace
{
const bool kSuccess = true;
const bool kFailure = false;
}  // namespace

namespace mvvm::utils
{

namespace
{

/**
 * @brief Checks if given item can be considered for move to another place.
 */
bool IsValidChildForMove(const SessionItem* item)
{
  return (item != nullptr) && (item->GetModel() != nullptr) && (item->GetParent() != nullptr);
}

/**
 * @brief Checks if given item can be considered for move to another place.
 */
bool IsValidParentToMoveIn(const SessionItem* item)
{
  return (item != nullptr) && (item->GetModel() != nullptr);
}

}  // namespace

TagIndex GetInsertTagIndex(const SessionItem* parent, const TagIndex& tag_index)
{
  if (parent == nullptr)
  {
    throw NullArgumentException("Unintialised parent");
  }
  return parent->GetTaggedItems()->GetInsertTagIndex(tag_index);
}

std::error_code GetInsertItemErrorCode(const SessionItem* item, const SessionItem* parent,
                                       const TagIndex& tag_index)
{
  if (item == nullptr)
  {
    return ModelError::NullItem;
  }

  if (item == parent)
  {
    return ModelError::ItemAndParentAreTheSame;
  }

  if (item->GetParent() != nullptr)
  {
    return ModelError::ItemHasParent;
  }

  if (parent == nullptr)
  {
    return ModelError::InvalidParentItem;
  }

  if (utils::IsItemAncestor(parent, item))
  {
    return ModelError::ItemIsAncestorOfNewParent;
  }

  if (!parent->GetTaggedItems()->CanInsertItem(item, GetInsertTagIndex(parent, tag_index)))
  {
    return ModelError::FailedToInsertItem;
  }

  return ModelError::Success;
}

void ValidateItemInsert(const SessionItem* item, const SessionItem* parent,
                        const TagIndex& tag_index)
{
  const auto error_code = GetInsertItemErrorCode(item, parent, tag_index);
  if (error_code != ModelError::Success)
  {
    throw InvalidOperationException("Item insert validation failed: "
                                    + std::string(error_code.message()));
  }
}

std::pair<bool, std::string> CanInsertType(const std::string& item_type, const SessionItem* parent,
                                           const TagIndex& tag_index)
{
  if (parent == nullptr)
  {
    return {kFailure, "Invalid parent item"};
  }

  if (!parent->GetTaggedItems()->CanInsertType(item_type, GetInsertTagIndex(parent, tag_index)))
  {
    return {kFailure, "Can't insert item of the given type to the parent"};
  }

  return {kSuccess, ""};
}

void ValidateItemTypeInsert(const std::string& item_type, const SessionItem* parent,
                            const TagIndex& tag_index)
{
  if (auto [flag, reason] = CanInsertType(item_type, parent, tag_index); !flag)
  {
    throw InvalidOperationException(reason);
  }
}

std::pair<bool, std::string> CanMoveItem(const SessionItem* item, const SessionItem* new_parent,
                                         const TagIndex& tag_index)
{
  if (!IsValidChildForMove(item))
  {
    return {kFailure, "Invalid input item"};
  }

  if (!IsValidParentToMoveIn(new_parent))
  {
    return {kFailure, "Invalid parent item"};
  }

  if (item->GetModel() != new_parent->GetModel())
  {
    return {kFailure, "Items belong to different models"};
  }

  auto current_parent = item->GetParent();

  if (!current_parent->GetTaggedItems()->CanTakeItem(item->GetTagIndex()))
  {
    return {kFailure, "Can't take item from parent"};
  }

  if (!new_parent->GetTaggedItems()->CanMoveItem(item, GetInsertTagIndex(new_parent, tag_index)))
  {
    return {kFailure, "Can't move item to this place"};
  }

  if (utils::IsItemAncestor(new_parent, item))
  {
    return {kFailure, "Attempt to make ancestor a child"};
  }

  if (item == new_parent)
  {
    return {kFailure, "Attempt to insert an item to itself"};
  }

  return {kSuccess, ""};
}

void ValidateItemMove(const SessionItem* item, const SessionItem* new_parent,
                      const TagIndex& tag_index)
{
  if (auto [flag, reason] = CanMoveItem(item, new_parent, tag_index); !flag)
  {
    throw InvalidOperationException(reason);
  }
}

std::pair<bool, std::string> CanTakeItem(const SessionItem* parent, const TagIndex& tag_index)
{
  if (parent == nullptr)
  {
    return {kFailure, "Parent is not defined"};
  }

  if (!parent->GetTaggedItems()->CanTakeItem(tag_index))
  {
    return {kFailure, "Can't take item from parent"};
  }

  return {kSuccess, ""};
}

void ValidateTakeItem(const SessionItem* parent, const TagIndex& tag_index)
{
  if (auto [flag, reason] = CanTakeItem(parent, tag_index); !flag)
  {
    throw InvalidOperationException(reason);
  }
}

}  // namespace mvvm::utils
