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

namespace mvvm
{
void CommandStack::Push(std::unique_ptr<CommandInterface> command) {}

bool CommandStack::CanUndo() const
{
  return false;
}

bool CommandStack::CanRedo() const
{
  return false;
}

int CommandStack::GetIndex() const
{
  return 0;
}

int CommandStack::GetSize() const
{
  return 0;
}

void CommandStack::Undo() {}

void CommandStack::Redo() {}

void CommandStack::Clear() {}

void CommandStack::SetUndoLimit(int limit) {}

void CommandStack::BeginMacro(const std::string &name) {}

void CommandStack::EndMacro() {}

}  // namespace mvvm
