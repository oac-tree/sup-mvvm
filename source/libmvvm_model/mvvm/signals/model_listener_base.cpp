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

#include "mvvm/signals/model_listener_base.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/signals/model_event_handler.h>

namespace mvvm
{

struct ModelListenerBase::ModelListenerBaseImpl
{
  SessionModelInterface *m_model{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections

  ModelEventHandler *GetEventHandler() const { return m_model->GetEventHandler(); }

  Slot *GetSlot() const { return m_slot.get(); }

  explicit ModelListenerBaseImpl(SessionModelInterface *model)
      : m_model(model), m_slot(std::make_unique<Slot>())
  {
  }
};

ModelListenerBase::ModelListenerBase(SessionModelInterface *model)
    : p_impl(std::make_unique<ModelListenerBaseImpl>(model))
{
}

Connection ModelListenerBase::SetOnAboutToInsertItem(const Callbacks::item_tagindex_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<AboutToInsertItemEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->GetEventHandler()->Connect<AboutToInsertItemEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnItemInserted(const Callbacks::item_tagindex_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ItemInsertedEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->GetEventHandler()->Connect<ItemInsertedEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<AboutToRemoveItemEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->GetEventHandler()->Connect<AboutToRemoveItemEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnItemRemoved(const Callbacks::item_tagindex_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ItemRemovedEvent>(event);
    f(concrete_event.m_parent, concrete_event.m_tag_index);
  };
  return p_impl->GetEventHandler()->Connect<ItemRemovedEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnDataChanged(const Callbacks::item_int_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    f(concrete_event.m_item, concrete_event.m_data_role);
  };
  return p_impl->GetEventHandler()->Connect<DataChangedEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnModelAboutToBeReset(const Callbacks::model_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelAboutToBeResetEvent>(event);
    f(concrete_event.m_model);
  };
  return p_impl->GetEventHandler()->Connect<ModelAboutToBeResetEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnModelReset(const Callbacks::model_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelResetEvent>(event);
    f(concrete_event.m_model);
  };
  return p_impl->GetEventHandler()->Connect<ModelResetEvent>(adapter, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnModelAboutToBeDestroyed(const Callbacks::model_t &f)
{
  auto adapter = [f](const event_t &event)
  {
    auto concrete_event = std::get<ModelAboutToBeDestroyedEvent>(event);
    f(concrete_event.m_model);
  };
  return p_impl->GetEventHandler()->Connect<ModelAboutToBeDestroyedEvent>(adapter,
                                                                          p_impl->GetSlot());
}

SessionModelInterface *ModelListenerBase::GetCurrentModel() const
{
  return p_impl->m_model;
}

ModelListenerBase::~ModelListenerBase() = default;

}  // namespace mvvm
