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
#include <optional>

namespace mvvm
{

class SessionItem;

class MVVM_MODEL_EXPORT SetValueCommandV2 : public AbstractCommand
{
public:
  SetValueCommandV2(SessionItem* item, const variant_t& value, int role);
  ~SetValueCommandV2() override;

  SetValueCommandV2(const SetValueCommandV2&) = delete;
  SetValueCommandV2& operator=(const SetValueCommandV2&) = delete;
  SetValueCommandV2(SetValueCommandV2&&) = delete;
  SetValueCommandV2& operator=(SetValueCommandV2&&) = delete;

  bool GetResult() const;

  std::optional<event_variant_t> GetEventBefore() const;

  std::optional<event_variant_t> GetEventAfter() const;

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
