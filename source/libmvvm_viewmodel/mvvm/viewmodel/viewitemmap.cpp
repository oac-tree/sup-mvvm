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

#include "mvvm/viewmodel/viewitemmap.h"

#include <stdexcept>

namespace mvvm
{
ViewItemMap::ViewItemMap() = default;

//! Save instruction and its corresponding view.

void ViewItemMap::Insert(const SessionItem *item, ViewItem *view_item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    throw std::runtime_error("Error in ViewItemMap: item is already registered");
  }
  m_item_to_view.insert(it, {item, view_item});
}

//! Update the instruction for view.

void ViewItemMap::Update(const SessionItem *item, ViewItem *view_item)
{
  m_item_to_view.insert_or_assign(item, view_item);
}

//! Find view for given item.

ViewItem *ViewItemMap::FindView(const SessionItem *item)
{
  auto it = m_item_to_view.find(item);
  return it == m_item_to_view.end() ? nullptr : it->second;
}

//! Removes views corresponding to given instruction.

void ViewItemMap::Remove(const SessionItem *item)
{
  auto it = m_item_to_view.find(item);
  if (it != m_item_to_view.end())
  {
    m_item_to_view.erase(it);
  }
  else
  {
    throw std::runtime_error("Error in ViewItemMap: not exist");
  }
}

void ViewItemMap::Clear()
{
  m_item_to_view.clear();
}

}  // namespace mvvm
