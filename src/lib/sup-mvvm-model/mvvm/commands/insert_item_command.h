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

#ifndef MVVM_COMMANDS_INSERT_ITEM_COMMAND_H_
#define MVVM_COMMANDS_INSERT_ITEM_COMMAND_H_

#include <mvvm/commands/model_command.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class IItemBackupStrategy;

/**
 * @brief The InsertItemCommand class allows to insert a child to the part and undo the change.
 */
class InsertItemCommand : public ModelCommand
{
public:
  using result_t = SessionItem*;
  InsertItemCommand(std::unique_ptr<SessionItem> item, SessionItem* parent,
                    const TagIndex& tag_index);
  ~InsertItemCommand() override;

  InsertItemCommand(const InsertItemCommand&) = delete;
  InsertItemCommand& operator=(const InsertItemCommand&) = delete;
  InsertItemCommand(InsertItemCommand&&) = delete;
  InsertItemCommand& operator=(InsertItemCommand&&) = delete;

  result_t GetResult() const;

  std::optional<event_variant_t> GetEventBefore() const override;

  std::optional<event_variant_t> GetEventAfter() const override;

private:
  void ExecuteImpl() override;
  void UndoImpl() override;
  SessionItem* FindParent() const;

  mutable SessionItem* m_cached_parent{nullptr};
  std::string m_parent_identifier;
  TagIndex m_tag_index;
  std::unique_ptr<IItemBackupStrategy> m_backup_strategy;
  std::unique_ptr<SessionItem> m_to_insert;
  SessionItem* m_result{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_INSERT_ITEM_COMMAND_H_
