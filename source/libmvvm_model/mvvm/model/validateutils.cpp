/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/core/exceptions.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/model/tagindex.h"

namespace mvvm::utils
{

TagIndex GetActualInsertTagIndex(const mvvm::SessionItem *parent, const TagIndex &tag_index)
{
  if (!parent)
  {
    throw ArgumentNullException("Unintialised parent");
  }

  std::string actual_tag =
      tag_index.tag.empty() ? parent->GetTaggedItems()->GetDefaultTag() : tag_index.tag;

  if (actual_tag.empty())
  {
    throw InvalidInsertException("No tag is marked as default");
  }

  int actual_index = tag_index.index < 0 ? parent->GetItemCount(actual_tag) : tag_index.index;

  return {TagIndex{actual_tag, actual_index}};
}

void ValidateItemInsert(const SessionItem *item, const SessionItem *parent,
                        const TagIndex &tag_index)
{
  if (!item)
  {
    throw InvalidInsertException("Invalid input item");
  }

  if (item->GetParent())
  {
    throw InvalidInsertException("Item belongs to another parent");
  }

  if (!parent)
  {
    throw InvalidInsertException("Invalid parent item");
  }

  auto actual_tag_index = GetActualInsertTagIndex(parent, tag_index);

  if (!parent->GetTaggedItems()->CanInsertItem(item, actual_tag_index))
  {
    throw InvalidInsertException("Can't insert item to a new parent");
  }
}

void ValidateItemMove(const mvvm::SessionItem *item, const mvvm::SessionItem *new_parent,
                      const TagIndex &tag_index)
{
  if (!item || !item->GetModel() || !item->GetParent())
  {
    throw InvalidMoveException("Invalid input item");
  }

  if (!new_parent || !new_parent->GetModel())
  {
    throw InvalidMoveException("Invalid parent item");
  }

  if (item->GetModel() != new_parent->GetModel())
  {
    throw InvalidMoveException("Items belong to different models");
  }

  auto current_parent = item->GetParent();
  if (!current_parent->GetTaggedItems()->CanTakeItem(item->GetTagIndex()))
  {
    throw InvalidMoveException("Can't take item from parent");
  }

  if (!new_parent->GetTaggedItems()->CanInsertItem(item, tag_index))
  {
    throw InvalidMoveException("Can't insert item to a new parent");
  }
}

}  // namespace mvvm::utils
