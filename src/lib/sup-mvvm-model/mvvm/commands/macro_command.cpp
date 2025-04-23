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

#include "macro_command.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <algorithm>

namespace mvvm
{

MacroCommand::MacroCommand(const std::string& macro_name)
{
  SetDescription(macro_name);
}

CommandStatus MacroCommand::GetCommandStatus() const
{
  // we assume that all children are in the same state
  return m_children.empty() ? AbstractCommand::GetCommandStatus()
                            : m_children[0]->GetCommandStatus();
}

void MacroCommand::Append(std::unique_ptr<ICommand> command)
{
  m_children.push_back(std::move(command));
}

std::size_t MacroCommand::GetCommandCount() const
{
  return m_children.size();
}

std::vector<const ICommand*> MacroCommand::GetCommands() const
{
  std::vector<const ICommand*> result;

  std::transform(m_children.begin(), m_children.end(), std::back_inserter(result),
                 [](auto& iter) { return iter.get(); });

  return result;
}

void MacroCommand::ExecuteImpl()
{
  std::for_each(m_children.begin(), m_children.end(), [](auto& iter) { (*iter).Execute(); });
}

void MacroCommand::UndoImpl()
{
  // in reverse order
  std::for_each(m_children.rbegin(), m_children.rend(), [](auto& iter) { (*iter).Undo(); });
}

}  // namespace mvvm
