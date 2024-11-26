/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "linked_item.h"

#include <mvvm/model/session_model.h>

namespace
{
const std::string empty_link;
}

namespace mvvm
{

LinkedItem::LinkedItem() : SessionItem(Type)
{
  SetData(empty_link);
  SetEditable(false);  // prevent editing in widgets, link is set programmatically.
}

std::unique_ptr<SessionItem> LinkedItem::Clone(bool make_unique_id) const
{
  return std::make_unique<LinkedItem>(*this);
}

//! Set link to given item.

void LinkedItem::SetLink(const SessionItem* item)
{
  SetData(item ? item->GetIdentifier() : empty_link);
}

SessionItem* LinkedItem::GetLink() const
{
  return GetModel() ? GetModel()->FindItem(Data<std::string>()) : nullptr;
}

}  // namespace mvvm
