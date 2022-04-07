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

void ValidateItemMove(SessionItem *item, SessionItem *new_parent, const TagIndex &tag_index)
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
