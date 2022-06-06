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

#include "mvvm/interfaces/model_event_subscriber_interface.h"
#include "mvvm/model/application_model.h"

namespace mvvm
{

struct ModelListenerBase::ModelListenerBaseImpl
{
  ApplicationModel *m_model{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections

  ModelEventSubscriberInterface *GetSubscriber() const { return m_model->GetSubscriber(); }

  Slot *GetSlot() const { return m_slot.get(); }

  explicit ModelListenerBaseImpl(ApplicationModel *model)
      : m_model(model), m_slot(std::make_unique<Slot>())
  {
  }
};

ModelListenerBase::ModelListenerBase(ApplicationModel *model)
    : p_impl(std::make_unique<ModelListenerBaseImpl>(model))
{
}

Connection ModelListenerBase::SetOnAboutToInsertItem(const Callbacks::item_tagindex_t &f)
{
  return p_impl->GetSubscriber()->SetOnAboutToInsertItem(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnItemInserted(const Callbacks::item_tagindex_t &f)
{
  return p_impl->GetSubscriber()->SetOnItemInserted(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnAboutToRemoveItem(const Callbacks::item_tagindex_t &f)
{
  return p_impl->GetSubscriber()->SetOnAboutToRemoveItem(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnItemRemoved(const Callbacks::item_tagindex_t &f)
{
  return p_impl->GetSubscriber()->SetOnItemRemoved(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnDataChanged(const Callbacks::item_int_t &f)
{
  return p_impl->GetSubscriber()->SetOnDataChanged(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnModelAboutToBeReset(const Callbacks::model_t &f)
{
  return p_impl->GetSubscriber()->SetOnModelAboutToBeReset(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnModelReset(const Callbacks::model_t &f)
{
  return p_impl->GetSubscriber()->SetOnModelReset(f, p_impl->GetSlot());
}

Connection ModelListenerBase::SetOnModelAboutToBeDestroyed(const Callbacks::model_t &f)
{
  return p_impl->GetSubscriber()->SetOnModelAboutToBeDestroyed(f, p_impl->GetSlot());
}

ApplicationModel *ModelListenerBase::GetCurrentModel() const
{
  return p_impl->m_model;
}

ModelListenerBase::~ModelListenerBase() = default;

}  // namespace mvvm
