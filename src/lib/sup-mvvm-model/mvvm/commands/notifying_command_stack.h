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

#ifndef MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_
#define MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The NotifyingCommandStack class extends command stack with notification capabilities.
 */
class MVVM_MODEL_EXPORT NotifyingCommandStack : public ICommandStack
{
public:
  explicit NotifyingCommandStack(ICommandStack* decoratee);
  ~NotifyingCommandStack() override;

  ICommand* Execute(std::unique_ptr<ICommand> command) override;
  bool CanUndo() const override;
  bool CanRedo() const override;
  int GetIndex() const override;
  int GetCommandCount() const override;
  std::vector<const ICommand*> GetCommands() const override;
  void Undo() override;
  void Redo() override;
  void Clear() override;
  void SetUndoLimit(std::size_t limit) override;
  void BeginMacro(const std::string& name) override;
  void EndMacro() override;

private:
  ICommand* GetNextUndoCommand();
  ICommand* GetNextRedoCommand();
  void NotifyBefore(ICommand* command);
  void NotifyAfter(ICommand* command);

  ICommandStack* m_decoratee{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_
