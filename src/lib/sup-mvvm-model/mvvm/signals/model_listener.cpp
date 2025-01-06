/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "model_listener.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/i_session_model.h>

namespace mvvm
{

ModelListener::ModelListener(const ISessionModel *model)
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

ModelListener::~ModelListener() = default;

const ISessionModel *ModelListener::GetModel() const
{
  return m_model;
}

ModelEventHandler *ModelListener::GetEventHandler()
{
  return m_model->GetEventHandler();
}

Slot *ModelListener::GetSlot() const
{
  return m_slot.get();
}

}  // namespace mvvm
