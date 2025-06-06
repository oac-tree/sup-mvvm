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

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/factories/item_backup_strategy_factory.h>
#include <mvvm/model/i_item_backup_strategy.h>
#include <mvvm/model/i_model_composer.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/path.h>
#include <mvvm/model/session_item.h>

#include <sstream>

namespace
{

std::string GenerateDescription(mvvm::SessionItem* parent, mvvm::SessionItem* child,
                                const mvvm::TagIndex& tag_index)
{
  (void)parent;
  std::ostringstream ostr;
  const std::string parent_name = parent ? parent->GetDisplayName() : "nullptr";
  const std::string child_name = child ? child->GetDisplayName() : "nullptr";
  ostr << "InsertItem: " << parent_name << " " << child_name << " " << tag_index.GetTag() << " "
       << tag_index.GetIndex();
  return ostr.str();
}

}  // namespace

namespace mvvm
{

struct InsertItemCommand::InsertItemCommandImpl
{
  IModelComposer* m_composer{nullptr};
  Path m_parent_path;
  TagIndex m_tag_index;
  std::unique_ptr<IItemBackupStrategy> m_backup_strategy;
  std::unique_ptr<SessionItem> m_to_insert;
  SessionItem* m_result{nullptr};

  InsertItemCommandImpl(IModelComposer* composer, std::unique_ptr<SessionItem> item,
                        SessionItem* parent, const TagIndex& tag_index)
      : m_composer(composer)
      , m_tag_index(tag_index)
      , m_backup_strategy(CreateItemTreeDataBackupStrategy(&GetGlobalItemFactory()))
      , m_to_insert(std::move(item))
  {
    m_backup_strategy->SaveItem(*m_to_insert);
    m_parent_path = utils::PathFromItem(parent);
  }

  //! Find parent item.
  SessionItem* FindParent() const
  {
    return utils::ItemFromPath(*m_composer->GetModel(), m_parent_path);
  }
};

InsertItemCommand::InsertItemCommand(IModelComposer* composer, std::unique_ptr<SessionItem> item,
                                     SessionItem* parent, const TagIndex& tag_index)
    : p_impl(std::make_unique<InsertItemCommandImpl>(composer, std::move(item), parent, tag_index))
{
  SetDescription(GenerateDescription(parent, p_impl->m_to_insert.get(), tag_index));
}

SessionItem* InsertItemCommand::GetResult() const
{
  return p_impl->m_result;
}

InsertItemCommand::~InsertItemCommand() = default;

void InsertItemCommand::ExecuteImpl()
{
  SetIsObsolete(false);

  auto parent = p_impl->FindParent();

  if (!parent)
  {
    throw RuntimeException("Can't find parent");
  }

  auto to_insert = p_impl->m_to_insert ? std::move(p_impl->m_to_insert)
                                       : p_impl->m_backup_strategy->RestoreItem();

  p_impl->m_result =
      p_impl->m_composer->InsertItem(std::move(to_insert), parent, p_impl->m_tag_index);
}

void InsertItemCommand::UndoImpl()
{
  auto parent = p_impl->FindParent();
  p_impl->m_composer->TakeItem(parent, p_impl->m_tag_index);

  // may be it is better to store taken item on board, and not to use backup at all?
}

}  // namespace mvvm
