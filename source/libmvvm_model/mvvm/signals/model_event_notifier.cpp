/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/signals/model_event_notifier.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/signals/event_handler.h>
#include <mvvm/signals/signal_slot.h>

namespace mvvm
{
struct ModelEventNotifier::ModelEventNotifierImpl
{
  Signal<void(SessionItem *, const TagIndex &)> m_about_to_insert_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_inserted;
  Signal<void(SessionItem *, const TagIndex &)> m_about_to_remove_item;
  Signal<void(SessionItem *, const TagIndex &)> m_item_removed;
  Signal<void(SessionItem *, int)> m_data_changed;
  Signal<void(SessionModelInterface *)> m_model_about_to_reset;
  Signal<void(SessionModelInterface *)> m_model_reset;
  Signal<void(SessionModelInterface *)> m_model_about_to_be_destroyed;
  EventHandler m_event_handler;
};

ModelEventNotifier::ModelEventNotifier() : p_impl(std::make_unique<ModelEventNotifierImpl>()) {}

ModelEventNotifier::~ModelEventNotifier() = default;

Connection ModelEventNotifier::SetOnAboutToInsertItem(const Callbacks::item_tagindex_t &f,
                                                      Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<AboutToInsertItemEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->m_event_handler.Connect<AboutToInsertItemEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnItemInserted(const Callbacks::item_tagindex_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ItemInsertedEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->m_event_handler.Connect<ItemInsertedEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t &f,
                                                      Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<AboutToRemoveItemEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->m_event_handler.Connect<AboutToRemoveItemEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnItemRemoved(const Callbacks::item_tagindex_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ItemRemovedEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->m_event_handler.Connect<ItemRemovedEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnDataChanged(const Callbacks::item_int_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    f(concrete_event.m_item, concrete_event.m_data_role);
  };
  return p_impl->m_event_handler.Connect<DataChangedEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnModelAboutToBeReset(const Callbacks::model_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelAboutToBeResetEvent>(event);
    f(concrete_event.m_model);
  };
  return p_impl->m_event_handler.Connect<ModelAboutToBeResetEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnModelReset(const Callbacks::model_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelResetEvent>(event);
    f(concrete_event.m_model);
  };
  return p_impl->m_event_handler.Connect<ModelResetEvent>(adapter, slot);
}

Connection ModelEventNotifier::SetOnModelAboutToBeDestroyed(const Callbacks::model_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelAboutToBeDestroyedEvent>(event);
    f(concrete_event.m_model);
  };
  return p_impl->m_event_handler.Connect<ModelAboutToBeDestroyedEvent>(adapter, slot);
}

// ------------------------------------------------------------------------

void ModelEventNotifier::AboutToInsertItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_event_handler.Notify<AboutToInsertItemEvent>(parent, tag_index);
}

void ModelEventNotifier::ItemInsertedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_event_handler.Notify<ItemInsertedEvent>(parent, tag_index);
}

void ModelEventNotifier::AboutToRemoveItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_event_handler.Notify<AboutToRemoveItemEvent>(parent, tag_index);
}

void ModelEventNotifier::ItemRemovedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  p_impl->m_event_handler.Notify<ItemRemovedEvent>(parent, tag_index);
}

void ModelEventNotifier::DataChangedNotify(SessionItem *item, int role)
{
  p_impl->m_event_handler.Notify<DataChangedEvent>(item, role);
}

void ModelEventNotifier::ModelAboutToBeResetNotify(SessionModelInterface *model)
{
  p_impl->m_event_handler.Notify<ModelAboutToBeResetEvent>(model);
}

void ModelEventNotifier::ModelResetNotify(SessionModelInterface *model)
{
  p_impl->m_event_handler.Notify<ModelResetEvent>(model);
}

void ModelEventNotifier::ModelAboutToBeDestroyedNotify(SessionModelInterface *model)
{
  p_impl->m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(model);
}

}  // namespace mvvm
