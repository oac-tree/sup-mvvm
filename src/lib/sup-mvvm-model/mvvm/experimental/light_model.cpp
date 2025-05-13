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

#include "light_model.h"

#include "light_command.h"
#include "light_item.h"

#include <mvvm/commands/i_command_stack.h>

namespace mvvm::experimental
{

LightModel::LightModel(notify_func_t notify_func)
    : m_root(std::make_unique<LightItem>()), m_notify_func(std::move(notify_func))
{
}

LightModel::~LightModel() = default;

bool LightModel::SetData(ILightItem *item, const variant_t &value, int32_t role)
{
  return item->SetData(value, role);
}

void LightModel::ExecuteCommand(std::unique_ptr<LightCommand> command)
{
  Notify(command->GetNextEvent());
  command->Execute();
  Notify(command->GetNextEvent());
}

void LightModel::Notify(const std::optional<event_variant_t> &optional_event)
{
  if (m_notify_func && optional_event.has_value())
  {
    m_notify_func(optional_event.value());
  }
}

}  // namespace mvvm::experimental
