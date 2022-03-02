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

#include "mvvm/signals/itemlistenerbase.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/modelutils.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/signals/itemconnectutils.h"

namespace mvvm
{

struct ItemListenerBase::ItemListenerBaseImpl
{
  SessionItem *m_item{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections
  ItemListenerBase *m_self{nullptr};

  explicit ItemListenerBaseImpl(ItemListenerBase *self)
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

ItemListenerBase::ItemListenerBase() : p_impl(std::make_unique<ItemListenerBaseImpl>(this)) {}

ItemListenerBase::~ItemListenerBase() =
    default;  // destruction of m_slot will destruct all connections

void ItemListenerBase::SetItem(SessionItem *item)
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

  if (!utils::HasSignals(item->GetModel()))
  {
    throw std::runtime_error("Error in ItemListenerBase: model doesn't have signals");
  }

  p_impl->m_item = item;

  Subscribe();
}

void ItemListenerBase::SetOnItemInserted(const Callbacks::item_tagindex_t &func)
{
  connect::OnItemInserted(GetCurrentItem(), func, p_impl->GetSlot());
}

void ItemListenerBase::SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t &func)
{
  connect::OnAboutToRemoveItem(GetCurrentItem(), func, p_impl->GetSlot());
}

void ItemListenerBase::SetOnItemRemoved(const Callbacks::item_tagindex_t &func)
{
  connect::OnItemRemoved(GetCurrentItem(), func, p_impl->GetSlot());
}

void ItemListenerBase::SetOnDataChanged(const Callbacks::item_int_t &func)
{
  connect::OnDataChanged(GetCurrentItem(), func, p_impl->GetSlot());
}

void ItemListenerBase::SetOnPropertyChanged(const Callbacks::item_str_t &func)
{
  connect::OnPropertyChanged(GetCurrentItem(), func, p_impl->GetSlot());
}

SessionItem *ItemListenerBase::GetCurrentItem() const
{
  return p_impl->m_item;
}

}  // namespace mvvm
