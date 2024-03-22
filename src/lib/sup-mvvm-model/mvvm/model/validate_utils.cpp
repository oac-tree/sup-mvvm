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

#include "validate_utils.h"

#include "item_utils.h"
#include "sessionitem.h"
#include "tagged_items.h"
#include "tagindex.h"

#include <mvvm/core/exceptions.h>

namespace
{
const bool kSuccess = true;
const bool kFailure = false;
}  // namespace

namespace mvvm::utils
{

TagIndex GetInsertTagIndex(const SessionItem *parent, const TagIndex &tag_index)
{
  if (!parent)
  {
    throw NullArgumentException("Unintialised parent");
  }
  return parent->GetTaggedItems()->GetInsertTagIndex(tag_index);
}

std::pair<bool, std::string> CanInsertItem(const SessionItem *item, const SessionItem *parent,
                                           const TagIndex &tag_index)
{
  if (!item)
  {
    return {kFailure, "Invalid input item"};
  }

  if (item == parent)
  {
    return {kFailure, "Attempt to insert to itself"};
  }

  if (item->GetParent())
  {
    return {kFailure, "Item belongs to another parent"};
  }

  if (!parent)
  {
    return {kFailure, "Invalid parent item"};
  }

  if (utils::IsItemAncestor(parent, item))
  {
    return {kFailure, "Attempt to turn ancestor into a child"};
  }

  if (!parent->GetTaggedItems()->CanInsertItem(item, tag_index))
  {
    return {kFailure, "Can't insert item to parent"};
  }

  return {kSuccess, ""};
}

void ValidateItemInsert(const SessionItem *item, const SessionItem *parent,
                        const TagIndex &tag_index)
{
  if (auto [flag, reason] = CanInsertItem(item, parent, tag_index); !flag)
  {
    throw InvalidOperationException(reason);
  }
}

std::pair<bool, std::string> CanMoveItem(const SessionItem *item, const SessionItem *new_parent,
                                         const TagIndex &tag_index)
{
  if (!item || !item->GetModel() || !item->GetParent())
  {
    return {kFailure, "Invalid input item"};
  }

  if (!new_parent || !new_parent->GetModel())
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

  if (!new_parent->GetTaggedItems()->CanMoveItem(item, tag_index))
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

void ValidateItemMove(const SessionItem *item, const SessionItem *new_parent,
                      const TagIndex &tag_index)
{
  if (auto [flag, reason] = CanMoveItem(item, new_parent, tag_index); !flag)
  {
    throw InvalidOperationException(reason);
  }
}

std::pair<bool, std::string> CanTakeItem(const SessionItem *parent, const TagIndex &tag_index)
{
  if (!parent)
  {
    return {kFailure, "Parent is not defined"};
  }

  if (!parent->GetTaggedItems()->CanTakeItem(tag_index))
  {
    return {kFailure, "Can't take item from parent"};
  }

  return {kSuccess, ""};
}

void ValidateTakeItem(const SessionItem *parent, const TagIndex &tag_index)
{
  if (auto [flag, reason] = CanTakeItem(parent, tag_index); !flag)
  {
    throw InvalidOperationException(reason);
  }
}

}  // namespace mvvm::utils
