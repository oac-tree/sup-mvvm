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

#ifndef MVVM_COMMANDS_COMMAND_STACK_H_
#define MVVM_COMMANDS_COMMAND_STACK_H_

#include <mvvm/commands/i_command_stack.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The CommandStack class is the default implementation of undo/redo command stack.
 */
class MVVM_MODEL_EXPORT CommandStack : public ICommandStack
{
public:
  CommandStack();
  ~CommandStack() override;

  ICommand *Execute(std::unique_ptr<ICommand> command) override;
  bool CanUndo() const override;
  bool CanRedo() const override;
  int GetIndex() const override;
  int GetCommandCount() const override;
  void Undo() override;
  void Redo() override;
  void Clear() override;
  void SetUndoLimit(int limit) override;
  void BeginMacro(const std::string &name) override;
  void EndMacro() override;

  /**
   * @brief Checks if we are currently recording a macro command.
   */
  bool IsMacroMode() const;

private:
  struct CommandStackImpl;
  std::unique_ptr<CommandStackImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_STACK_H_
