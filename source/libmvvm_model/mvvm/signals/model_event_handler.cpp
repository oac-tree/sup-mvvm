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

#include "mvvm/signals/model_event_handler.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/signals/signal_slot.h>

namespace mvvm
{

ModelEventHandler::ModelEventHandler()
{
  Register<DataChangedEvent>();
  Register<AboutToInsertItemEvent>();
  Register<ItemInsertedEvent>();
  Register<AboutToRemoveItemEvent>();
  Register<ItemRemovedEvent>();
  Register<ModelAboutToBeResetEvent>();
  Register<ModelResetEvent>();
  Register<ModelAboutToBeDestroyedEvent>();
}

Connection ModelEventHandler::SetOnAboutToInsertItem(const Callbacks::item_tagindex_t &f,
                                                     Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<AboutToInsertItemEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return Connect<AboutToInsertItemEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnItemInserted(const Callbacks::item_tagindex_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ItemInsertedEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return Connect<ItemInsertedEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t &f,
                                                     Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<AboutToRemoveItemEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return Connect<AboutToRemoveItemEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnItemRemoved(const Callbacks::item_tagindex_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ItemRemovedEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return Connect<ItemRemovedEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnDataChanged(const Callbacks::item_int_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    f(concrete_event.m_item, concrete_event.m_data_role);
  };
  return Connect<DataChangedEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnModelAboutToBeReset(const Callbacks::model_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelAboutToBeResetEvent>(event);
    f(concrete_event.m_model);
  };
  return Connect<ModelAboutToBeResetEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnModelReset(const Callbacks::model_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelResetEvent>(event);
    f(concrete_event.m_model);
  };
  return Connect<ModelResetEvent>(adapter, slot);
}

Connection ModelEventHandler::SetOnModelAboutToBeDestroyed(const Callbacks::model_t &f, Slot *slot)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelAboutToBeDestroyedEvent>(event);
    f(concrete_event.m_model);
  };
  return Connect<ModelAboutToBeDestroyedEvent>(adapter, slot);
}

// ------------------------------------------------------------------------

void ModelEventHandler::AboutToInsertItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  Notify<AboutToInsertItemEvent>(parent, tag_index);
}

void ModelEventHandler::ItemInsertedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  Notify<ItemInsertedEvent>(parent, tag_index);
}

void ModelEventHandler::AboutToRemoveItemNotify(SessionItem *parent, const TagIndex &tag_index)
{
  Notify<AboutToRemoveItemEvent>(parent, tag_index);
}

void ModelEventHandler::ItemRemovedNotify(SessionItem *parent, const TagIndex &tag_index)
{
  Notify<ItemRemovedEvent>(parent, tag_index);
}

void ModelEventHandler::DataChangedNotify(SessionItem *item, int role)
{
  Notify<DataChangedEvent>(item, role);
}

void ModelEventHandler::ModelAboutToBeResetNotify(SessionModelInterface *model)
{
  Notify<ModelAboutToBeResetEvent>(model);
}

void ModelEventHandler::ModelResetNotify(SessionModelInterface *model)
{
  Notify<ModelResetEvent>(model);
}

void ModelEventHandler::ModelAboutToBeDestroyedNotify(SessionModelInterface *model)
{
  Notify<ModelAboutToBeDestroyedEvent>(model);
}

}  // namespace mvvm
