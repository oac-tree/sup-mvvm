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

#include "remove_item_command.h"

#include <mvvm/core/exceptions.h>
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

std::string GenerateDescription(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index)
{
  (void)parent;
  std::ostringstream ostr;
  ostr << "RemoveItem: " << tag_index.tag << " " << tag_index.index;
  return ostr.str();
}

}  // namespace

namespace mvvm
{

struct RemoveItemCommand::RemoveItemCommandImpl
{
  IModelComposer* m_composer{nullptr};
  Path m_parent_path;
  TagIndex m_tag_index;
  std::unique_ptr<IItemBackupStrategy> m_backup_strategy;
  std::unique_ptr<SessionItem> m_taken;

  RemoveItemCommandImpl(IModelComposer* composer, SessionItem* parent, const TagIndex& tag_index)
      : m_composer(composer), m_tag_index(tag_index)
  {
    m_backup_strategy = std::move(CreateItemTreeDataBackupStrategy(&GetGlobalItemFactory()));
    m_parent_path = utils::PathFromItem(parent);
  }

  //! Find parent item.
  SessionItem* FindParent() const
  {
    return utils::ItemFromPath(*m_composer->GetModel(), m_parent_path);
  }
};

RemoveItemCommand::RemoveItemCommand(IModelComposer* composer, SessionItem* parent,
                                     const TagIndex& tag_index)
    : p_impl(std::make_unique<RemoveItemCommandImpl>(composer, parent, tag_index))
{
  SetDescription(GenerateDescription(parent, tag_index));
}

std::unique_ptr<SessionItem> RemoveItemCommand::GetResult() const
{
  return std::move(p_impl->m_taken);
}

RemoveItemCommand::~RemoveItemCommand() = default;

void RemoveItemCommand::ExecuteImpl()
{
  SetIsObsolete(false);

  auto parent = p_impl->FindParent();

  if (!parent)
  {
    throw RuntimeException("Can't find parent");
  }

  auto taken = p_impl->m_composer->TakeItem(parent, p_impl->m_tag_index);
  if (!taken)
  {
    throw RuntimeException("Can't take an item");
  }

  p_impl->m_backup_strategy->SaveItem(*taken);

  p_impl->m_taken = std::move(taken);
}

void RemoveItemCommand::UndoImpl()
{
  auto parent = p_impl->FindParent();
  p_impl->m_composer->InsertItem(p_impl->m_backup_strategy->RestoreItem(), parent,
                                 p_impl->m_tag_index);
}

}  // namespace mvvm
