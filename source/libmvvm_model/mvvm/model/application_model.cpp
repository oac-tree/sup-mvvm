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

#include <mvvm/interfaces/item_manager_interface.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/signals/model_event_notifier.h>

namespace
{
std::unique_ptr<mvvm::ModelComposerInterface> CreateComposer(
    mvvm::ModelEventNotifierInterface* notifier, mvvm::SessionModelInterface* model)
{
  return std::make_unique<mvvm::NotifyingModelComposer<mvvm::ModelComposer>>(notifier, *model);
}

}  // namespace

namespace mvvm
{

struct ApplicationModel::ApplicationModelImpl
{
  ModelEventNotifier m_notifier;
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
  SetComposer(CreateComposer(&p_impl->m_notifier, this));
}

ApplicationModel::~ApplicationModel()
{
  p_impl->m_notifier.ModelAboutToBeDestroyedNotify(this);
}

ModelEventSubscriberInterface* ApplicationModel::GetSubscriber() const
{
  return &p_impl->m_notifier;
}

void ApplicationModel::CheckIn(SessionItem* item)
{
  SessionModel::CheckIn(item);
  item->Activate();
}

}  // namespace mvvm
