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

#ifndef MVVM_COMMANDS_DEFAULT_COMMAND_PROCESSOR_H_
#define MVVM_COMMANDS_DEFAULT_COMMAND_PROCESSOR_H_

#include <mvvm/commands/i_command_stack.h>

namespace mvvm
{

/**
 * @brief The DefaultCommandProcessor class immediately executes a command and does not store it
 * after.
 *
 * It doesn't havy any notification capabilities.
 */
class MVVM_MODEL_EXPORT DefaultCommandProcessor : public ICommandStack
{
public:
  DefaultCommandProcessor() = default;
  ~DefaultCommandProcessor() override = default;

  DefaultCommandProcessor(const DefaultCommandProcessor&) = delete;
  DefaultCommandProcessor& operator=(const DefaultCommandProcessor&) = delete;
  DefaultCommandProcessor(DefaultCommandProcessor&&) = delete;
  DefaultCommandProcessor& operator=(DefaultCommandProcessor&&) = delete;

  ICommand* Execute(std::unique_ptr<ICommand> command) override;

  bool CanUndo() const override;

  bool CanRedo() const override;

  std::size_t GetIndex() const override;

  std::size_t GetCommandCount() const override;

  void Undo() override;

  void Redo() override;

  void Clear() override;

  void SetUndoLimit(std::size_t limit) override;

  void BeginMacro(const std::string& name) override;

  void EndMacro() override;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_DEFAULT_COMMAND_PROCESSOR_H_
