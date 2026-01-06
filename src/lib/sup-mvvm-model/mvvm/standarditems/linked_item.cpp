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

#include "linked_item.h"

#include <mvvm/model/session_model.h>

namespace
{
constexpr std::string_view kEmptyLinkIdentifier;  // intentionally left uninitialized
}

namespace mvvm
{

LinkedItem::LinkedItem() : SessionItem(GetTypeName<LinkedItem>())
{
  (void)SetData(std::string(kEmptyLinkIdentifier));
  (void)SetEditable(false);  // prevent editing in widgets, link is set programmatically.
}

std::unique_ptr<SessionItem> LinkedItem::Clone() const
{
  return std::make_unique<LinkedItem>(*this);
}

//! Set link to given item.

void LinkedItem::SetLink(const SessionItem* item)
{
  (void)SetData((item != nullptr) ? item->GetIdentifier() : std::string(kEmptyLinkIdentifier));
}

SessionItem* LinkedItem::GetLink() const
{
  return (GetModel() != nullptr) ? GetModel()->FindItem(Data<std::string>()) : nullptr;
}

}  // namespace mvvm
