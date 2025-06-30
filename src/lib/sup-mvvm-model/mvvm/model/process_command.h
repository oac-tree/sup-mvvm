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

#include <mvvm/model/i_session_model.h>

namespace mvvm
{

/**
 * @brief Processes the command through the model and returns result to the user.
 */
template <typename C, typename... Args>
typename C::result_t ProcessCommand(ISessionModel& model, Args&&... args)
{
  auto command = std::make_unique<C>(std::forward<Args>(args)...);
  // check if the model was able to execute the command and it wasn't expired
  if (auto processed_command = static_cast<C*>(model.ExecuteCommand(std::move(command)));
      processed_command)
  {
    return processed_command->GetResult();
  }
  return {};
}

}  // namespace mvvm

#endif  // MVVM_MODEL_FUNCTION_TYPES_H_
