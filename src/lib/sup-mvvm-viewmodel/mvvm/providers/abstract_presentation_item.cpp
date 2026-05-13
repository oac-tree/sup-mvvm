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

#include "abstract_presentation_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

AbstractItemPresentation::AbstractItemPresentation(SessionItem* item) : m_item(item) {}

bool AbstractItemPresentation::IsEnabled() const
{
  return m_item->IsEnabled();
}

bool AbstractItemPresentation::IsEditable() const
{
  return m_item->IsEditable();
}

SessionItem* AbstractItemPresentation::GetItem()
{
  return m_item;
}

const SessionItem* AbstractItemPresentation::GetItem() const
{
  return m_item;
}

}  // namespace mvvm