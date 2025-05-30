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

#include "remove_item_command.h"

#include "command_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/factories/item_backup_strategy_factory.h>
#include <mvvm/model/i_item_backup_strategy.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/tagged_items.h>

namespace mvvm
{

struct RemoveItemCommand::RemoveItemCommandImpl
{
  SessionItem* m_cached_parent{nullptr};
  ISessionModel* m_model{nullptr};
  std::string m_parent_identifier;
  TagIndex m_tag_index;
  std::unique_ptr<IItemBackupStrategy> m_backup_strategy;
  std::unique_ptr<SessionItem> m_taken;

  RemoveItemCommandImpl(SessionItem* parent, const TagIndex& tag_index)
      : m_cached_parent(parent)
      , m_tag_index(tag_index)
      , m_backup_strategy(CreateItemTreeDataBackupStrategy(&GetGlobalItemFactory()))
  {
    if (!m_cached_parent)
    {
      throw RuntimeException("Uninitialized item");
    }

    m_model = m_cached_parent->GetModel();
    m_parent_identifier = m_cached_parent->GetIdentifier();
  }

  SessionItem* FindParent() const
  {
    return m_model ? m_model->FindItem(m_parent_identifier) : m_cached_parent;
  }
};

RemoveItemCommand::RemoveItemCommand(SessionItem* parent, const TagIndex& tag_index)
    : p_impl(std::make_unique<RemoveItemCommandImpl>(parent, tag_index))
{
}

RemoveItemCommand::~RemoveItemCommand() = default;

std::unique_ptr<SessionItem> RemoveItemCommand::GetResult() const
{
  return std::move(p_impl->m_taken);
}

std::optional<event_variant_t> RemoveItemCommand::GetEventBefore() const
{
  // we find parent once and then use it for notification and command execution
  p_impl->m_cached_parent = p_impl->FindParent();
  if (!p_impl->m_cached_parent)
  {
    throw RuntimeException("Can't find parent");
  }

  return GetBeforeEventForRemoveCommand(GetCommandStatus(), p_impl->m_cached_parent,
                                        p_impl->m_tag_index);
}

std::optional<event_variant_t> RemoveItemCommand::GetEventAfter() const
{
  return GetAfterEventForRemoveCommand(GetCommandStatus(), p_impl->m_cached_parent,
                                       p_impl->m_tag_index);
}

void RemoveItemCommand::ExecuteImpl()
{
  SetIsObsolete(false);

  auto taken = p_impl->m_cached_parent->GetTaggedItems()->TakeItem(p_impl->m_tag_index);
  if (!taken)
  {
    throw RuntimeException("Can't take an item");
  }

  taken->SetParent(nullptr);
  taken->SetModel(nullptr);

  p_impl->m_backup_strategy->SaveItem(*taken);

  p_impl->m_taken = std::move(taken);
}

void RemoveItemCommand::UndoImpl()
{
  auto result = p_impl->m_cached_parent->GetTaggedItems()->InsertItem(p_impl->m_backup_strategy->RestoreItem(),
                                                        p_impl->m_tag_index);
  result->SetParent(p_impl->m_cached_parent);
  result->SetModel(p_impl->m_cached_parent->GetModel());
}

}  // namespace mvvm
