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

#include "group_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>
#include <iterator>

namespace mvvm
{

namespace
{

//! Returns vector of item types for the given vector of items.
std::vector<std::string> GetItemTypes(const std::vector<SessionItem*>& items)
{
  std::vector<std::string> result;
  (void)std::transform(items.begin(), items.end(), std::back_inserter(result),
                       [](const auto* item) { return item->GetType(); });
  return result;
}

}  // namespace

GroupItem::GroupItem(const std::string& item_type) : SessionItem(item_type), m_index_to_select(0)
{
  RegisterTag(TagInfo::CreateUniversalTag(std::string(kGroupTag)), /*set_as_default*/ true);
  (void)SetData(ComboProperty());
}

std::int32_t GroupItem::GetCurrentIndex() const
{
  return Data<ComboProperty>().GetCurrentIndex();
}

std::string GroupItem::GetCurrentType() const
{
  const auto* current_item = FindCurrentItem();
  return (current_item == nullptr) ? std::string() : current_item->GetType();
}

void GroupItem::SetCurrentType(const std::string& item_type)
{
  auto index = utils::IndexOfItem(GetItemTypes(GetAllItems()), item_type);
  if (index == constants::kInvalidIndex)
  {
    throw RuntimeException("Error in GroupItem::SetCurrentType(): item type '" + item_type
                           + "' doesn't belong to the group");
  }
  SetCurrentIndex(static_cast<std::int32_t>(index));
}

void GroupItem::SetCurrentIndex(std::int32_t index)
{
  auto combo = Data<ComboProperty>();
  combo.SetCurrentIndex(index);
  (void)SetData(combo);
}

void GroupItem::UpdateCombo()
{
  ComboProperty combo;
  combo.SetValues(m_item_text);
  combo.SetCurrentIndex(m_index_to_select);
  (void)SetData(combo);
}

}  // namespace mvvm
