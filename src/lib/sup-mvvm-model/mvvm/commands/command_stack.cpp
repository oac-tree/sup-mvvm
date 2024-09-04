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

#include <mvvm/core/exceptions.h>

#include <list>

namespace mvvm
{

struct CommandStack::CommandStackImpl
{
  std::list<std::unique_ptr<ICommand>> m_commands;

  // Points to the position in the list corresponding to the command which will be redone on the
  // next call to Redo()
  std::list<std::unique_ptr<ICommand>>::iterator m_pos;  //!< position in the command list

  CommandStackImpl() { m_pos = m_commands.end(); }

  /**
   * @brief Saves single command in the command stack.
   */
  ICommand *SaveSingleCommand(std::unique_ptr<ICommand> command)
  {
    // removing commands from the 'next redo` position till the end
    m_commands.erase(m_pos, m_commands.end());

    auto command_ptr = command.get();
    m_commands.emplace_back(std::move(command));
    m_pos = m_commands.end();

    return command_ptr;
  }
};

CommandStack::~CommandStack() = default;

CommandStack::CommandStack() : p_impl(std::make_unique<CommandStackImpl>()) {}

ICommand *CommandStack::Execute(std::unique_ptr<ICommand> command)
{
  if (command->IsObsolete())
  {
    throw RuntimeException("Attempt to insert obsolete command");
  }

  command->Execute();

  if (!command->IsObsolete())
  {
    return p_impl->SaveSingleCommand(std::move(command));
  }

  return nullptr;
}

bool CommandStack::CanUndo() const
{
  return p_impl->m_pos != p_impl->m_commands.begin();
}

bool CommandStack::CanRedo() const
{
  return p_impl->m_pos != p_impl->m_commands.end();
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

void CommandStack::BeginMacro(const std::string &name) {}

void CommandStack::EndMacro() {}

}  // namespace mvvm
