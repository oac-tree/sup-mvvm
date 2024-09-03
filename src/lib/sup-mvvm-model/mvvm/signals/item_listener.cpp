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

#include "item_listener.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

struct ItemListener::ItemListenerImpl
{
  SessionItem *m_item{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections
  ItemListener *m_self{nullptr};

  explicit ItemListenerImpl(ItemListener *self)
      : m_self(self), m_slot(std::make_unique<Slot>())
  {
  }

  Slot *GetSlot() const { return m_slot.get(); }

  void UnsubscribeFromCurrent()
  {
    if (!m_item)
    {
      return;
    }

    m_self->Unsubscribe();              // let derived classes to do some peculiar job
    m_slot = std::make_unique<Slot>();  // drops all previous connections
    m_item = nullptr;
  }
};

ItemListener::ItemListener() : p_impl(std::make_unique<ItemListenerImpl>(this)) {}

ItemListener::~ItemListener() =
    default;  // destruction of m_slot will destruct all connections

void ItemListener::SetItem(SessionItem *item)
{
  if (GetCurrentItem() == item)
  {
    return;
  }

  p_impl->UnsubscribeFromCurrent();

  if (!item)
  {
    return;
  }

  if (!item->GetModel() || !item->GetModel()->GetEventHandler())
  {
    throw RuntimeException("Error in ItemListenerBase: model doesn't have signals");
  }

  p_impl->m_item = item;

  Subscribe();
}

SessionItem *ItemListener::GetCurrentItem() const
{
  return p_impl->m_item;
}

Slot *ItemListener::GetSlot() const
{
  return p_impl->m_slot.get();
}

}  // namespace mvvm
