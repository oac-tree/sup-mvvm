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

  // Points to the position in the list corresponding to the command which will be redone on the
  // next call to Redo()
  std::list<std::unique_ptr<ICommand>>::iterator m_pos;  //!< position in the command list

  CommandStackImpl() { m_pos = m_commands.end(); }

  /**
   * @brief Saves a command in the command stack.
   */
  void SaveSingleCommand(std::unique_ptr<ICommand> command)
  {
    // we do not allow command tree branching: removing commands from the 'next redo` position till
    // the end
    m_commands.erase(m_pos, m_commands.end());

    m_commands.emplace_back(std::move(command));
    m_pos = m_commands.end();
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
};

CommandStack::CommandStack() : p_impl(std::make_unique<CommandStackImpl>()) {}

CommandStack::~CommandStack() = default;

ICommand *CommandStack::Execute(std::unique_ptr<ICommand> command)
{
  if (command->IsObsolete())
  {
    throw RuntimeException("Attempt to insert obsolete command");
  }

  command->Execute();

  if (command->IsObsolete())
  {
    return nullptr;
  }

  auto command_ptr = command.get();
  p_impl->SaveCommand(std::move(command), IsMacroMode());

  return command_ptr;
}

bool CommandStack::CanUndo() const
{
  return p_impl->m_pos != p_impl->m_commands.begin() && !IsMacroMode();
}

bool CommandStack::CanRedo() const
{
  return p_impl->m_pos != p_impl->m_commands.end() && !IsMacroMode();
}

int CommandStack::GetIndex() const
{
  return static_cast<int>(std::distance(p_impl->m_commands.begin(), p_impl->m_pos));
}

int CommandStack::GetCommandCount() const
{
  return static_cast<int>(p_impl->m_commands.size());
}

void CommandStack::Undo()
{
  if (CanUndo())
  {
    p_impl->m_pos--;
    (*p_impl->m_pos)->Undo();
  }
}

void CommandStack::Redo()
{
  if (CanRedo())
  {
    (*p_impl->m_pos)->Execute();
    p_impl->m_pos++;
  }
}

void CommandStack::Clear()
{
  p_impl->m_commands.clear();
  p_impl->m_pos = p_impl->m_commands.end();
}

void CommandStack::SetUndoLimit(int limit) {}

void CommandStack::BeginMacro(const std::string &name)
{
  const bool nested_macro_mode = IsMacroMode();

  auto macro = std::make_unique<MacroCommand>(name);
  auto macro_ptr = macro.get();

  p_impl->SaveCommand(std::move(macro), nested_macro_mode);

  p_impl->m_macro_stack.push(macro_ptr);
}

void CommandStack::EndMacro()
{
  if (!IsMacroMode())
  {
    throw RuntimeException("No command macro is currently recording");
  }
  // removing macro pointer fro macro stack, the macro command itself remains in the command stack
  p_impl->m_macro_stack.pop();
}

bool CommandStack::IsMacroMode() const
{
  return !p_impl->m_macro_stack.empty();
}

std::vector<const ICommand *> CommandStack::GetCommands() const
{
  std::vector<const ICommand *> result;

  std::transform(p_impl->m_commands.begin(), p_impl->m_commands.end(), std::back_inserter(result),
                 [](auto &iter) { return iter.get(); });

  return result;
}

}  // namespace mvvm
