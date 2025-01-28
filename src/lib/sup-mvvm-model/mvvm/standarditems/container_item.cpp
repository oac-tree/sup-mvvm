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

#include "container_item.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/model/item_utils.h>

namespace mvvm
{
ContainerItem::ContainerItem(const std::string& model_type) : CompoundItem(model_type)
{
  RegisterTag(TagInfo::CreateUniversalTag(constants::kChildrenTag), /*set_as_default*/ true);
}

std::string ContainerItem::GetStaticType()
{
  return "Container";
}

std::unique_ptr<SessionItem> ContainerItem::Clone() const
{
  return std::make_unique<ContainerItem>(*this);
}

bool ContainerItem::IsEmpty() const
{
  return GetSize() == 0;
}

std::size_t ContainerItem::GetSize() const
{
  return GetItemCount(constants::kChildrenTag);
}

std::vector<SessionItem*> ContainerItem::GetChildren() const
{
  return GetAllItems();
}

void ContainerItem::Clear()
{
  for (auto child : GetChildren())
  {
    // using universal function to provide notifications, if possible
    utils::RemoveItem(*child);
  }
}

}  // namespace mvvm
