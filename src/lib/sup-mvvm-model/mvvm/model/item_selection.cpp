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

#include "item_selection.h"

#include <mvvm/utils/container_utils.h>

namespace mvvm
{

ItemSelection::ItemSelection(const SessionItem* selected_item)
{
  if (selected_item != nullptr)
  {
    // we do not store nullptr
    m_items.push_back(selected_item);
  }
}

ItemSelection::ItemSelection(std::nullptr_t) {}

ItemSelection::ItemSelection(const std::vector<const SessionItem*>& selected_items)
    : m_items(selected_items)
{
}

const SessionItem* ItemSelection::GetItem() const
{
  return m_items.empty() ? nullptr : m_items.front();
}

std::vector<const SessionItem*> ItemSelection::GetItems() const
{
  return m_items;
}

bool ItemSelection::IsSameSelection(const ItemSelection& other) const
{
  return utils::HaveSameElements(GetItems(), other.GetItems());
}

bool ItemSelection::IsEmpty() const
{
  return GetItems().empty();
}

bool operator==(const ItemSelection& lhs, const ItemSelection& rhs)
{
  return lhs.GetItems() == rhs.GetItems();
}

bool operator!=(const ItemSelection& lhs, const ItemSelection& rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm
