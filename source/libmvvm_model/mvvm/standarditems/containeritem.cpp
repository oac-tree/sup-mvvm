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

#include "mvvm/standarditems/containeritem.h"

#include <sstream>

namespace
{
const std::string kChildren = "kChildren";
}

namespace mvvm
{
ContainerItem::ContainerItem(const std::string& model_type) : CompoundItem(model_type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*set_as_default*/ true);
}

bool ContainerItem::IsEmpty() const
{
  return GetItemCount(kChildren) == 0;
}

}  // namespace ModelView
