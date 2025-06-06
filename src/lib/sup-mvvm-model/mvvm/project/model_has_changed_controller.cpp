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

#include "model_has_changed_controller.h"

#include <mvvm/model/tagindex.h>

namespace mvvm
{

//! Constructor of ModelHasChangedController.
//! Acccept 'model' to listen, and a 'callback' to report about changes in a model.

ModelHasChangedController::ModelHasChangedController(ISessionModel* model,
                                                     const callback_t& callback)
    : ModelListener(model), m_callback(callback)
{
  Connect<DataChangedEvent>([this](auto) { OnChange(); });
  Connect<ItemInsertedEvent>([this](auto) { OnChange(); });
  Connect<ItemRemovedEvent>([this](auto) { OnChange(); });
  Connect<ModelResetEvent>([this](auto) { OnChange(); });
}

//! Returns true if the model was changed since last call of resetChanged.

bool ModelHasChangedController::IsChanged() const
{
  return m_has_changed;
}

//! Reset has_changed flag.

void ModelHasChangedController::ResetIsChanged()
{
  m_has_changed = false;
}

//! Sets 'has_changed' flag and reports back to client.

void ModelHasChangedController::OnChange()
{
  m_has_changed = true;
  if (m_callback)
  {
    m_callback();
  }
}

}  // namespace mvvm
