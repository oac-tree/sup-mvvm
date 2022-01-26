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

namespace mvvm
{

struct ItemListenerBase::ItemListenerBaseImpl
{
  SessionItem *m_item{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections

  ItemListenerBaseImpl() : m_slot(std::make_unique<Slot>()) {}
};

ItemListenerBase::ItemListenerBase(SessionItem *item)
    : p_impl(std::make_unique<ItemListenerBaseImpl>())
{
}

ItemListenerBase::~ItemListenerBase()
{
  // destruction of m_slot will destruct all connections
}

void ItemListenerBase::SetItem(SessionItem *item) {}

void ItemListenerBase::SetOnDataChange(Callbacks::item_int_t f) {}

SessionItem *ItemListenerBase::GetCurrentItem() const
{
  return p_impl->m_item;
}

}  // namespace mvvm
