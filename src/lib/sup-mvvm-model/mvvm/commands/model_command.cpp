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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "model_command.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/signals/model_event_handler.h>

namespace mvvm
{

ModelCommand::ModelCommand(ISessionModel *model) : m_model(model)
{
  if (!m_model)
  {
    throw RuntimeException("Model is not defined");
  }

  if (!m_model->GetEventHandler())
  {
    throw RuntimeException("Model doesn't have notification capabilities");
  }
}

void ModelCommand::Notify(const std::optional<event_variant_t> &event)
{
  if (event.has_value())
  {
    m_model->GetEventHandler()->Notify(event.value());
  }
}

}  // namespace mvvm
