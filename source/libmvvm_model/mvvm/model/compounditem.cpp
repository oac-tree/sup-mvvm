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

#include "mvvm/model/compounditem.h"

#include "mvvm/model/itemutils.h"

using namespace ModelView;

namespace
{
bool HasCustomDisplayName(const SessionItem* item)
{
  return item->SessionItem::GetDisplayName() != item->GetType();
}
}  // namespace

CompoundItem::CompoundItem(const std::string& model_type) : SessionItem(model_type) {}

//! Returns custom display name with index appended.
//! CompoundItem0, CompoundItem1, CompoundItem2, ...

std::string CompoundItem::GetDisplayName() const
{
  // if item has already display name, use it
  if (HasCustomDisplayName(this))
  {
    return SessionItem::GetDisplayName();
  }

  int copy_number = Utils::CopyNumber(this);
  return copy_number != -1 ? SessionItem::GetDisplayName() + std::to_string(copy_number)
                           : SessionItem::GetDisplayName();
}
