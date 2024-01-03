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
  enum class CommandStatus
  {
    Initial,
    AfterExecute,
    AfterUndo
  };

  bool m_is_obsolete{false};
  std::string m_description;
  CommandStatus m_status{CommandStatus::Initial};
  AbstractCommand* m_self{nullptr};

  explicit AbstractCommandImpl(AbstractCommand* self) : m_self(self){};

  void SetAfterExecute() { m_status = CommandStatus::AfterExecute; }
  void SetAfterUndo() { m_status = CommandStatus::AfterUndo; }
  bool CanExecute() const { return m_status != CommandStatus::AfterExecute; }
  bool CanUndo() const { return m_status == CommandStatus::AfterExecute && !m_self->IsObsolete(); }
};

AbstractCommand::AbstractCommand() : p_impl(std::make_unique<AbstractCommandImpl>(this)) {}

void AbstractCommand::Execute()
{
  if (!p_impl->CanExecute())
  {
    throw mvvm::RuntimeException("Can't execute the command. Wrong order.");
  }

  ExecuteImpl();

  p_impl->SetAfterExecute();
}

void AbstractCommand::Undo()
{
  if (!p_impl->CanUndo())
  {
    throw mvvm::RuntimeException("Can't undo the command. Wrong order.");
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

AbstractCommand::~AbstractCommand() = default;

}  // namespace mvvm
