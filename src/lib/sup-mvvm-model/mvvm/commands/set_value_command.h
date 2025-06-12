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

#ifndef MVVM_COMMANDS_SET_VALUE_COMMAND_V2_H_
#define MVVM_COMMANDS_SET_VALUE_COMMAND_V2_H_

#include <mvvm/commands/abstract_command.h>
#include <mvvm/core/variant.h>
#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{

class SessionItem;

/**
 * @brief The SetValueCommand class allows to set the value to the item and undo the change.
 */
class MVVM_MODEL_EXPORT SetValueCommand : public AbstractCommand
{
public:
  using result_t = bool;
  SetValueCommand(SessionItem* item, const variant_t& value, role_t role);
  ~SetValueCommand() override;

  SetValueCommand(const SetValueCommand&) = delete;
  SetValueCommand& operator=(const SetValueCommand&) = delete;
  SetValueCommand(SetValueCommand&&) = delete;
  SetValueCommand& operator=(SetValueCommand&&) = delete;

  result_t GetResult() const;

  std::optional<event_variant_t> GetEventBefore() const override;

  std::optional<event_variant_t> GetEventAfter() const override;

private:
  void ExecuteImpl() override;
  void UndoImpl() override;
  void SwapValues();

  void SetResult(bool value);

  struct SetValueCommandImpl;
  std::unique_ptr<SetValueCommandImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_SET_VALUE_COMMAND_V2_H_
