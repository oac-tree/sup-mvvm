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

#include "standard_children_strategies.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/utils/container_utils.h>

namespace mvvm
{

std::vector<SessionItem*> AllChildrenStrategy::GetChildren(const SessionItem* item) const
{
  // we returns all children, even if some marked as invisible
  return item ? item->GetAllItems() : std::vector<SessionItem*>();
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
