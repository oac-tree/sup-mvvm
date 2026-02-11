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

#include "validate_utils.h"

#include "item_utils.h"
#include "session_item.h"
#include "tagged_items.h"
#include "tagindex.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace mvvm::utils
{

namespace
{

/**
 * @brief Checks if given item can be considered for move to another place.
 */
bool IsValidChildForMove(const SessionItem& item)
{
  return (item.GetModel() != nullptr) && (item.GetParent() != nullptr);
}

/**
 * @brief Checks if given item can be considered for move to another place.
 */
bool IsValidParentToMoveIn(const SessionItem& item)
{
  return (item.GetModel() != nullptr);
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

std::error_code GetInsertItemErrorCode(const SessionItem& item, const SessionItem& parent,
                                       const TagIndex& tag_index)
{
  if (&item == &parent)
  {
    return ModelError::ItemAndParentAreTheSame;
  }

  if (item.GetParent() != nullptr)
  {
    return ModelError::ItemHasParent;
  }

  if (utils::IsItemAncestor(&parent, &item))
  {
    return ModelError::ItemIsAncestorOfNewParent;
  }

  if (!parent.GetTaggedItems()->CanInsertItem(&item, GetInsertTagIndex(&parent, tag_index)))
  {
    return ModelError::FailedToInsertItem;
  }

  return ModelError::Success;
}

void ValidateItemInsert(const SessionItem& item, const SessionItem& parent,
                        const TagIndex& tag_index)
{
  const auto error_code = GetInsertItemErrorCode(item, parent, tag_index);
  if (error_code != ModelError::Success)
  {
    throw InvalidOperationException("Item insert validation failed: "
                                    + std::string(error_code.message()));
  }
}

std::error_code GetInsertTypeErrorCode(const std::string& item_type, const SessionItem* parent,
                                       const TagIndex& tag_index)
{
  if (parent == nullptr)
  {
    return ModelError::InvalidParentItem;
  }

  if (!parent->GetTaggedItems()->CanInsertType(item_type, GetInsertTagIndex(parent, tag_index)))
  {
    return ModelError::FailedToInsertItem;
  }

  return ModelError::Success;
}

void ValidateItemTypeInsert(const std::string& item_type, const SessionItem* parent,
                            const TagIndex& tag_index)
{
  const auto error_code = GetInsertTypeErrorCode(item_type, parent, tag_index);
  if (error_code != ModelError::Success)
  {
    throw InvalidOperationException("Item type insert validation failed: "
                                    + std::string(error_code.message()));
  }
}

std::error_code GetMoveItemErrorCode(const SessionItem& item, const SessionItem& new_parent,
                                     const TagIndex& tag_index)
{
  if (!IsValidChildForMove(item))
  {
    return ModelError::NullItem;
  }

  if (!IsValidParentToMoveIn(new_parent))
  {
    return ModelError::InvalidParentItem;
  }

  if (item.GetModel() != new_parent.GetModel())
  {
    return ModelError::ItemBelongToDifferentModel;
  }

  auto current_parent = item.GetParent();

  if (!current_parent->GetTaggedItems()->CanTakeItem(item.GetTagIndex()))
  {
    return ModelError::FailedToTakeItem;
  }

  if (!new_parent.GetTaggedItems()->CanMoveItem(&item, GetInsertTagIndex(&new_parent, tag_index)))
  {
    return ModelError::FailedToMoveItem;
  }

  if (utils::IsItemAncestor(&new_parent, &item))
  {
    return ModelError::ItemIsAncestorOfNewParent;
  }

  if (&item == &new_parent)
  {
    return ModelError::ItemAndParentAreTheSame;
  }

  return ModelError::Success;
}

void ValidateItemMove(const SessionItem& item, const SessionItem& new_parent,
                      const TagIndex& tag_index)
{
  const auto error_code = GetMoveItemErrorCode(item, new_parent, tag_index);
  if (error_code)
  {
    throw InvalidOperationException("Item move validation failed: "
                                    + std::string(error_code.message()));
  }
}

std::error_code GetTakeItemErrorCode(const SessionItem& parent, const TagIndex& tag_index)
{
  if (!parent.GetTaggedItems()->CanTakeItem(tag_index))
  {
    return ModelError::FailedToTakeItem;
  }

  return ModelError::Success;
}

void ValidateTakeItem(const SessionItem& parent, const TagIndex& tag_index)
{
  const auto error_code = GetTakeItemErrorCode(parent, tag_index);
  if (error_code)
  {
    throw InvalidOperationException("Take item validation failed: "
                                    + std::string(error_code.message()));
  }
}

}  // namespace mvvm::utils
