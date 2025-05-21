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

#include "insert_item_command_v2.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/factories/item_backup_strategy_factory.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/session_item.h>

// #include <sstream>

// namespace
// {

// std::string GenerateDescription(mvvm::SessionItem* parent, mvvm::SessionItem* child,
//                                 const mvvm::TagIndex& tag_index)
// {
//   (void)parent;
//   std::ostringstream ostr;
//   const std::string parent_name = parent ? parent->GetDisplayName() : "nullptr";
//   const std::string child_name = child ? child->GetDisplayName() : "nullptr";
//   ostr << "InsertItem: " << parent_name << " " << child_name << " " << tag_index.GetTag() << " "
//        << tag_index.GetIndex();
//   return ostr.str();
// }

// }  // namespace

namespace mvvm
{

struct InsertItemCommandV2::InsertItemCommandImpl
{
  SessionItem* m_cached_parent{nullptr};
  ISessionModel* m_model{nullptr};
  std::string m_parent_identifier;
  TagIndex m_tag_index;
  std::unique_ptr<IItemBackupStrategy> m_backup_strategy;
  std::unique_ptr<SessionItem> m_to_insert;
  SessionItem* m_result{nullptr};

  InsertItemCommandImpl(std::unique_ptr<SessionItem> item, SessionItem* parent,
                        const TagIndex& tag_index)
      : m_cached_parent(parent)
      , m_tag_index(tag_index)
      , m_backup_strategy(CreateItemTreeDataBackupStrategy(&GetGlobalItemFactory()))
      , m_to_insert(std::move(item))
  {
    if (!m_cached_parent)
    {
      throw RuntimeException("Uninitialized item");
    }

    m_parent_identifier = m_cached_parent->GetIdentifier();
    m_backup_strategy->SaveItem(*m_to_insert);
  }

  SessionItem* FindParent() const
  {
    return m_model ? m_model->FindItem(m_parent_identifier) : m_cached_parent;
  }
};

InsertItemCommandV2::InsertItemCommandV2(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                         const TagIndex& tag_index)
    : p_impl(std::make_unique<InsertItemCommandImpl>(std::move(item), parent, tag_index))
{
}

InsertItemCommandV2::~InsertItemCommandV2() = default;

SessionItem* InsertItemCommandV2::GetResult() const
{
  return p_impl->m_result;
}

std::optional<event_variant_t> InsertItemCommandV2::GetEventBefore() const
{
  p_impl->m_cached_parent = p_impl->FindParent();
  if (!p_impl->m_cached_parent)
  {
    throw RuntimeException("Can't find parent");
  }
  return AboutToInsertItemEvent{p_impl->m_cached_parent, p_impl->m_tag_index};
}

std::optional<event_variant_t> InsertItemCommandV2::GetEventAfter() const
{
  return ItemInsertedEvent{p_impl->m_cached_parent, p_impl->m_tag_index};
}

void InsertItemCommandV2::ExecuteImpl()
{
  SetIsObsolete(false);

  auto to_insert = p_impl->m_to_insert ? std::move(p_impl->m_to_insert)
                                       : p_impl->m_backup_strategy->RestoreItem();
  p_impl->m_result = p_impl->m_cached_parent->InsertItem(std::move(to_insert), p_impl->m_tag_index);
}

void InsertItemCommandV2::UndoImpl()
{
  p_impl->m_cached_parent->TakeItem(p_impl->m_tag_index);

  // may be it is better to store taken item on board, and not to use backup at all?
}

}  // namespace mvvm
