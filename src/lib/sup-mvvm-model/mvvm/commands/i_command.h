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

#ifndef MVVM_COMMANDS_I_COMMAND_H_
#define MVVM_COMMANDS_I_COMMAND_H_

#include <mvvm/commands/command_status.h>
#include <mvvm/model_export.h>

#include <string>

namespace mvvm
{

/**
 * @brief The ICommand class is an interface for generic command of the undo/redo framework.
 */
class MVVM_MODEL_EXPORT ICommand
{
public:
  ICommand() = default;
  virtual ~ICommand() = default;

  ICommand(const ICommand&) = delete;
  ICommand& operator=(const ICommand&) = delete;
  ICommand(ICommand&&) = delete;
  ICommand& operator=(ICommand&&) = delete;

  /**
   * @brief Executes the command.
   */
  virtual void Execute() = 0;

  /**
   * @brief Returns command status.
   */
  virtual CommandStatus GetCommandStatus() const = 0;

  /**
   * @brief Undoes command as it was before execution.
   */
  virtual void Undo() = 0;

  /**
   * @brief Checks whether the command is obsolete.
   *
   * Obsolete command is considered as unimportant, and will not be kept in the command stack.
   */
  virtual bool IsObsolete() const = 0;

  /**
   * @brief Sets obsolete flag to a given value.
   */
  virtual void SetIsObsolete(bool value) = 0;

  /**
   * @brief Returns command description.
   */
  virtual std::string GetDescription() const = 0;

  /**
   * @brief Sets command description.
   */
  virtual void SetDescription(const std::string& text) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_I_COMMAND_H_
