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

#include "notifying_command_stack.h"

#include "i_command.h"

namespace mvvm
{

NotifyingCommandStack::NotifyingCommandStack(ICommandStack *decoratee) : m_decoratee(decoratee) {}

NotifyingCommandStack::~NotifyingCommandStack() = default;

ICommand *NotifyingCommandStack::Execute(std::unique_ptr<ICommand> command)
{
  if (m_current_command)
  {
    m_buffer_to_process.push(std::move(command));
  }

  auto command_ptr = command.get();
  ProcessCommand(std::move(command));

  return command_ptr;
}

bool NotifyingCommandStack::CanUndo() const
{
  return m_decoratee->CanUndo();
}

bool NotifyingCommandStack::CanRedo() const
{
  return m_decoratee->CanRedo();
}

int NotifyingCommandStack::GetIndex() const
{
  return m_decoratee->GetIndex();
}

int NotifyingCommandStack::GetCommandCount() const
{
  return m_decoratee->GetCommandCount();
}

std::vector<const ICommand *> NotifyingCommandStack::GetCommands() const
{
  return m_decoratee->GetCommands();
}

void NotifyingCommandStack::Undo()
{
  auto command = GetNextUndoCommand();

  NotifyBefore(command);

  m_decoratee->Undo();

  NotifyAfter(command);
}

void NotifyingCommandStack::Redo()
{
  m_decoratee->Redo();
}

void NotifyingCommandStack::Clear()
{
  m_decoratee->Clear();
}

void NotifyingCommandStack::SetUndoLimit(std::size_t limit)
{
  m_decoratee->SetUndoLimit(limit);
}

void NotifyingCommandStack::BeginMacro(const std::string &name)
{
  m_decoratee->BeginMacro(name);
}

void NotifyingCommandStack::EndMacro()
{
  m_decoratee->EndMacro();
}

void NotifyingCommandStack::ProcessCommand(std::unique_ptr<ICommand> command)
{
  auto command_ptr = command.get();

  m_current_command = command_ptr;
  NotifyBefore(command_ptr);
  m_decoratee->Execute(std::move(command));
  NotifyAfter(command_ptr);
  m_current_command = nullptr;
}

ICommand *NotifyingCommandStack::GetNextUndoCommand()
{
  return nullptr;
}

ICommand *NotifyingCommandStack::GetNextRedoCommand()
{
  return nullptr;
}

void NotifyingCommandStack::NotifyBefore(ICommand *command)
{
  (void) command;
}

void NotifyingCommandStack::NotifyAfter(ICommand *command)
{
  (void) command;
}

void NotifyingCommandStack::ProcessBuffer()
{
  while (!m_buffer_to_process.empty())
  {
    ProcessCommand(std::move(m_buffer_to_process.front()));
    m_buffer_to_process.pop();
  }
}

}  // namespace mvvm
