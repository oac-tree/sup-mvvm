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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "property_item.h"

namespace mvvm
{

PropertyItem::PropertyItem() : SessionItem(GetStaticType()) {}

std::string PropertyItem::GetStaticType()
{
  return "PropertyItem";
}

std::unique_ptr<SessionItem> PropertyItem::Clone() const
{
  return std::make_unique<PropertyItem>(*this);
}

}  // namespace mvvm
