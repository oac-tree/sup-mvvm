/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/application_model.h"

#include <mvvm/commands/command_model_composer.h>
#include <mvvm/commands/command_stack.h>
#include <mvvm/interfaces/item_manager_interface.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/signals/model_event_handler.h>

namespace
{
std::unique_ptr<mvvm::ModelComposerInterface> CreateNotifyingComposer(
    mvvm::ModelEventHandler* notifier, mvvm::SessionModelInterface* model)
{
  return std::make_unique<mvvm::NotifyingModelComposer<mvvm::ModelComposer>>(notifier, *model);
}

std::unique_ptr<mvvm::ModelComposerInterface> CreateCommandComposer(
    mvvm::CommandStackInterface* command_stack,
    std::unique_ptr<mvvm::ModelComposerInterface> composer)
{
  return std::make_unique<mvvm::CommandModelComposer>(command_stack, std::move(composer));
}

}  // namespace

namespace mvvm
{

struct ApplicationModel::ApplicationModelImpl
{
  ModelEventHandler m_notifier;
  std::unique_ptr<CommandStackInterface> m_command_stack;
};

ApplicationModel::ApplicationModel(std::string model_type)
    : ApplicationModel(std::move(model_type), std::move(CreateDefaultItemManager()))
{
}

ApplicationModel::ApplicationModel(std::string model_type,
                                   std::unique_ptr<ItemManagerInterface> manager)
    : SessionModel(std::move(model_type), std::move(manager), {})
    , p_impl(std::make_unique<ApplicationModelImpl>())
{
  SetComposer(CreateNotifyingComposer(&p_impl->m_notifier, this));
}

ApplicationModel::~ApplicationModel()
{
  p_impl->m_notifier.ModelAboutToBeDestroyedNotify(this);
}

ModelEventHandler* ApplicationModel::GetSubscriber() const
{
  return &p_impl->m_notifier;
}

void ApplicationModel::CheckIn(SessionItem* item)
{
  SessionModel::CheckIn(item);
  item->Activate();
}

void ApplicationModel::SetUndoEnabled(bool value)
{
  if (value)
  {
    p_impl->m_command_stack = std::make_unique<CommandStack>();
    auto notifying_composer = CreateNotifyingComposer(&p_impl->m_notifier, this);
    SetComposer(std::move(CreateCommandComposer(GetCommandStack(), std::move(notifying_composer))));
  }
  else
  {
    p_impl->m_command_stack.reset();
    SetComposer(std::move(CreateNotifyingComposer(&p_impl->m_notifier, this)));
  }
}

CommandStackInterface* ApplicationModel::GetCommandStack() const
{
  return p_impl->m_command_stack.get();
}

}  // namespace mvvm
