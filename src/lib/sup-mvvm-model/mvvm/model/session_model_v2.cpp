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

#include "session_model_v2.h"

#include "session_item.h"

#include <mvvm/commands/default_command_processor.h>
#include <mvvm/commands/i_command.h>

namespace mvvm
{

SessionModelV2::SessionModelV2() : m_command_stack(std::make_unique<DefaultCommandProcessor>()) {}

ICommand *SessionModelV2::ExecuteCommand(std::unique_ptr<ICommand> command)
{
  return m_command_stack->Execute(std::move(command));
}

bool SessionModelV2::SetData(SessionItem *item, const variant_t &value, int role)
{
  return item->SetDataV2(value, role);
}

}  // namespace mvvm
