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

#include "item_listener.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

ItemListener::ItemListener(SessionItem *item) : m_item(item), m_slot(std::make_unique<Slot>())
{
  if (!m_item)
  {
    throw RuntimeException("Unitialised item");
  }

  if (!m_item->GetModel() || !m_item->GetModel()->GetEventHandler())
  {
    throw RuntimeException("Error in ItemListenerBase: model doesn't have signals");
  }
}

ItemListener::~ItemListener() = default;

SessionItem *ItemListener::GetItem() const
{
  return m_item;
}

Slot *ItemListener::GetSlot() const
{
  return m_slot.get();
}

}  // namespace mvvm
