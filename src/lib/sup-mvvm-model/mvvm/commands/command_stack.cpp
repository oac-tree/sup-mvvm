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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "command_stack.h"

#include "i_command.h"
#include "macro_command.h"

#include <mvvm/core/exceptions.h>

#include <algorithm>
#include <list>
#include <stack>

namespace mvvm
{

struct CommandStack::CommandStackImpl
{
  std::list<std::unique_ptr<ICommand>> m_commands;
  std::stack<MacroCommand *> m_macro_stack;

  //!< Position in the command list. This command will be redone on the next call to Redo().
  std::list<std::unique_ptr<ICommand>>::const_iterator m_pos;

  size_t m_undo_limit{0};  // no limit by default
  bool m_operation_is_in_progress{false};

  CommandStackImpl() { m_pos = m_commands.cend(); }

  /**
   * @brief Sets operation in progress flag to a given value.
   *
   * Will throw on attempt to set the same flag as before. See @IsOperationInProgress() for more
   * explanations.
   */
  void SetInProgress(bool value)
  {
    if (m_operation_is_in_progress == value)
    {
      throw RuntimeException(
          "Attempt to execute new command before current one has finished its work");
    }

    m_operation_is_in_progress = value;
  }

  /**
   * @brief Checks if we are in the middle of undo/redo operation.
   *
   * It happens when ICommand::UndoImpl() or ICommand::RedoImpl() calls back to the command stack.
   * For example, at the moment of undo of some macro, some of the command has tried to create
   * another macro. This means vicious model/view callback circle somewhere.
   */
  bool IsOperationInProgress() const { return m_operation_is_in_progress; }

  /**
   * @brief Saves a command in the command stack.
   */
  void SaveSingleCommand(std::unique_ptr<ICommand> command)
  {
    // we do not allow command tree branching: removing commands from the 'next redo` position till
    // the end
    m_commands.erase(m_pos, m_commands.cend());

    m_commands.emplace_back(std::move(command));
    m_pos = m_commands.cend();
  }

  /**
   * @brief Saves a command in macro.
   *
   * Command will be added to the macro command, and not directly to the stack.
   */
  void SaveMacroCommand(std::unique_ptr<ICommand> command)
  {
    auto last_macro = m_macro_stack.top();
    last_macro->Append(std::move(command));
  }

  void SaveCommand(std::unique_ptr<ICommand> command, bool macro_mode)
  {
    if (macro_mode)
    {
      SaveMacroCommand(std::move(command));
    }
    else
    {
      SaveSingleCommand(std::move(command));
    }
  }

  size_t GetIndex() const { return std::distance(m_commands.cbegin(), m_pos); }

  /**
   * @brief Cleanup commands if they count exeeds number of commands.
   */
  void AssureCommandLimit()
  {
    if (m_undo_limit == 0 || m_undo_limit >= m_commands.size())
    {
      return;
    }

    // Ideally, we want to remove commands from the beginning of the list if their count exceeds the
    // limit. But we don't want to cross current position in the command list. This mean that we
    // allow only erasing items which are in AfterExecute state, and we disallow erasing items in
    // AfterUndo state.
    const size_t del_count = std::min(m_commands.size() - m_undo_limit, GetIndex());
    for (size_t index = 0; index < del_count; ++index)
    {
      m_commands.pop_front();
    }
  }
};

CommandStack::CommandStack() : p_impl(std::make_unique<CommandStackImpl>()) {}

CommandStack::~CommandStack() = default;

ICommand *CommandStack::Execute(std::unique_ptr<ICommand> command)
{
  if (command->IsObsolete())
  {
    throw RuntimeException("Attempt to insert obsolete command");
  }

  p_impl->SetInProgress(true);
  command->Execute();
  p_impl->SetInProgress(false);

  if (command->IsObsolete())
  {
    return nullptr;
  }

  auto command_ptr = command.get();
  p_impl->SaveCommand(std::move(command), IsMacroMode());

  p_impl->AssureCommandLimit();

  return command_ptr;
}

bool CommandStack::CanUndo() const
{
  return p_impl->m_pos != p_impl->m_commands.cbegin() && !IsMacroMode();
}

bool CommandStack::CanRedo() const
{
  return p_impl->m_pos != p_impl->m_commands.cend() && !IsMacroMode();
}

int CommandStack::GetIndex() const
{
  return static_cast<int>(p_impl->GetIndex());
}

int CommandStack::GetCommandCount() const
{
  return static_cast<int>(p_impl->m_commands.size());
}

std::vector<const ICommand *> CommandStack::GetCommands() const
{
  std::vector<const ICommand *> result;

  std::transform(p_impl->m_commands.begin(), p_impl->m_commands.end(), std::back_inserter(result),
                 [](auto &iter) { return iter.get(); });

  return result;
}

void CommandStack::Undo()
{
  if (CanUndo())
  {
    p_impl->m_pos--;
    p_impl->SetInProgress(true);
    (*p_impl->m_pos)->Undo();
    p_impl->SetInProgress(false);
    p_impl->AssureCommandLimit();
  }
}

void CommandStack::Redo()
{
  if (CanRedo())
  {
    p_impl->SetInProgress(true);
    (*p_impl->m_pos)->Execute();
    p_impl->SetInProgress(false);
    p_impl->m_pos++;
    p_impl->AssureCommandLimit();
  }
}

void CommandStack::Clear()
{
  p_impl->m_commands.clear();
  p_impl->m_pos = p_impl->m_commands.cend();
}

void CommandStack::SetUndoLimit(size_t limit)
{
  p_impl->m_undo_limit = limit;
  p_impl->AssureCommandLimit();
}

void CommandStack::BeginMacro(const std::string &name)
{
  if (p_impl->IsOperationInProgress())
  {
    throw RuntimeException(
        "Attempt to start macro operation before current command has finished its work");
  }

  const bool nested_macro_mode = IsMacroMode();

  auto macro = std::make_unique<MacroCommand>(name);
  auto macro_ptr = macro.get();

  p_impl->SaveCommand(std::move(macro), nested_macro_mode);

  p_impl->m_macro_stack.push(macro_ptr);
}

void CommandStack::EndMacro()
{
  if (p_impl->IsOperationInProgress())
  {
    throw RuntimeException(
        "Attempt to stop macro operation before current command has finished its work");
  }

  if (!IsMacroMode())
  {
    throw RuntimeException("No command macro is currently recording");
  }

  // removing macro pointer from macro stack, the macro command itself remains in the command stack
  p_impl->m_macro_stack.pop();
}

bool CommandStack::IsMacroMode() const
{
  return !p_impl->m_macro_stack.empty();
}

}  // namespace mvvm
