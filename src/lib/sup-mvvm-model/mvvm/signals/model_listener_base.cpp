/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "model_listener_base.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/sessionmodel_interface.h>

namespace mvvm
{

struct ModelListenerBase::ModelListenerBaseImpl
{
  const SessionModelInterface *m_model{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections

  ModelEventHandler *GetEventHandler() const { return m_model->GetEventHandler(); }

  Slot *GetSlot() const { return m_slot.get(); }

  explicit ModelListenerBaseImpl(const SessionModelInterface *model)
      : m_model(model), m_slot(std::make_unique<Slot>())
  {
    if (!m_model)
    {
      throw NullArgumentException("Attempt to set-up listener for uninitialized model");
    }

    if (!m_model->GetEventHandler())
    {
      throw NullArgumentException(
          "Attempt to set-up listener for the model which lacks event handler");
    }
  }
};

ModelListenerBase::ModelListenerBase(const SessionModelInterface *model)
    : p_impl(std::make_unique<ModelListenerBaseImpl>(model))
{
}

ModelListenerBase::~ModelListenerBase() = default;

const SessionModelInterface *ModelListenerBase::GetModelBase() const
{
  return p_impl->m_model;
}

ModelEventHandler *ModelListenerBase::GetEventHandler()
{
  return p_impl->GetEventHandler();
}

Slot *ModelListenerBase::GetSlot() const
{
  return p_impl->GetSlot();
}

}  // namespace mvvm
