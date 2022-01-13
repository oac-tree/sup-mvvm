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

namespace mvvm
{
ModelEventNotifier::ModelEventNotifier(ModelEventListenerInterface *listener)
{
  if (listener)
  {
    Subscribe(listener);
  }
}

ModelEventNotifier::~ModelEventNotifier()
{
  for (auto it : m_connections)
  {
    // tell all listeners not to bother us with Unsubscribe on their own deletion
    it.first->SetNotifier(nullptr);
  }
}

void ModelEventNotifier::Unsubscribe(ModelEventListenerInterface *listener)
{
  auto it = m_connections.find(listener);
  if (it == m_connections.end())
  {
    throw std::runtime_error("Error in ModelEventNotifier: no such listener exists");
  }

  for (auto &conn : it->second)
  {
    QObject::disconnect(conn);
  }
  m_connections.erase(it);

  listener->SetNotifier(nullptr);
}

//! Subscribe listener for our notifications. Since listener is not QObject, we have to store
//! connections explicitely, to be able to disconnect later, if necessary.

void ModelEventNotifier::Subscribe(ModelEventListenerInterface *listener)
{
  if (m_connections.find(listener) != m_connections.end())
  {
    throw std::runtime_error("Error in ModelEventNotifier: item is already connected");
  }

  std::vector<QMetaObject::Connection> connections;

  auto on_about_to_insert = [listener](auto parent, auto tag_index)
  { listener->OnAboutToInsertItem(parent, tag_index); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::AboutToInsertItem, on_about_to_insert));

  auto on_inserted = [listener](auto parent, auto tag_index)
  { listener->OnItemInserted(parent, tag_index); };
  connections.emplace_back(connect(this, &ModelEventNotifier::ItemInserted, on_inserted));

  auto on_about_to_remove = [listener](auto parent, auto tag_index)
  { listener->OnAboutToRemoveItem(parent, tag_index); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::AboutToRemoveItem, on_about_to_remove));

  auto on_removed = [listener](auto parent, auto tag_index)
  { listener->OnItemRemoved(parent, tag_index); };
  connections.emplace_back(connect(this, &ModelEventNotifier::ItemRemoved, on_removed));

  auto on_data_changed = [listener](auto item, int role) { listener->OnDataChanged(item, role); };
  connections.emplace_back(connect(this, &ModelEventNotifier::DataChanged, on_data_changed));

  auto on_model_about_reset = [listener](auto model) { listener->OnModelAboutToBeReset(model); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::ModelAboutToBeReset, on_model_about_reset));

  auto on_model_reset = [listener](auto model) { listener->OnModelReset(model); };
  connections.emplace_back(connect(this, &ModelEventNotifier::ModelReset, on_model_reset));

  auto on_model_destroyed = [listener](auto model) { listener->OnModelAboutToBeDestroyed(model); };
  connections.emplace_back(
      connect(this, &ModelEventNotifier::ModelAboutToBeDestroyed, on_model_destroyed));

  m_connections.insert(m_connections.find(listener), {listener, connections});

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

}  // namespace ModelView
