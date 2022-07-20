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
#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/validate_utils.h>
#include <mvvm/signals/model_event_notifier.h>

namespace mvvm
{
struct ApplicationModel::ApplicationModelImpl
{
  ModelEventNotifier m_notifier;
  ApplicationModel *m_self{nullptr};
  explicit ApplicationModelImpl(ApplicationModel *self) : m_self(self) {}

  //! Returns real parent and index where item will be inserted.
  std::pair<SessionItem *, TagIndex> GetInsertData(SessionItem *parent, const TagIndex &tag_index)
  {
    if (!parent)
    {
      parent = m_self->GetRootItem();
    }

    return {parent, utils::GetActualInsertTagIndex(parent, tag_index)};
  }
};

ApplicationModel::ApplicationModel(std::string model_type)
    : ApplicationModel(std::move(model_type), std::move(CreateDefaultItemManager()))
{
}

ApplicationModel::ApplicationModel(std::string model_type,
                                   std::unique_ptr<ItemManagerInterface> manager)
    : AbstractModelDecorator(
        std::make_unique<SessionModel>(std::move(model_type), std::move(manager)))
    , p_impl(std::make_unique<ApplicationModelImpl>(this))
{
  m_decorated_model->GetRootItem()->SetModel(this);
}

ApplicationModel::~ApplicationModel()
{
  p_impl->m_notifier.ModelAboutToBeDestroyedNotify(this);
}

ModelEventSubscriberInterface *ApplicationModel::GetSubscriber() const
{
  return &p_impl->m_notifier;
}

SessionItem *ApplicationModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem *parent,
                                          const TagIndex &tag_index)
{
  // We have to know already here the actual parent and tag_index where item will be inserted
  // to provide correct notification.
  auto [actual_parent, actual_tag_index] = p_impl->GetInsertData(parent, tag_index);

  utils::ValidateItemInsert(item.get(), actual_parent, actual_tag_index);

  p_impl->m_notifier.AboutToInsertItemNotify(actual_parent, actual_tag_index);
  auto result =
      AbstractModelDecorator::InsertItem(std::move(item), actual_parent, actual_tag_index);
  p_impl->m_notifier.ItemInsertedNotify(actual_parent, actual_tag_index);

  return result;
}

std::unique_ptr<SessionItem> ApplicationModel::TakeItem(SessionItem *parent,
                                                        const TagIndex &tag_index)
{
  p_impl->m_notifier.AboutToRemoveItemNotify(parent, tag_index);
  auto result = utils::TakeItem(*this, parent, tag_index);
  p_impl->m_notifier.ItemRemovedNotify(parent, tag_index);
  return result;
}

void ApplicationModel::RemoveItem(SessionItem *item)
{
  utils::RemoveItem(*this, item);
}

void ApplicationModel::MoveItem(SessionItem *item, SessionItem *new_parent,
                                const TagIndex &tag_index)
{
  utils::MoveItem(*this, item, new_parent, tag_index);
}

bool ApplicationModel::SetData(SessionItem *item, const variant_t &value, int role)
{
  auto result = AbstractModelDecorator::SetData(item, value, role);
  if (result)
  {
    p_impl->m_notifier.DataChangedNotify(item, role);
  }

  return result;
}

void ApplicationModel::Clear(std::unique_ptr<SessionItem> root_item)
{
  p_impl->m_notifier.ModelAboutToBeResetNotify(this);
  AbstractModelDecorator::Clear(std::move(root_item));
  GetRootItem()->SetModel(this); // FIXME Simplify when method SetRootItem is implemented
  p_impl->m_notifier.ModelResetNotify(this);
}

void ApplicationModel::CheckIn(SessionItem *item)
{
  AbstractModelDecorator::CheckIn(item);
  item->Activate();
}

}  // namespace mvvm
