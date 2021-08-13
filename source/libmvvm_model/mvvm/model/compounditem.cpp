/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
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
bool has_custom_display_name(const SessionItem* item)
{
  return item->SessionItem::displayName() != item->modelType();
}
}  // namespace

CompoundItem::CompoundItem(const std::string& modelType) : SessionItem(modelType) {}

std::string CompoundItem::displayName() const
{
  if (has_custom_display_name(this))
    return SessionItem::displayName();

  int copy_number = Utils::CopyNumber(this);
  return copy_number != -1 ? SessionItem::displayName() + std::to_string(copy_number)
                           : SessionItem::displayName();
}
