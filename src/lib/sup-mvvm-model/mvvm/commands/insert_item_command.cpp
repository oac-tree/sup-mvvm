/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/insert_item_command.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/factories/item_backup_strategy_factory.h>
#include <mvvm/interfaces/item_backup_strategy_interface.h>
#include <mvvm/interfaces/model_composer_interface.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/path.h>
#include <mvvm/model/sessionitem.h>

#include <sstream>

namespace
{

std::string GenerateDescription(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index)
{
  (void)parent;
  std::ostringstream ostr;
  ostr << "InsertItem: " << tag_index.tag << " " << tag_index.index;
  return ostr.str();
}

}  // namespace

namespace mvvm
{

struct InsertItemCommand::InsertItemCommandImpl
{
  ModelComposerInterface* m_composer{nullptr};
  Path m_parent_path;
  TagIndex m_tag_index;
  std::unique_ptr<ItemBackupStrategyInterface> m_backup_strategy;
  std::unique_ptr<SessionItem> m_to_insert;
  SessionItem* m_result{nullptr};

  InsertItemCommandImpl(ModelComposerInterface* composer, std::unique_ptr<SessionItem> item,
                        SessionItem* parent, const TagIndex& tag_index)
      : m_composer(composer), m_tag_index(tag_index), m_to_insert(std::move(item))
  {
    m_backup_strategy =
        std::move(CreateItemTreeDataBackupStrategy(composer->GetModel()->GetFactory()));
    m_backup_strategy->SaveItem(*m_to_insert);
    m_parent_path = utils::PathFromItem(parent);
  }

  //! Find parent item.
  SessionItem* FindParent() const
  {
    return utils::ItemFromPath(*m_composer->GetModel(), m_parent_path);
  }
};

InsertItemCommand::InsertItemCommand(ModelComposerInterface* composer,
                                     std::unique_ptr<SessionItem> item, SessionItem* parent,
                                     const TagIndex& tag_index)
    : p_impl(std::make_unique<InsertItemCommandImpl>(composer, std::move(item), parent, tag_index))
{
  SetDescription(GenerateDescription(parent, tag_index));
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
