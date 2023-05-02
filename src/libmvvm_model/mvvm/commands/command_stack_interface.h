/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_COMMANDS_COMMAND_STACK_INTERFACE_H_
#define MVVM_COMMANDS_COMMAND_STACK_INTERFACE_H_

#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{

class CommandInterface;

//! Interface class for command stack.

class MVVM_MODEL_EXPORT CommandStackInterface
{
public:
  virtual ~CommandStackInterface() = default;

  //! Push command in the stack and immediately executes it.
  //! Returns pointer to this command, if command was executed successfully. Returns nullptr if
  //! command is obsolete.
  virtual CommandInterface* Execute(std::unique_ptr<CommandInterface> command) = 0;

  //! Returns true if there is a command available for undo; otherwise returns false.
  virtual bool CanUndo() const = 0;

  //! Returns true if there is a command available for redo; otherwise returns false.
  virtual bool CanRedo() const = 0;

  //! Returns the index of the current command. This is the command that will be executed on the
  //! next call to Redo(). It is not always the top-most command on the stack, since a number of
  //! commands may have been undone.
  virtual int GetIndex() const = 0;

  //! Returns number of commands in a stack. Macro commands are treated as single command.
  virtual int GetSize() const = 0;

  virtual void Undo() = 0;

  virtual void Redo() = 0;

  virtual void Clear() = 0;

  virtual void SetUndoLimit(int limit) = 0;

  virtual void BeginMacro(const std::string& name) = 0;

  virtual void EndMacro() = 0;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_STACK_INTERFACE_H_
