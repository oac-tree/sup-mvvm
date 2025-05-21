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

#include "mock_command_stack.h"

#include <mvvm/commands/i_command.h>

namespace mvvm::test
{

ICommand *CommandStackDecorator::Execute(std::unique_ptr<ICommand> command)
{
  return m_decoratee->Execute(std::move(command));
}

bool CommandStackDecorator::CanUndo() const
{
  return m_decoratee->CanUndo();
}

bool CommandStackDecorator::CanRedo() const
{
  return m_decoratee->CanRedo();
}

std::size_t CommandStackDecorator::GetIndex() const
{
  return m_decoratee->GetIndex();
}

std::size_t CommandStackDecorator::GetCommandCount() const
{
  return m_decoratee->GetCommandCount();
}

void CommandStackDecorator::Undo()
{
  m_decoratee->Undo();
}

void CommandStackDecorator::Redo()
{
  m_decoratee->Redo();
}

void CommandStackDecorator::Clear()
{
  m_decoratee->Clear();
}

void CommandStackDecorator::SetUndoLimit(std::size_t limit)
{
  m_decoratee->SetUndoLimit(limit);
}

void CommandStackDecorator::BeginMacro(const std::string &name)
{
  m_decoratee->BeginMacro(name);
}

void CommandStackDecorator::EndMacro()
{
  m_decoratee->EndMacro();
}

const ICommand *CommandStackDecorator::GetNextUndoCommand() const
{
  return m_decoratee->GetNextUndoCommand();
}

const ICommand *CommandStackDecorator::GetNextRedoCommand() const
{
  return m_decoratee->GetNextRedoCommand();
}

}  // namespace mvvm::test
