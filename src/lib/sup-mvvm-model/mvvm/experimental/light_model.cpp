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

#include "light_model.h"

#include "light_command.h"
#include "light_item.h"

#include <mvvm/commands/command_stack.h>

namespace mvvm::experimental
{

LightModel::LightModel(notify_func_t notify_func)
    : m_root(std::make_unique<LightItem>())
    , m_command_stack(std::make_unique<CommandStack>())
    , m_notify_func(std::move(notify_func))
{
  m_root->SetModel(this);
}

ILightItem *LightModel::InsertItem(std::unique_ptr<ILightItem> item, ILightItem *parent,
                                   std::size_t index)
{
  return parent->InsertItem(std::move(item), index);
}

LightModel::~LightModel() = default;

bool LightModel::SetData(ILightItem *item, const variant_t &value, int32_t role)
{
  return item->SetData(value, role);
}

bool LightModel::ExecuteCommand(std::unique_ptr<LightCommand> command)
{
  auto command_ptr = command.get();

  Notify(command_ptr->GetNextEvent());
  m_command_stack->Execute(std::move(command));
  Notify(command_ptr->GetNextEvent());

  return true;
}

void LightModel::Undo()
{
  m_command_stack->Undo();
}

void LightModel::Redo()
{
  m_command_stack->Redo();
}

ILightItem *LightModel::GetRootItem()
{
  return m_root.get();
}

void LightModel::Notify(const std::optional<event_variant_t> &optional_event)
{
  if (m_notify_func && optional_event.has_value())
  {
    m_notify_func(optional_event.value());
  }
}

}  // namespace mvvm::experimental
