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

#include "command_model_composer.h"

#include "insert_item_command.h"
#include "remove_item_command.h"
#include "set_value_command.h"

#include <mvvm/model/session_item.h>

namespace mvvm
{

SessionItem *CommandModelComposer::InsertItem(std::unique_ptr<SessionItem> item,
                                              SessionItem *parent, const TagIndex &tag_index)
{
  auto command =
      ProcessCommand<InsertItemCommand>(m_composer.get(), std::move(item), parent, tag_index);
  return command ? command->GetResult() : nullptr;
}

std::unique_ptr<SessionItem> mvvm::CommandModelComposer::TakeItem(SessionItem *parent,
                                                                  const TagIndex &tag_index)
{
  auto command = ProcessCommand<RemoveItemCommand>(m_composer.get(), parent, tag_index);
  return command ? command->GetResult() : std::unique_ptr<SessionItem>();
}

bool CommandModelComposer::SetData(SessionItem *item, const variant_t &value, int role)
{
  auto command = ProcessCommand<SetValueCommand>(m_composer.get(), item, value, role);

  return command ? command->GetResult() : false;
}

void CommandModelComposer::ReplaceRootItem(std::unique_ptr<SessionItem> &old_root_item,
                                           std::unique_ptr<SessionItem> new_root_item)
{
  m_composer->ReplaceRootItem(old_root_item, std::move(new_root_item));
  m_command_stack->Clear();
}

ISessionModel *CommandModelComposer::GetModel() const
{
  return m_composer->GetModel();
}

}  // namespace mvvm
