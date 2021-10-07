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

#include <stdexcept>

namespace ModelView
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

    return {parent, TagIndex{tag_index.tag, actual_index}};
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

void ModelComposer::EstablishConnections(ModelEventListenerInterface* listener)
{
  p_impl->m_notifier->EstablishConnections(listener);
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

SessionItem* ModelComposer::InsertNewItem(const std::string& item_type, SessionItem* parent,
                                          const TagIndex& tag_index)
{
  // We have to know already here the actual parent and tag_index where item will be inserted
  // to provide correct notification.
  auto [actual_parent, actual_tag_index] = p_impl->GetInsertData(parent, tag_index);

  p_impl->m_notifier->AboutToInsertItemNotify(actual_parent, actual_tag_index);
  auto* result = p_impl->m_model->SessionModel::InsertNewItem(item_type, actual_parent, actual_tag_index);
  p_impl->m_notifier->ItemInsertedNotify(actual_parent, actual_tag_index);

  return result;
}

void ModelComposer::RemoveItem(SessionItem* parent, const TagIndex& tag_index)
{
  p_impl->m_notifier->AboutToRemoveItemNotify(parent, tag_index);
  p_impl->m_model->SessionModel::RemoveItem(parent, tag_index);
  p_impl->m_notifier->ItemRemovedNotify(parent, tag_index);
}

}  // namespace ModelView
