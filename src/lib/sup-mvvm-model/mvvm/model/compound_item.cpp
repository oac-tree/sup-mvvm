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

#include "compound_item.h"

#include "make_clone.h"

namespace mvvm
{

CompoundItem::CompoundItem(const std::string& item_type) : SessionItem(item_type) {}

std::unique_ptr<SessionItem> CompoundItem::Clone() const
{
  return MakeClone(*this);
}

PropertyItem& CompoundItem::AddProperty(const std::string_view& name, const char* value)
{
  return AddProperty(name, std::string(value));
}

}  // namespace mvvm
