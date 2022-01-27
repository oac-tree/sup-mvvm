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

#include "mvvm/signals/itemmapper.h"

#include "mvvm/model/applicationmodel.h"

namespace mvvm
{
struct ItemMapper::ItemMapperImpl
{
  SessionItem *m_item{nullptr};
  std::unique_ptr<Slot> m_slot;
  Signal<void(SessionItem *, const TagIndex &)> m_item_inserted;
  Signal<void(SessionItem *, const TagIndex &)> m_about_to_remove_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_removed;
  Signal<void(SessionItem *, int)> m_data_changed;
  Signal<void(SessionItem *, std::string)> m_property_changed;
  Signal<void(SessionItem *)> m_item_about_to_be_destroyed;

  explicit ItemMapperImpl(SessionItem *item) : m_item(item), m_slot(std::make_unique<Slot>())
  {
    if (!GetModel())
    {
      throw std::runtime_error(
          "Error in ItemListenerBase(): item doesn't belong to model with notifications enabled");
    }
  }

  ApplicationModel *GetModel() const
  {
    return dynamic_cast<ApplicationModel *>(m_item->GetModel());
  }

  ModelEventSubscriberInterface *GetSubscriber() { return GetModel()->GetSubscriber(); }

  void EstablishConnectionWithTheModel() {}
};

ItemMapper::ItemMapper(SessionItem *item) : p_impl(std::make_unique<ItemMapperImpl>(item)) {}

Connection ItemMapper::SetOnItemInserted(Callbacks::item_tagindex_t f, Slot *slot)
{
  p_impl->EstablishConnectionWithTheModel();
}

Connection ItemMapper::SetOnAboutToRemoveItem(Callbacks::item_tagindex_t f, Slot *slot)
{
  p_impl->EstablishConnectionWithTheModel();
}

Connection ItemMapper::SetOnItemRemoved(Callbacks::item_tagindex_t f, Slot *slot)
{
  p_impl->EstablishConnectionWithTheModel();
}

Connection ItemMapper::SetOnDataChanged(Callbacks::item_int_t f, Slot *slot)
{
  p_impl->EstablishConnectionWithTheModel();
}

Connection ItemMapper::SetOnPropertyChanged(Callbacks::item_str_t f, Slot *slot)
{
  p_impl->EstablishConnectionWithTheModel();
}

Connection ItemMapper::SetOnItemDestroy(Callbacks::item_t f, Slot *slot)
{
  p_impl->EstablishConnectionWithTheModel();
}

}  // namespace mvvm
