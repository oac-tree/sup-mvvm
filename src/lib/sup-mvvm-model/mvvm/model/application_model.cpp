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
#include <mvvm/commands/i_command.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_event_handler.h>

namespace
{

/**
 * @brief Creates command processor with undo/redo and notifications.
 */
std::unique_ptr<mvvm::ICommandStack> CreateCommandStackProcessor(std::size_t undo_limit)
{
  auto result = std::make_unique<mvvm::CommandStack>();
  result->SetUndoLimit(undo_limit);
  return result;
}

}  // namespace

namespace mvvm
{

struct ApplicationModel::ApplicationModelImpl
{
  ModelEventHandler m_event_handler;
};

ApplicationModel::ApplicationModel(std::string model_type)
    : ApplicationModel(std::move(model_type), {})
{
}

ApplicationModel::ApplicationModel(std::string model_type, std::shared_ptr<ItemPool> pool)
    : SessionModel(std::move(model_type), std::move(pool), {})
    , p_impl(std::make_unique<ApplicationModelImpl>())
{
  SetCommandProcessor(std::make_unique<mvvm::DefaultCommandProcessor>());
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

void ApplicationModel::ReplaceRootItem(std::unique_ptr<SessionItem> root_item)
{
  p_impl->m_event_handler.Notify<ModelAboutToBeResetEvent>(this);
  SessionModel::ReplaceRootItem(std::move(root_item));
  p_impl->m_event_handler.Notify<ModelResetEvent>(this);
}

void ApplicationModel::SetUndoEnabled(bool value, std::size_t undo_limit)
{
  if (value)
  {
    SetCommandProcessor(CreateCommandStackProcessor(undo_limit));
  }
  else
  {
    SetCommandProcessor(std::make_unique<mvvm::DefaultCommandProcessor>());
  }
}

}  // namespace mvvm
