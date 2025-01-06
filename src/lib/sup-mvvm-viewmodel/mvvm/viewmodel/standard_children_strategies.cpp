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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "standard_children_strategies.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>

namespace mvvm
{

std::vector<SessionItem*> AllChildrenStrategy::GetChildren(const SessionItem* item) const
{
  // we returns all children, even if some marked as invisible
  return item ? item->GetAllItems() : std::vector<SessionItem*>();
}

std::vector<SessionItem*> AllVisibleChildrenStrategy::GetChildren(const SessionItem* item) const
{
  if (!item)
  {
    return {};
  }

  std::vector<SessionItem*> result;
  auto children = item->GetAllItems();
  std::copy_if(children.begin(), children.end(), std::back_inserter(result),
                 [](auto item) { return item->IsVisible(); });
  return result;
}

std::vector<SessionItem*> TopItemsStrategy::GetChildren(const SessionItem* item) const
{
  return item ? utils::TopLevelItems(*item) : std::vector<SessionItem*>();
}

std::vector<SessionItem*> PropertyItemsStrategy::GetChildren(const SessionItem* item) const
{
  return item ? utils::SinglePropertyItems(*item) : std::vector<SessionItem*>();
}

FixedItemTypeStrategy::FixedItemTypeStrategy(std::vector<std::string> item_types)
    : m_item_types(std::move(item_types))
{
}

std::vector<SessionItem*> FixedItemTypeStrategy::GetChildren(const SessionItem* item) const
{
  if (!item)
  {
    return {};
  }

  std::vector<SessionItem*> result;
  for (auto child : item->GetAllItems())
  {
    if (utils::Contains(m_item_types, child->GetType()))
    {
      result.push_back(child);
    }
  }
  return result;
}

}  // namespace mvvm
