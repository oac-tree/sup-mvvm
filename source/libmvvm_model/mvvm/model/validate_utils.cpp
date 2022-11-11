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

#include <mvvm/core/exceptions.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/tagindex.h>

namespace mvvm::utils
{

TagIndex GetActualInsertTagIndex(const SessionItem *parent, const TagIndex &tag_index)
{
  if (!parent)
  {
    throw NullArgumentException("Unintialised parent");
  }

  std::string actual_tag =
      tag_index.tag.empty() ? parent->GetTaggedItems()->GetDefaultTag() : tag_index.tag;

  if (actual_tag.empty())
  {
    throw InvalidOperationException("No tag is marked as default");
  }

  int actual_index = tag_index.index < 0 ? parent->GetItemCount(actual_tag) : tag_index.index;

  return {TagIndex{actual_tag, actual_index}};
}

void ValidateItemInsert(const SessionItem *item, const SessionItem *parent,
                        const TagIndex &tag_index)
{
  if (!item)
  {
    throw InvalidOperationException("Invalid input item");
  }

  if (item == parent)
  {
    throw InvalidOperationException("Attempt to insert to itself");
  }

  if (item->GetParent())
  {
    throw InvalidOperationException("Item belongs to another parent");
  }

  if (!parent)
  {
    throw InvalidOperationException("Invalid parent item");
  }

  if (utils::IsItemAncestor(parent, item))
  {
    throw InvalidOperationException("Attempt to turn ancestor into a child");
  }

  if (!parent->GetTaggedItems()->CanInsertItem(item, tag_index))
  {
    throw InvalidOperationException("Can't insert item to parent");
  }
}

void ValidateItemMove(const SessionItem *item, const SessionItem *new_parent,
                      const TagIndex &tag_index)
{
  if (!item || !item->GetModel() || !item->GetParent())
  {
    throw InvalidOperationException("Invalid input item");
  }

  if (!new_parent || !new_parent->GetModel())
  {
    throw InvalidOperationException("Invalid parent item");
  }

  if (item->GetModel() != new_parent->GetModel())
  {
    throw InvalidOperationException("Items belong to different models");
  }

  auto current_parent = item->GetParent();
  if (!current_parent->GetTaggedItems()->CanTakeItem(item->GetTagIndex()))
  {
    throw InvalidOperationException("Can't take item from parent");
  }

  if (!new_parent->GetTaggedItems()->CanInsertItem(item, tag_index))
  {
    throw InvalidOperationException(
        "Can't insert item to a new parent. It doesn't allow more children of this type.");
  }

  if (item == new_parent)
  {
    throw InvalidOperationException("Attempt to insert an item to itself");
  }

  if (utils::IsItemAncestor(new_parent, item))
  {
    throw InvalidOperationException("Attempt to make ancestor a child");
  }
}

void ValidateTakeItem(const SessionModelInterface *model, const SessionItem *parent,
                      const TagIndex &tag_index)
{
  if (!parent)
  {
    throw InvalidOperationException("Parent is not defined");
  }

  if (!model)
  {
    throw InvalidOperationException("Model is not defined");
  }

  if (parent->GetModel() != model)
  {
    throw InvalidOperationException("Parent doesn't belong to given model");
  }

  if (!parent->GetTaggedItems()->CanTakeItem(tag_index))
  {
    throw InvalidOperationException("Can't take item from parent");
  }
}

}  // namespace mvvm::utils
