/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/applicationmodel.h"

#include "mvvm/interfaces/modeleventlistenerinterface.h"
#include "mvvm/model/modelcomposer.h"
#include "mvvm/viewmodel/modeleventnotifier.h"

namespace ModelView
{
struct ApplicationModel::ApplicationModelImpl
{
  ModelEventNotifier m_notifier;
  ModelComposer m_composer;
  explicit ApplicationModelImpl(SessionModel *self) : m_composer(self, &m_notifier) {}
};

ApplicationModel::ApplicationModel(std::string model_type, std::shared_ptr<ItemPool> pool)
    : SessionModel(std::move(model_type), std::move(pool))
    , p_impl(std::make_unique<ApplicationModelImpl>(this))
{
}

ApplicationModel::~ApplicationModel()
{
  p_impl->m_notifier.ModelAboutToBeDestroyedNotify(this);
}

void ApplicationModel::Subscribe(ModelEventListenerInterface *listener)
{
  p_impl->m_notifier.Subscribe(listener);
}

SessionItem *ApplicationModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem *parent,
                                          const TagIndex &tag_index)
{
  return p_impl->m_composer.InsertItem(std::move(item), parent, tag_index);
}

std::unique_ptr<SessionItem> ApplicationModel::TakeItem(SessionItem *parent,
                                                        const TagIndex &tag_index)
{
  return p_impl->m_composer.TakeItem(parent, tag_index);
}

void ApplicationModel::RemoveItem(SessionItem *item)
{
  p_impl->m_composer.RemoveItem(item);
}

bool ApplicationModel::SetData(SessionItem *item, const variant_t &value, int role)
{
  return p_impl->m_composer.SetData(item, value, role);
}

void ApplicationModel::Clear(std::function<void(SessionItem *)> callback)
{
  return p_impl->m_composer.Clear(callback);
}

}  // namespace ModelView
