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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "application_model_composer.h"

#include "model_composer.h"
#include "session_item.h"

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/commands/insert_item_command.h>
#include <mvvm/commands/remove_item_command.h>
#include <mvvm/commands/set_value_command.h>
#include <mvvm/signals/model_event_handler.h>

namespace mvvm
{

namespace
{
template <typename C, typename... Args>
C *ProcessCommand(ICommandStack *command_stack, Args &&...args)
{
  auto command = std::make_unique<C>(std::forward<Args>(args)...);
  return static_cast<C *>(command_stack->Execute(std::move(command)));
}

}  // namespace

ApplicationModelComposer::ApplicationModelComposer(ISessionModel *model,
                                                   ModelEventHandler *event_handler,
                                                   ICommandStack *command_stack)
    : m_model_composer(std::make_unique<ModelComposer>(*model))
    , m_event_handler(event_handler)
    , m_command_stack(command_stack)
{
}

ApplicationModelComposer::~ApplicationModelComposer() = default;

SessionItem *ApplicationModelComposer::InsertItem(std::unique_ptr<SessionItem> item,
                                                  SessionItem *parent, const TagIndex &tag_index)
{
  (void)item;
  (void)parent;
  (void)tag_index;
  return {};
}

std::unique_ptr<SessionItem> mvvm::ApplicationModelComposer::TakeItem(SessionItem *parent,
                                                                      const TagIndex &tag_index)
{
  (void)parent;
  (void)tag_index;
  return {};
}

bool ApplicationModelComposer::SetData(SessionItem *item, const variant_t &value, int role)
{
  auto command =
      ProcessCommand<SetValueCommand>(m_command_stack, m_model_composer.get(), item, value, role);

  auto result = command ? command->GetResult() : false;
  if (result)
  {
    m_event_handler->Notify<DataChangedEvent>(item, role);
  }

  return result;
}

void ApplicationModelComposer::ReplaceRootItem(std::unique_ptr<SessionItem> &old_root_item,
                                               std::unique_ptr<SessionItem> new_root_item)
{
  (void)old_root_item;
  (void)new_root_item;
}

ISessionModel *ApplicationModelComposer::GetModel() const
{
  return {};
}

}  // namespace mvvm
