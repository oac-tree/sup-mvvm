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

#include "mvvm/signals/modeleventnotifier.h"

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/signals/signalslot.h"

namespace mvvm
{
struct ModelEventNotifier::ModelEventNotifierImpl
{
  std::map<ModelEventListenerInterface *, std::vector<Connection>> m_connections;

  Signal<void(SessionItem *, const TagIndex &)> m_about_to_insert_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_inserted;
  Signal<void(SessionItem *, const TagIndex &)> m_about_to_remove_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_removed;
  Signal<void(SessionItem *, int)> m_data_changed;
  Signal<void(SessionModel *)> m_model_about_to_reset;
  Signal<void(SessionModel *)> m_model_reset;
  Signal<void(SessionModel *)> m_model_about_to_be_destroyed;

  void disconnect(Connection connection)
  {
    m_about_to_insert_item.disconnect(connection);
    m_item_inserted.disconnect(connection);
    m_about_to_remove_item.disconnect(connection);
    m_item_removed.disconnect(connection);
    m_data_changed.disconnect(connection);
    m_model_about_to_reset.disconnect(connection);
    m_model_reset.disconnect(connection);
    m_model_about_to_be_destroyed.disconnect(connection);
  }
};

ModelEventNotifier::ModelEventNotifier(ModelEventListenerInterface *listener)
    : p_impl(std::make_unique<ModelEventNotifierImpl>())
{
  if (listener)
  {
    Subscribe(listener);
  }
}

ModelEventNotifier::~ModelEventNotifier()
{
  for (auto it : p_impl->m_connections)
  {
    // tell all listeners not to bother us with Unsubscribe on their own deletion
    it.first->SetNotifier(nullptr);
  }
}

void ModelEventNotifier::Unsubscribe(ModelEventListenerInterface *listener)
{
  auto it = p_impl->m_connections.find(listener);
  if (it == p_impl->m_connections.end())
  {
    throw std::runtime_error("Error in ModelEventNotifier: no such listener exists");
  }

  for (const auto &conn : it->second)
  {
    p_impl->disconnect(conn);
  }
  p_impl->m_connections.erase(it);

  listener->SetNotifier(nullptr);
}

//! Subscribe listener for our notifications. Since listener is not QObject, we have to store
//! connections explicitely, to be able to disconnect later, if necessary.

void ModelEventNotifier::Subscribe(ModelEventListenerInterface *listener)
{
  if (p_impl->m_connections.find(listener) != p_impl->m_connections.end())
  {
    throw std::runtime_error("Error in ModelEventNotifier: item is already connected");
  }

  std::vector<Connection> connections;

  // FIXME provide templated method for the wollowing syntax
  //  connections.emplace_back(p_impl->m_about_to_insert_item.connect(
  //      listener, &ModelEventListenerInterface::OnAboutToInsertItem));
  auto on_about_to_insert = [listener](auto item, auto tagindex)
  { listener->OnAboutToInsertItem(item, tagindex); };
  connections.emplace_back(SetOnAboutToInsertItem(on_about_to_insert, nullptr));

  auto on_item_inserted = [listener](auto item, auto tagindex)
  { listener->OnItemInserted(item, tagindex); };
  connections.emplace_back(SetOnItemInserted(on_item_inserted, nullptr));

  auto on_about_to_remove = [listener](auto item, auto tagindex)
  { listener->OnAboutToRemoveItem(item, tagindex); };
  connections.emplace_back(SetOnAboutToRemoveItem(on_about_to_remove, nullptr));

  auto on_item_removed = [listener](auto item, auto tagindex)
  { listener->OnItemRemoved(item, tagindex); };
  connections.emplace_back(SetOnItemRemoved(on_item_removed, nullptr));

  auto on_data_changed = [listener](auto item, auto role) { listener->OnDataChanged(item, role); };
  connections.emplace_back(SetOnDataChanged(on_data_changed, nullptr));

  auto on_model_about_reset = [listener](auto model) { listener->OnModelAboutToBeReset(model); };
  connections.emplace_back(SetOnModelAboutToBeReset(on_model_about_reset, nullptr));

  auto on_model_reset = [listener](auto model) { listener->OnModelReset(model); };
  connections.emplace_back(SetOnModelReset(on_model_reset, nullptr));

  auto on_model_about_destroyed = [listener](auto model) { listener->OnModelAboutToBeDestroyed(model); };
  connections.emplace_back(SeOnModelAboutToBeDestroyed(on_model_about_destroyed, nullptr));

  p_impl->m_connections.insert(p_impl->m_connections.find(listener), {listener, connections});

  listener->SetNotifier(this);
}

Connection ModelEventNotifier::SetOnAboutToInsertItem(Callbacks::item_tagindex_t f, Slot *slot)
{
  return p_impl->m_about_to_insert_item.connect(f, slot);
}

Connection ModelEventNotifier::SetOnItemInserted(Callbacks::item_tagindex_t f, Slot *slot)
{
  return p_impl->m_item_inserted.connect(f, slot);
}

Connection ModelEventNotifier::SetOnAboutToRemoveItem(Callbacks::item_tagindex_t f, Slot *slot)
{
  return p_impl->m_about_to_remove_item.connect(f, slot);
}

Connection ModelEventNotifier::SetOnItemRemoved(Callbacks::item_tagindex_t f, Slot *slot)
{
  return p_impl->m_item_removed.connect(f, slot);
}

Connection ModelEventNotifier::SetOnDataChanged(Callbacks::item_int_t f, Slot *slot)
{
  return p_impl->m_data_changed.connect(f, slot);
}

Connection ModelEventNotifier::SetOnModelAboutToBeReset(Callbacks::model_t f, Slot *slot)
{
  return p_impl->m_model_about_to_reset.connect(f, slot);
}

Connection ModelEventNotifier::SetOnModelReset(Callbacks::model_t f, Slot *slot)
{
  return p_impl->m_model_reset.connect(f, slot);
}

Connection ModelEventNotifier::SeOnModelAboutToBeDestroyed(Callbacks::model_t f, Slot *slot)
{
  return p_impl->m_model_about_to_be_destroyed.connect(f, slot);
}

// ------------------------------------------------------------------------

void ModelEventNotifier::AboutToInsertItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_about_to_insert_item(parent, tag_index);
}

void ModelEventNotifier::ItemInsertedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_item_inserted(parent, tag_index);
}

void ModelEventNotifier::AboutToRemoveItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_about_to_remove_item(parent, tag_index);
}

void ModelEventNotifier::ItemRemovedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_item_removed(parent, tag_index);
}

void ModelEventNotifier::DataChangedNotify(SessionItem *item, int role)
{
  p_impl->m_data_changed(item, role);
}

void ModelEventNotifier::ModelAboutToBeResetNotify(SessionModel *model)
{
  p_impl->m_model_about_to_reset(model);
}

void ModelEventNotifier::ModelResetNotify(SessionModel *model)
{
  p_impl->m_model_reset(model);
}

void ModelEventNotifier::ModelAboutToBeDestroyedNotify(SessionModel *model)
{
  p_impl->m_model_about_to_be_destroyed(model);
}

}  // namespace mvvm
