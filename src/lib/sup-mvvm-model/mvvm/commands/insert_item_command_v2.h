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

#ifndef MVVM_COMMANDS_INSERT_ITEM_COMMAND_V2_H_
#define MVVM_COMMANDS_INSERT_ITEM_COMMAND_V2_H_

#include <mvvm/commands/abstract_command.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class TagIndex;

/**
 * @brief The InsertItemCommandV2 class allows to insert a child to the part and undo the change.
 */
class MVVM_MODEL_EXPORT InsertItemCommandV2 : public AbstractCommand
{
public:
  InsertItemCommandV2(std::unique_ptr<SessionItem> item,
                    SessionItem* parent, const TagIndex& tag_index);
  ~InsertItemCommandV2() override;

  InsertItemCommandV2(const InsertItemCommandV2&) = delete;
  InsertItemCommandV2& operator=(const InsertItemCommandV2&) = delete;
  InsertItemCommandV2(InsertItemCommandV2&&) = delete;
  InsertItemCommandV2& operator=(InsertItemCommandV2&&) = delete;

  SessionItem* GetResult() const;

  std::optional<event_variant_t> GetEventBefore() const override;

  std::optional<event_variant_t> GetEventAfter() const override;

private:
  void ExecuteImpl() override;
  void UndoImpl() override;

  struct InsertItemCommandImpl;
  std::unique_ptr<InsertItemCommandImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_INSERT_ITEM_COMMAND_V2_H_
