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

  connections.emplace_back(p_impl->m_about_to_insert_item.connect(
      listener, &ModelEventListenerInterface::OnAboutToInsertItem));

  connections.emplace_back(
      p_impl->m_item_inserted.connect(listener, &ModelEventListenerInterface::OnItemInserted));

  connections.emplace_back(p_impl->m_about_to_remove_item.connect(
      listener, &ModelEventListenerInterface::OnAboutToRemoveItem));

  connections.emplace_back(
      p_impl->m_item_removed.connect(listener, &ModelEventListenerInterface::OnItemRemoved));

  connections.emplace_back(
      p_impl->m_data_changed.connect(listener, &ModelEventListenerInterface::OnDataChanged));

  connections.emplace_back(p_impl->m_model_about_to_reset.connect(
      listener, &ModelEventListenerInterface::OnModelAboutToBeReset));

  connections.emplace_back(
      p_impl->m_model_reset.connect(listener, &ModelEventListenerInterface::OnModelReset));

  connections.emplace_back(p_impl->m_model_about_to_be_destroyed.connect(
      listener, &ModelEventListenerInterface::OnModelAboutToBeDestroyed));

  p_impl->m_connections.insert(p_impl->m_connections.find(listener), {listener, connections});

  listener->SetNotifier(this);
}

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
