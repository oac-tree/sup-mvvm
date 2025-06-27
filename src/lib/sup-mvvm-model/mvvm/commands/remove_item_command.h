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

#ifndef MVVM_COMMANDS_REMOVE_ITEM_COMMAND_H_
#define MVVM_COMMANDS_REMOVE_ITEM_COMMAND_H_

#include <mvvm/commands/abstract_command.h>
#include <mvvm/core/variant.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class TagIndex;
class IItemBackupStrategy;

/**
 * @brief The RemoveItemCommand class allows to remopve a child from part and undo the change.
 */
class MVVM_MODEL_EXPORT RemoveItemCommand : public AbstractCommand
{
public:
  using result_t = std::unique_ptr<SessionItem>;

  RemoveItemCommand(SessionItem* parent, const TagIndex& tag_index);
  ~RemoveItemCommand() override;

  RemoveItemCommand(const RemoveItemCommand&) = delete;
  RemoveItemCommand& operator=(const RemoveItemCommand&) = delete;
  RemoveItemCommand(RemoveItemCommand&&) = delete;
  RemoveItemCommand& operator=(RemoveItemCommand&&) = delete;

  result_t GetResult();

  std::optional<event_variant_t> GetEventBefore() const override;

  std::optional<event_variant_t> GetEventAfter() const override;

private:
  void ExecuteImpl() override;
  void UndoImpl() override;
  SessionItem* FindParent() const;

  mutable SessionItem* m_cached_parent{nullptr};
  ISessionModel* m_model{nullptr};
  std::string m_parent_identifier;
  TagIndex m_tag_index;
  std::unique_ptr<IItemBackupStrategy> m_backup_strategy;
  std::unique_ptr<SessionItem> m_taken;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_REMOVE_ITEM_COMMAND_H_
