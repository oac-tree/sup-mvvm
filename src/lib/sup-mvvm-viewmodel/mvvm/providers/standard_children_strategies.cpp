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

#include "standard_children_strategies.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/group_item.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace mvvm
{

// IChildrenStrategy::GetChildren() takes a const item but hands out mutable children (see
// mvvm/providers/i_children_strategy.h). That contract is deliberately left as it is, so the
// strategies below have to drop the constness the item accessors now correctly preserve.

std::vector<SessionItem*> AllChildrenStrategy::GetChildren(const SessionItem* item) const
{
  // we returns all children, even if some marked as invisible
  return (item != nullptr) ? utils::RemoveConst(item->GetAllItems()) : std::vector<SessionItem*>();
}

std::vector<SessionItem*> AllVisibleChildrenStrategy::GetChildren(const SessionItem* item) const
{
  if (item == nullptr)
  {
    return {};
  }

  std::vector<const SessionItem*> result;
  auto children = item->GetAllItems();
  (void)std::copy_if(children.begin(), children.end(), std::back_inserter(result),
                     [](const auto* item) { return item->IsVisible(); });
  return utils::RemoveConst(result);
}

std::vector<SessionItem*> TopItemsStrategy::GetChildren(const SessionItem* item) const
{
  return (item != nullptr) ? utils::RemoveConst(utils::TopLevelItems(*item))
                           : std::vector<SessionItem*>();
}

std::vector<SessionItem*> PropertyItemsStrategy::GetChildren(const SessionItem* item) const
{
  const auto* group = dynamic_cast<const GroupItem*>(item);
  const auto* next_item = (group == nullptr) ? item : group->FindCurrentItem();

  return (next_item == nullptr) ? std::vector<SessionItem*>()
                                : utils::RemoveConst(utils::SinglePropertyItems(*next_item));
}

FixedItemTypeStrategy::FixedItemTypeStrategy(std::vector<std::string> item_types)
    : m_item_types(std::move(item_types))
{
}

std::vector<SessionItem*> FixedItemTypeStrategy::GetChildren(const SessionItem* item) const
{
  if (item == nullptr)
  {
    return {};
  }

  std::vector<const SessionItem*> result;
  for (const auto* child : item->GetAllItems())
  {
    if (utils::Contains(m_item_types, child->GetType()))
    {
      result.push_back(child);
    }
  }
  return utils::RemoveConst(result);
}

}  // namespace mvvm
