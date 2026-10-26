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

#include "test_strategies.h"

#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewitem.h>
#include <mvvm/standarditems/container_item.h>

namespace mvvm::test
{

QStringList EmptyRowTestStrategy::GetHorizontalHeaderLabels() const
{
  return {};
}

std::vector<std::unique_ptr<ViewItem> > EmptyRowTestStrategy::ConstructRowImpl(SessionItem *)
{
  return {};
}

std::vector<SessionItem*> FlatContainerTestStrategy::GetChildren(SessionItem* item) const
{
  std::vector<SessionItem*> result;
  if (item == nullptr)
  {
    return result;
  }

  for (auto* child : item->GetAllItems())
  {
    if (dynamic_cast<ContainerItem*>(child) != nullptr)
    {
      auto container_children = child->GetAllItems();
      (void)result.insert(result.end(), container_children.begin(), container_children.end());
    }
    else
    {
      result.push_back(child);
    }
  }
  return result;
}

SessionItem* FlatContainerTestStrategy::GetViewParent(SessionItem* item) const
{
  auto* parent = item->GetParent();
  return (dynamic_cast<ContainerItem*>(parent) != nullptr) ? parent->GetParent() : parent;
}

}  // namespace mvvm::test
