/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/compound_item.h"

#include <mvvm/model/item_utils.h>

namespace
{
bool HasCustomDisplayName(const mvvm::SessionItem *item)
{
  return item->SessionItem::GetDisplayName() != item->GetType();
}
}  // namespace

namespace mvvm
{

CompoundItem::CompoundItem(const std::string &item_type) : SessionItem(item_type) {}

std::unique_ptr<SessionItem> CompoundItem::Clone(bool make_unique_id) const
{
  return std::make_unique<CompoundItem>(*this, make_unique_id);
}

//! Specialized version for const char: method is introduced to avoid "const char" conversion into
//! variant<bool>.
PropertyItem *CompoundItem::AddProperty(const std::string &name, const char *value)
{
  return AddProperty(name, std::string(value));
}

//! Returns custom display name with index appended.
//! CompoundItem0, CompoundItem1, CompoundItem2, ...

std::string CompoundItem::GetDisplayName() const
{
  // if item has already display name, use it
  if (HasCustomDisplayName(this))
  {
    return SessionItem::GetDisplayName();
  }

  int copy_number = utils::CopyNumber(this);
  return copy_number != -1 ? SessionItem::GetDisplayName() + std::to_string(copy_number)
                           : SessionItem::GetDisplayName();
}

}  // namespace mvvm
