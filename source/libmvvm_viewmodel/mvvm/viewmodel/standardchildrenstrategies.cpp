/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/standardchildrenstrategies.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/model/sessionitem.h"

namespace ModelView
{
// ----------------------------------------------------------------------------

std::vector<SessionItem*> AllChildrenStrategy::GetChildren(const SessionItem* item) const
{
  // we returns all children, even if some marked as invisible
  return item ? item->GetAllItems() : std::vector<SessionItem*>();
}

std::vector<SessionItem*> TopItemsStrategy::GetChildren(const SessionItem* item) const
{
  return item ? Utils::TopLevelItems(*item) : std::vector<SessionItem*>();
}

// ----------------------------------------------------------------------------

std::vector<SessionItem*> PropertyItemsStrategy::GetChildren(const SessionItem* item) const
{
  if (!item)
  {
    return std::vector<SessionItem*>();
  }

  // FIXME restore, when group property is there
  //    auto group = dynamic_cast<const GroupItem*>(item);
  //    auto next_item = group ? group->currentItem() : item;
  //    return Utils::SinglePropertyItems(*next_item);
  return Utils::SinglePropertyItems(*item);
}

}  // namespace ModelView
