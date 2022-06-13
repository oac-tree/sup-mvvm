/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/command_stack.h"

#include "mvvm/commands/command_interface.h"
#include "mvvm/core/exceptions.h"

#include <iostream>
#include <list>

namespace mvvm
{

struct CommandStack::CommandStackImpl
{
  std::list<std::unique_ptr<CommandInterface>> m_commands;
  std::list<std::unique_ptr<CommandInterface>>::iterator m_pos;

  CommandStackImpl() { m_pos = m_commands.end(); }
};

CommandStack::~CommandStack() = default;

CommandStack::CommandStack() : p_impl(std::make_unique<CommandStackImpl>()) {}

void CommandStack::Execute(std::unique_ptr<CommandInterface> command)
{
  if (command->IsObsolete())
  {
    throw RuntimeException("Attempt to inser obsolete command");
  }

  command->Execute();

  if (!command->IsObsolete())
  {
    p_impl->m_commands.emplace_back(std::move(command));
    p_impl->m_pos = p_impl->m_commands.end();
  }
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

int CommandStack::GetSize() const
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

void CommandStack::Clear() {}

void CommandStack::SetUndoLimit(int limit) {}

void CommandStack::BeginMacro(const std::string &name) {}

void CommandStack::EndMacro() {}

}  // namespace mvvm
