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

#include "compound_item.h"

#include <mvvm/model/item_utils.h>

namespace mvvm
{

CompoundItem::CompoundItem(const std::string &item_type) : SessionItem(item_type) {}

std::string CompoundItem::GetStaticType()
{
  return "Compound";
}

std::unique_ptr<SessionItem> CompoundItem::Clone() const
{
  return std::make_unique<CompoundItem>(*this);
}

PropertyItem &CompoundItem::AddProperty(const std::string &name, const char *value)
{
  return AddProperty(name, std::string(value));
}

std::string CompoundItem::GetDisplayName() const
{
  // if item has already display name, use it
  if (HasData(mvvm::DataRole::kDisplay))
  {
    return Data<std::string>(DataRole::kDisplay);
  }

  // add index to type name
  const int copy_number = utils::CopyNumber(this);
  return copy_number < 0 ? GetType() : GetType() + std::to_string(copy_number);
}

}  // namespace mvvm
