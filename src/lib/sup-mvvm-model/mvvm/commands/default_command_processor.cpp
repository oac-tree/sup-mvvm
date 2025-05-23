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

#include "default_command_processor.h"

#include "i_command.h"

namespace mvvm
{

ICommand *DefaultCommandProcessor::Execute(std::unique_ptr<ICommand> command)
{
  auto result = command.get();
  command->Execute();
  return result;
}

bool DefaultCommandProcessor::CanUndo() const
{
  return false;
}

bool DefaultCommandProcessor::CanRedo() const
{
  return false;
}

std::size_t DefaultCommandProcessor::GetIndex() const
{
  return 0U;
}

std::size_t DefaultCommandProcessor::GetCommandCount() const
{
  return 0U;
}

void DefaultCommandProcessor::Undo()
{
  // do nothing
}

void DefaultCommandProcessor::Redo()
{
  // do nothing
}

void DefaultCommandProcessor::Clear()
{
  // do nothing
}

void DefaultCommandProcessor::SetUndoLimit(std::size_t limit)
{
  (void)limit;
  // do nothing
}

void DefaultCommandProcessor::BeginMacro(const std::string &name)
{
  (void)name;
  // do nothing
}

void DefaultCommandProcessor::EndMacro()
{
  // do nothing
}

}  // namespace mvvm
