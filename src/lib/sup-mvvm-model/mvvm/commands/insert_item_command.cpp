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

#include "insert_item_command.h"

#include "command_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/factories/item_backup_strategy_factory.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

InsertItemCommand::InsertItemCommand(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                     const TagIndex& tag_index)
    : m_cached_parent(parent)
    , m_tag_index(tag_index)
    , m_backup_strategy(CreateItemCloneBackupStrategy())
    , m_to_insert(std::move(item))
{
  if (!m_cached_parent)
  {
    throw RuntimeException("Uninitialized item");
  }

  m_model = m_cached_parent->GetModel();
  m_parent_identifier = m_cached_parent->GetIdentifier();
  m_backup_strategy->SaveItem(*m_to_insert);
}

InsertItemCommand::~InsertItemCommand() = default;

InsertItemCommand::result_t InsertItemCommand::GetResult() const
{
  return m_result;
}

std::optional<event_variant_t> InsertItemCommand::GetEventBefore() const
{
  // we find parent once and then use it for notification and command execution
  m_cached_parent = FindParent();
  if (!m_cached_parent)
  {
    throw RuntimeException("Can't find parent");
  }

  return GetBeforeEventForInsertCommand(GetCommandStatus(), m_cached_parent, m_tag_index);
}

std::optional<event_variant_t> InsertItemCommand::GetEventAfter() const
{
  return GetAfterEventForInsertCommand(GetCommandStatus(), m_cached_parent, m_tag_index);
}

void InsertItemCommand::ExecuteImpl()
{
  SetIsObsolete(false);

  auto to_insert = m_to_insert ? std::move(m_to_insert) : m_backup_strategy->RestoreItem();

  m_result = utils::InsertItemImpl(std::move(to_insert), *m_cached_parent, m_tag_index);
}

void InsertItemCommand::UndoImpl()
{
  auto taken = utils::TakeItemImpl(*m_cached_parent, m_tag_index);
  (void)taken;  // FIXME save in the implementation
}

SessionItem* InsertItemCommand::FindParent() const
{
  return m_model ? m_model->FindItem(m_parent_identifier) : m_cached_parent;
}

}  // namespace mvvm
