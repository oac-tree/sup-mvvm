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

#include "abstract_command.h"

#include <mvvm/core/exceptions.h>

namespace mvvm
{

struct AbstractCommand::AbstractCommandImpl
{
  bool m_is_obsolete{false};
  std::string m_description;
  CommandStatus m_status{CommandStatus::Initial};
  AbstractCommand* m_command{nullptr};

  explicit AbstractCommandImpl(AbstractCommand* self) : m_command(self) {};

  void SetAfterExecute() { m_status = CommandStatus::AfterExecute; }
  void SetAfterUndo() { m_status = CommandStatus::AfterUndo; }
  bool CanExecute() const { return m_command->GetCommandStatus() != CommandStatus::AfterExecute; }
  bool CanUndo() const
  {
    return m_command->GetCommandStatus() == CommandStatus::AfterExecute && !m_command->IsObsolete();
  }
};

AbstractCommand::AbstractCommand() : p_impl(std::make_unique<AbstractCommandImpl>(this)) {}

AbstractCommand::~AbstractCommand() = default;

void AbstractCommand::Execute()
{
  if (!p_impl->CanExecute())
  {
    throw RuntimeException("Can't execute the command. Wrong order.");
  }

  ExecuteImpl();

  p_impl->SetAfterExecute();
}

CommandStatus AbstractCommand::GetCommandStatus() const
{
  return p_impl->m_status;
}

void AbstractCommand::Undo()
{
  if (!p_impl->CanUndo())
  {
    throw RuntimeException("Can't undo the command. Wrong order.");
  }

  UndoImpl();

  p_impl->SetAfterUndo();
}

bool AbstractCommand::IsObsolete() const
{
  return p_impl->m_is_obsolete;
}

void AbstractCommand::SetIsObsolete(bool value)
{
  p_impl->m_is_obsolete = value;
}

std::string AbstractCommand::GetDescription() const
{
  return p_impl->m_description;
}

void AbstractCommand::SetDescription(const std::string& text) const
{
  p_impl->m_description = text;
}

}  // namespace mvvm
