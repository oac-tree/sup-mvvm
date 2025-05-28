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

#include <mvvm/commands/command_stack.h>
#include <mvvm/commands/default_command_processor.h>
#include <mvvm/commands/notifying_command_stack.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_event_handler.h>

namespace
{

std::function<void(const mvvm::event_variant_t&)> CreateNotifierCallback(
    mvvm::ModelEventHandler* event_handler)
{
  return [event_handler](const mvvm::event_variant_t& event) { event_handler->Notify(event); };
}

/**
 * @brief Creates command processor with undo/redo and notifications.
 */
std::unique_ptr<mvvm::ICommandStack> CreateCommandStackProcessor(
    mvvm::ModelEventHandler* event_handler, std::size_t undo_limit)
{
  return std::make_unique<mvvm::CommandStack>();
  // auto result = std::make_unique<mvvm::NotifyingCommandStack<mvvm::CommandStack>>(
  //     CreateNotifierCallback(event_handler));
  // result->SetUndoLimit(undo_limit);
  // return result;
}

/**
 * @brief Creates command processor with notifications and without undo/redo
 */
std::unique_ptr<mvvm::ICommandStack> CreateDefaultCommandProcessor(
    mvvm::ModelEventHandler* event_handler)
{
  // return std::make_unique<mvvm::NotifyingCommandStack<mvvm::DefaultCommandProcessor>>(
  //     CreateNotifierCallback(event_handler));
  return std::make_unique<mvvm::DefaultCommandProcessor>();
}

}  // namespace

namespace mvvm
{

struct ApplicationModel::ApplicationModelImpl
{
  ModelEventHandler m_event_handler;
  std::unique_ptr<ICommandStack> m_command_stack;  //! FIXME remove after SetComposer removal
};

ApplicationModel::ApplicationModel(std::string model_type)
    : ApplicationModel(std::move(model_type), {})
{
}

ApplicationModel::ApplicationModel(std::string model_type, std::shared_ptr<ItemPool> pool)
    : SessionModel(std::move(model_type), std::move(pool), {})
    , p_impl(std::make_unique<ApplicationModelImpl>())
{
  SetCommandStack(CreateDefaultCommandProcessor(&p_impl->m_event_handler));
}

ApplicationModel::~ApplicationModel()
{
  p_impl->m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(this);
}

ModelEventHandler* ApplicationModel::GetEventHandler() const
{
  return &p_impl->m_event_handler;
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
    SetCommandStack(CreateCommandStackProcessor(&p_impl->m_event_handler, undo_limit));
  }
  else
  {
    SetCommandStack(CreateDefaultCommandProcessor(&p_impl->m_event_handler));
  }
}

}  // namespace mvvm
