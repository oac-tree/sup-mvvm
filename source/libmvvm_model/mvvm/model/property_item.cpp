/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/property_item.h"

#include <mvvm/model/mvvm_types.h>

namespace mvvm
{

PropertyItem::PropertyItem() : SessionItem(Type) {}

PropertyItem* PropertyItem::SetDisplayName(const std::string& name)
{
  // method is implemented to change the return type from SessionItem to ProprtyItem
  SessionItem::SetDisplayName(name);
  return this;
}

// FIXME uncomment
// PropertyItem* PropertyItem::setLimits(const RealLimits& value)
//{
//    this->setData(value, ItemDataRole::LIMITS);
//    return this;
//}

}  // namespace mvvm
