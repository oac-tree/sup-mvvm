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

#ifndef MVVM_MODEL_PROCESS_COMMAND_H_
#define MVVM_MODEL_PROCESS_COMMAND_H_

//! @file
//! Helper functions to process commands.

#include <mvvm/core/variant.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/signals/event_types.h>

#include <functional>
#include <memory>

namespace mvvm
{

std::function<void(const mvvm::event_variant_t&)> CreateNotifierCallback(
    mvvm::ModelEventHandler* event_handler);

/**
 * @brief Processes the command and returns result to the user.
 *
 * If model exists, the command will be passed to command processor (with notification
 * and possible undo/redo).
 */
template <typename C, typename... Args>
typename C::result_t ProcessCommand(ISessionModel* model, Args&&... args)
{
  if (model)
  {
    auto command = std::make_unique<C>(std::forward<Args>(args)...);
    // FIXME get rid of this setter, it is slow
    command->SetNotificationCallback(CreateNotifierCallback(model->GetEventHandler()));

    // check if the model was able to execute the command and it wasn't expired
    if (auto processed_command = static_cast<C*>(model->ExecuteCommand(std::move(command)));
        processed_command)
    {
      return processed_command->GetResult();
    }
    return {};
  }

  // in the absence of the model immediately execute command
  C command(std::forward<Args>(args)...);
  command.Execute();
  return command.GetResult();
}

}  // namespace mvvm

#endif  // MVVM_MODEL_FUNCTION_TYPES_H_
