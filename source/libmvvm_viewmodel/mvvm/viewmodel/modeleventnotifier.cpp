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

#include "mvvm/viewmodel/modeleventnotifier.h"

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/signals/signalslot.h"

namespace mvvm
{
struct ModelEventNotifier::ModelEventNotifierImpl
{
  std::map<ModelEventListenerInterface *, std::vector<QMetaObject::Connection>> m_connections;
  std::map<ModelEventListenerInterface *, std::vector<Connection>> m_connections2;

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

//  for (auto it : p_impl->m_connections2)
//  {
//    // tell all listeners not to bother us with Unsubscribe on their own deletion
//    it.first->SetNotifier(nullptr);
//  }

}

void ModelEventNotifier::Unsubscribe(ModelEventListenerInterface *listener)
{
  auto it = p_impl->m_connections.find(listener);
  if (it == p_impl->m_connections.end())
  {
    throw std::runtime_error("Error in ModelEventNotifier: no such listener exists");
  }

  for (auto &conn : it->second)
  {
    QObject::disconnect(conn);
  }
  p_impl->m_connections.erase(it);

  listener->SetNotifier(nullptr);

//  // ---
//  auto it2 = p_impl->m_connections2.find(listener);
//  if (it2 == p_impl->m_connections2.end())
//  {
//    throw std::runtime_error("Error in ModelEventNotifier: no such listener exists");
//  }

//  for (auto conn : it2->second)
//  {
//    p_impl->disconnect(conn);
//  }
//  p_impl->m_connections2.erase(it2);

//  listener->SetNotifier(nullptr);

}

//! Subscribe listener for our notifications. Since listener is not QObject, we have to store
//! connections explicitely, to be able to disconnect later, if necessary.

void ModelEventNotifier::Subscribe(ModelEventListenerInterface *listener)
{
  if (p_impl->m_connections.find(listener) != p_impl->m_connections.end())
  {
    throw std::runtime_error("Error in ModelEventNotifier: item is already connected");
  }

  std::vector<QMetaObject::Connection> connections;

  auto on_about_to_insert = [listener](auto parent, auto tag_index)
  { listener->OnAboutToInsertItem(parent, tag_index); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::AboutToInsertItem, on_about_to_insert));
  p_impl->m_about_to_insert_item.connect(listener,
                                         &ModelEventListenerInterface::OnAboutToInsertItem);

  auto on_inserted = [listener](auto parent, auto tag_index)
  { listener->OnItemInserted(parent, tag_index); };
  connections.emplace_back(connect(this, &ModelEventNotifier::ItemInserted, on_inserted));
  p_impl->m_item_inserted.connect(listener, &ModelEventListenerInterface::OnItemInserted);

  auto on_about_to_remove = [listener](auto parent, auto tag_index)
  { listener->OnAboutToRemoveItem(parent, tag_index); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::AboutToRemoveItem, on_about_to_remove));
  p_impl->m_about_to_remove_item.connect(listener,
                                         &ModelEventListenerInterface::OnAboutToRemoveItem);

  auto on_removed = [listener](auto parent, auto tag_index)
  { listener->OnItemRemoved(parent, tag_index); };
  connections.emplace_back(connect(this, &ModelEventNotifier::ItemRemoved, on_removed));
  p_impl->m_item_removed.connect(listener, &ModelEventListenerInterface::OnItemRemoved);

  auto on_data_changed = [listener](auto item, int role) { listener->OnDataChanged(item, role); };
  connections.emplace_back(connect(this, &ModelEventNotifier::DataChanged, on_data_changed));
  p_impl->m_data_changed.connect(listener, &ModelEventListenerInterface::OnDataChanged);

  auto on_model_about_reset = [listener](auto model) { listener->OnModelAboutToBeReset(model); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::ModelAboutToBeReset, on_model_about_reset));
  p_impl->m_model_about_to_reset.connect(listener,
                                         &ModelEventListenerInterface::OnModelAboutToBeReset);

  auto on_model_reset = [listener](auto model) { listener->OnModelReset(model); };
  connections.emplace_back(connect(this, &ModelEventNotifier::ModelReset, on_model_reset));
  p_impl->m_model_reset.connect(listener, &ModelEventListenerInterface::OnModelReset);

  auto on_model_destroyed = [listener](auto model) { listener->OnModelAboutToBeDestroyed(model); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::ModelAboutToBeDestroyed, on_model_destroyed));
  p_impl->m_model_about_to_be_destroyed.connect(
      listener, &ModelEventListenerInterface::OnModelAboutToBeDestroyed);

  p_impl->m_connections.insert(p_impl->m_connections.find(listener), {listener, connections});

  listener->SetNotifier(this);
}

void ModelEventNotifier::AboutToInsertItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit AboutToInsertItem(parent, tag_index);
}

void ModelEventNotifier::ItemInsertedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit ItemInserted(parent, tag_index);
}

void ModelEventNotifier::AboutToRemoveItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit AboutToRemoveItem(parent, tag_index);
}

void ModelEventNotifier::ItemRemovedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  emit ItemRemoved(parent, tag_index);
}

void ModelEventNotifier::DataChangedNotify(SessionItem *item, int role)
{
  emit DataChanged(item, role);
}

void ModelEventNotifier::ModelAboutToBeResetNotify(SessionModel *model)
{
  emit ModelAboutToBeReset(model);
}

void ModelEventNotifier::ModelResetNotify(SessionModel *model)
{
  emit ModelReset(model);
}

void ModelEventNotifier::ModelAboutToBeDestroyedNotify(SessionModel *model)
{
  emit ModelAboutToBeDestroyed(model);
}

}  // namespace mvvm
