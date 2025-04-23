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

#include "application_model.h"

#include <mvvm/commands/command_model_composer.h>
#include <mvvm/commands/command_stack.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_event_handler.h>

namespace
{

/**
 * @brief Creates model composer which knows how to send signals.
 */
std::unique_ptr<mvvm::IModelComposer> CreateNotifyingComposer(
    mvvm::ModelEventHandler* event_handler, mvvm::ISessionModel* model)
{
  return std::make_unique<mvvm::NotifyingModelComposer<mvvm::ModelComposer>>(event_handler, *model);
}

/**
 * @brief Creates model composer which knows how to undo changes.
 */
std::unique_ptr<mvvm::IModelComposer> CreateCommandComposer(
    mvvm::ICommandStack* command_stack, std::unique_ptr<mvvm::IModelComposer> composer)
{
  return std::make_unique<mvvm::CommandModelComposer>(command_stack, std::move(composer));
}

}  // namespace

namespace mvvm
{

struct ApplicationModel::ApplicationModelImpl
{
  ModelEventHandler m_event_handler;
  std::unique_ptr<ICommandStack> m_command_stack;
};

ApplicationModel::ApplicationModel(std::string model_type)
    : ApplicationModel(std::move(model_type), {})
{
}

ApplicationModel::ApplicationModel(std::string model_type, std::shared_ptr<ItemPool> pool)
    : SessionModel(std::move(model_type), std::move(pool))
    , p_impl(std::make_unique<ApplicationModelImpl>())
{
  SetComposer(CreateNotifyingComposer(&p_impl->m_event_handler, this));
}

ApplicationModel::~ApplicationModel()
{
  p_impl->m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(this);
}

ModelEventHandler* ApplicationModel::GetEventHandler() const
{
  return &p_impl->m_event_handler;
}

ICommandStack* ApplicationModel::GetCommandStack() const
{
  return p_impl->m_command_stack.get();
}

void ApplicationModel::CheckIn(SessionItem* item)
{
  SessionModel::CheckIn(item);
  item->Activate();
}

void ApplicationModel::SetUndoEnabled(bool value, std::size_t undo_limit)
{
  if (value)
  {
    p_impl->m_command_stack = std::make_unique<CommandStack>();
    auto notifying_composer = CreateNotifyingComposer(&p_impl->m_event_handler, this);
    SetComposer(CreateCommandComposer(GetCommandStack(), std::move(notifying_composer)));
    p_impl->m_command_stack->SetUndoLimit(undo_limit);
  }
  else
  {
    p_impl->m_command_stack.reset();
    SetComposer(CreateNotifyingComposer(&p_impl->m_event_handler, this));
  }
}

}  // namespace mvvm
