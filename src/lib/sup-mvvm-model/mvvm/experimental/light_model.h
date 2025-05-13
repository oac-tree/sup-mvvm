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

#ifndef MVVM_EXPERIMENTAL_LIGHT_MODEL_H_
#define MVVM_EXPERIMENTAL_LIGHT_MODEL_H_

#include <mvvm/core/variant.h>
#include <mvvm/signals/event_types.h>

#include <functional>
#include <memory>
#include <optional>

namespace mvvm
{
class ICommandStack;
}

namespace mvvm::experimental
{

class ILightItem;
class LightCommand;

class LightModel
{
public:
  using notify_func_t = std::function<void(const event_variant_t& event)>;

  explicit LightModel(notify_func_t notify_func);
  ~LightModel();

  bool SetData(ILightItem* item, const variant_t& value, std::int32_t role);

  bool ExecuteCommand(std::unique_ptr<LightCommand> command);

  void Undo();

  void Redo();

private:
  void Notify(const std::optional<event_variant_t>& optional_event);

  std::unique_ptr<ILightItem> m_root;
  std::unique_ptr<ICommandStack> m_command_stack;
  notify_func_t m_notify_func;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_MODEL_H_
