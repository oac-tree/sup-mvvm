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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_
#define MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_

#include <mvvm/model/i_model_composer.h>
#include <mvvm/model/session_item.h>
#include <mvvm/signals/model_event_handler.h>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The NotifyingModelComposer class enhances standard ModelComposer with notification
 * abilities.
 */
template <typename T>
class NotifyingModelComposer : public T
{
public:
  static_assert(std::is_base_of<IModelComposer, T>::value, "Invalid template argument");

  template <typename... Args>
  explicit NotifyingModelComposer(ModelEventHandler* notifier, Args&&... args)
      :  T(std::forward<Args>(args)...), m_event_handler(notifier)
  {
  }

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override
  {
    m_event_handler->Notify<AboutToInsertItemEvent>(parent, tag_index);
    auto result = T::InsertItem(std::move(item), parent, tag_index);
    m_event_handler->Notify<ItemInsertedEvent>(parent, tag_index);
    return result;
  }

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override
  {
    m_event_handler->Notify<AboutToRemoveItemEvent>(parent, tag_index);
    auto result = T::TakeItem(parent, tag_index);
    m_event_handler->Notify<ItemRemovedEvent>(parent, tag_index);
    return result;
  }

  bool SetData(SessionItem* item, const variant_t& value, int role) override
  {
    auto result = T::SetData(item, value, role);
    if (result)
    {
      m_event_handler->Notify<DataChangedEvent>(item, role);
    }
    return result;
  }

  void ReplaceRootItem(std::unique_ptr<SessionItem>& old_root_item,
                       std::unique_ptr<SessionItem> new_root_item) override
  {
    m_event_handler->Notify<ModelAboutToBeResetEvent>(T::GetModel());
    T::ReplaceRootItem(old_root_item, std::move(new_root_item));
    m_event_handler->Notify<ModelResetEvent>(T::GetModel());
  }

private:
  ModelEventHandler* m_event_handler{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_
