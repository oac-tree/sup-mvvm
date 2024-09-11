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

#ifndef MVVM_COMMANDS_I_COMMAND_STACK_H_
#define MVVM_COMMANDS_I_COMMAND_STACK_H_

#include <mvvm/model_export.h>

#include <memory>
#include <vector>

namespace mvvm
{

class ICommand;

/**
 * @brief The ICommandStack class is an interface for the stack of undo/redo commands.
 *
 * It is made in a spirit of Qt's QUndoStack.
 */
class MVVM_MODEL_EXPORT ICommandStack
{
public:
  virtual ~ICommandStack() = default;

  /**
   * @brief Pushes command in the stack and immediately executes it.
   *
   * Returns pointer to this command in the case of successful command execution. Returns nullptr
   * if the command is obsolete.
   */
  virtual ICommand* Execute(std::unique_ptr<ICommand> command) = 0;

  /**
   * @brief Checks if there is a command available for undo.
   */
  virtual bool CanUndo() const = 0;

  /**
   * @brief Checks if there is a command available for redo.
   */
  virtual bool CanRedo() const = 0;

  /**
   * @brief Returns the index of the current command.
   *
   * This is the command that will be executed on the next call to Redo(). It is not always the
   * top-most command on the stack, since a number of commands may have been undone.
   */
  virtual int GetIndex() const = 0;

  /**
   * @brief  Returns number of commands in a stack.
   *
   * Macro commands are treated as single command.
   */
  virtual int GetCommandCount() const = 0;

  /**
   * @brief Returns list of all commands.
   */
  virtual std::vector<const ICommand *> GetCommands() const = 0;

  /**
   * @brief Undoes the command below the current command by calling ICommand::Undo().
   *
   * Decrements the current command index.
   */
  virtual void Undo() = 0;

  /**
   * @brief Redoes the current command by calling ICommand::Redo().
   *
   * Increments the current command index.
   */
  virtual void Redo() = 0;

  /**
   * @brief Removes all commands from the stack.
   */
  virtual void Clear() = 0;

  /**
   * @brief Sets the maximum number of commands on this stack.
   *
   * When the number of commands on a stack exceedes the stack's undoLimit, commands are deleted
   * from the bottom of the stack.
   *
   * Limit set to zero means infinite command stack (and thus potential memory overflow).
   */
  virtual void SetUndoLimit(size_t limit) = 0;

  /**
   * @brief Begins composition of a macro command with the given text description.
   */
  virtual void BeginMacro(const std::string& name) = 0;

  /**
   * @brief Ends composition of a macro command.
   */
  virtual void EndMacro() = 0;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_I_COMMAND_STACK_H_
