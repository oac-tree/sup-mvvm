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

#include "mvvm/model/modelcomposer.h"

#include "mvvm/interfaces/modeleventnotifierinterface.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taggeditems.h"

#include <stdexcept>

namespace mvvm
{
struct ModelComposer::ModelComposerImpl
{
  SessionModel* m_model{nullptr};
  ModelEventNotifierInterface* m_notifier{nullptr};

  //! Returns real parent and index where item will be inserted.
  std::pair<SessionItem*, TagIndex> GetInsertData(SessionItem* parent, const TagIndex& tag_index)
  {
    if (!parent)
    {
      parent = m_model->GetRootItem();
    }

    int actual_index = tag_index.index < 0 ? parent->GetItemCount(tag_index.tag) : tag_index.index;

    std::string actual_tag =
        tag_index.tag.empty() ? parent->GetTaggedItems()->GetDefaultTag() : tag_index.tag;

    return {parent, TagIndex{actual_tag, actual_index}};
  }
};

ModelComposer::ModelComposer(SessionModel* model, ModelEventNotifierInterface* notifier)
    : p_impl(std::make_unique<ModelComposerImpl>())
{
  if (!model)
  {
    throw std::runtime_error("Error in ModelComposer: uninitialised model");
  }

  if (!notifier)
  {
    throw std::runtime_error("Error in ModelComposer: uninitialised notifier");
  }

  p_impl->m_model = model;
  p_impl->m_notifier = notifier;
}

ModelComposer::~ModelComposer() = default;

bool ModelComposer::SetData(SessionItem* item, const variant_t& value, int role)
{
  auto result = p_impl->m_model->SessionModel::SetData(item, value, role);
  if (result)
  {
    p_impl->m_notifier->DataChangedNotify(item, role);
  }

  return result;
}

SessionItem* ModelComposer::InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                       const TagIndex& tag_index)
{
  // We have to know already here the actual parent and tag_index where item will be inserted
  // to provide correct notification.
  auto [actual_parent, actual_tag_index] = p_impl->GetInsertData(parent, tag_index);

  p_impl->m_notifier->AboutToInsertItemNotify(actual_parent, actual_tag_index);
  auto result =
      p_impl->m_model->SessionModel::InsertItem(std::move(item), actual_parent, actual_tag_index);
  p_impl->m_notifier->ItemInsertedNotify(actual_parent, actual_tag_index);

  return result;
}

SessionItem* ModelComposer::InsertNewItem(const std::string& item_type, SessionItem* parent,
                                          const TagIndex& tag_index)
{
  // We have to know already here the actual parent and tag_index where item will be inserted
  // to provide correct notification.
  auto [actual_parent, actual_tag_index] = p_impl->GetInsertData(parent, tag_index);

  p_impl->m_notifier->AboutToInsertItemNotify(actual_parent, actual_tag_index);
  auto result =
      p_impl->m_model->SessionModel::InsertNewItem(item_type, actual_parent, actual_tag_index);
  p_impl->m_notifier->ItemInsertedNotify(actual_parent, actual_tag_index);

  return result;
}

std::unique_ptr<SessionItem> ModelComposer::TakeItem(SessionItem* parent, const TagIndex& tag_index)
{
  p_impl->m_notifier->AboutToRemoveItemNotify(parent, tag_index);
  auto result = p_impl->m_model->SessionModel::TakeItem(parent, tag_index);
  p_impl->m_notifier->ItemRemovedNotify(parent, tag_index);
  return result;
}

void ModelComposer::RemoveItem(SessionItem* item)
{
  if (!item)
  {
    throw std::runtime_error("Item is not initialised");
  }
  TakeItem(item->GetParent(), item->GetTagIndex());
}

void ModelComposer::MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index)
{
  // For the moment MoveItem is implemented in SessionModel::MoveItem as two subsequent calls:
  // TakeItem + InsertItem. This is turn will trigger four notifications:
  // AboutToRemoveItemNotify, ItemRemovedNotify, AboutToInsertItemNotify, ItemInsertedNotify.
  // If we decide to introduce own notifications for move, we will have to implement it here.
  throw std::runtime_error("Not implemented");
}

void ModelComposer::Clear(std::function<void(SessionItem*)> callback)
{
  p_impl->m_notifier->ModelAboutToBeResetNotify(p_impl->m_model);
  p_impl->m_model->SessionModel::Clear(callback);
  p_impl->m_notifier->ModelResetNotify(p_impl->m_model);
}

}  // namespace mvvm
