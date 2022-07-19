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

#include "mvvm/model/notifying_model.h"

#include <mvvm/interfaces/item_manager_interface.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/validate_utils.h>
#include <mvvm/signals/model_event_notifier.h>

namespace mvvm
{
struct NotifyingModel::NotifyingModelImpl
{
  ModelEventNotifier m_notifier;
  std::unique_ptr<SessionModelInterface> m_decorated_model;

  explicit NotifyingModelImpl(std::unique_ptr<SessionModelInterface> decorated_model)
      : m_decorated_model(std::move(decorated_model))
  {
  }

  //! Returns real parent and index where item will be inserted.
  std::pair<SessionItem *, TagIndex> GetInsertData(SessionItem *parent, const TagIndex &tag_index)
  {
    if (!parent)
    {
      parent = m_decorated_model->GetRootItem();
    }

    return {parent, utils::GetActualInsertTagIndex(parent, tag_index)};
  }
};

NotifyingModel::NotifyingModel(std::unique_ptr<SessionModelInterface> decorated_model)
    : p_impl(std::make_unique<NotifyingModelImpl>(std::move(decorated_model)))
{
  GetDecoratedModel()->GetRootItem()->SetModel(this);
}

NotifyingModel::~NotifyingModel()
{
  p_impl->m_notifier.ModelAboutToBeDestroyedNotify(this);
}

std::string NotifyingModel::GetType() const
{
  return GetDecoratedModel()->GetType();
}

SessionItem *NotifyingModel::GetRootItem() const
{
  return GetDecoratedModel()->GetRootItem();
}

const ItemFactoryInterface *NotifyingModel::GetFactory() const
{
  return GetDecoratedModel()->GetFactory();
}

ModelEventSubscriberInterface *NotifyingModel::GetSubscriber() const
{
  return &p_impl->m_notifier;
}

SessionItem *NotifyingModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem *parent,
                                        const TagIndex &tag_index)
{
  // We have to know already here the actual parent and tag_index where item will be inserted
  // to provide correct notification.
  auto [actual_parent, actual_tag_index] = p_impl->GetInsertData(parent, tag_index);

  utils::ValidateItemInsert(item.get(), actual_parent, actual_tag_index);

  p_impl->m_notifier.AboutToInsertItemNotify(actual_parent, actual_tag_index);
  auto result = GetDecoratedModel()->InsertItem(std::move(item), actual_parent, actual_tag_index);
  p_impl->m_notifier.ItemInsertedNotify(actual_parent, actual_tag_index);

  return result;
}

std::unique_ptr<SessionItem> NotifyingModel::TakeItem(SessionItem *parent,
                                                      const TagIndex &tag_index)
{
  p_impl->m_notifier.AboutToRemoveItemNotify(parent, tag_index);
  auto result = utils::TakeItem(*this, parent, tag_index);
  p_impl->m_notifier.ItemRemovedNotify(parent, tag_index);
  return result;
}

void NotifyingModel::RemoveItem(SessionItem *item)
{
  utils::RemoveItem(*this, item);
}

void NotifyingModel::MoveItem(SessionItem *item, SessionItem *new_parent, const TagIndex &tag_index)
{
  utils::MoveItem(*this, item, new_parent, tag_index);
}

bool NotifyingModel::SetData(SessionItem *item, const variant_t &value, int role)
{
  auto result = GetDecoratedModel()->SetData(item, value, role);
  if (result)
  {
    p_impl->m_notifier.DataChangedNotify(item, role);
  }

  return result;
}

SessionItem *NotifyingModel::FindItem(const std::string &id) const
{
  return GetDecoratedModel()->FindItem(id);
}

void NotifyingModel::Clear(std::function<void(SessionItem *)> callback)
{
  p_impl->m_notifier.ModelAboutToBeResetNotify(this);
  GetDecoratedModel()->Clear(callback);
  GetRootItem()->SetModel(this);  // FIXME Simplify when method SetRootItem is implemented
  p_impl->m_notifier.ModelResetNotify(this);
}

void NotifyingModel::CheckIn(SessionItem *item)
{
  GetDecoratedModel()->CheckIn(item);
  item->Activate();
}

void NotifyingModel::CheckOut(SessionItem *item)
{
  GetDecoratedModel()->CheckOut(item);
}

const SessionModelInterface *NotifyingModel::GetDecoratedModel() const
{
  return p_impl->m_decorated_model.get();
}

SessionModelInterface *NotifyingModel::GetDecoratedModel()
{
  return const_cast<SessionModelInterface *>(
      static_cast<const NotifyingModel *>(this)->GetDecoratedModel());
}

}  // namespace mvvm
