/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "remove_item_command.h"

#include "command_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/factories/item_backup_strategy_factory.h>
#include <mvvm/model/i_item_backup_strategy.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

RemoveItemCommand::RemoveItemCommand(SessionItem* parent, const TagIndex& tag_index)
    : ModelCommand((parent != nullptr) ? parent->GetModel() : nullptr)
    , m_cached_parent(parent)
    , m_tag_index(tag_index)
    , m_backup_strategy(CreateItemCloneBackupStrategy())
{
  if (m_cached_parent == nullptr)
  {
    throw RuntimeException("Uninitialized item");
  }

  m_parent_identifier = m_cached_parent->GetIdentifier();
}

RemoveItemCommand::~RemoveItemCommand() = default;

RemoveItemCommand::result_t RemoveItemCommand::GetResult()
{
  return std::move(m_taken);
}

std::optional<event_variant_t> RemoveItemCommand::GetEventBefore() const
{
  // we find parent once and then use it for notification and command execution
  m_cached_parent = FindParent();
  if (m_cached_parent == nullptr)
  {
    throw RuntimeException("Can't find parent");
  }

  return GetBeforeEventForRemoveCommand(GetCommandStatus(), m_cached_parent, m_tag_index);
}

std::optional<event_variant_t> RemoveItemCommand::GetEventAfter() const
{
  return GetAfterEventForRemoveCommand(GetCommandStatus(), m_cached_parent, m_tag_index);
}

void RemoveItemCommand::ExecuteImpl()
{
  SetIsObsolete(false);

  m_taken = utils::TakeItemImpl(*m_cached_parent, m_tag_index);
  m_backup_strategy->SaveItem(*m_taken);
}

void RemoveItemCommand::UndoImpl()
{
  (void)utils::InsertItemImpl(m_backup_strategy->RestoreItem(), *m_cached_parent, m_tag_index);
}

SessionItem* RemoveItemCommand::FindParent() const
{
  auto current_parent = FindItem(m_parent_identifier);
  return (current_parent != nullptr) ? current_parent : m_cached_parent;
}

}  // namespace mvvm
