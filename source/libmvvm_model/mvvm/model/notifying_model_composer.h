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

#ifndef MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_
#define MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_

#include <mvvm/interfaces/model_composer_interface.h>
#include <mvvm/interfaces/model_event_notifier_interface.h>
#include <mvvm/model/sessionitem.h>

namespace mvvm
{

class SessionModelInterface;

//! The decorator class to enhance standard ModelComposer with notification abilities.

template <typename T>
class NotifyingModelComposer : public T
{
public:
  static_assert(std::is_base_of<ModelComposerInterface, T>::value, "Invalid template argument");

  template <typename... Args>
  explicit NotifyingModelComposer(std::unique_ptr<ModelEventNotifierInterface> notifier,
                                  Args&&... args)
      : m_notifier(std::move(notifier)), T(std::forward<Args>(args)...)
  {
  }

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override
  {
    m_notifier->AboutToInsertItemNotify(parent, tag_index);
    auto result = T::InsertItem(std::move(item), parent, tag_index);
    m_notifier->ItemInsertedNotify(parent, tag_index);
    return result;
  }

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override
  {
    m_notifier->AboutToRemoveItemNotify(parent, tag_index);
    auto result = T::TakeItem(parent, tag_index);
    m_notifier->ItemRemovedNotify(parent, tag_index);
    return result;
  }

  bool SetData(SessionItem* item, const variant_t& value, int role) override
  {
    auto result = T::SetData(item, value, role);
    if (result)
    {
      m_notifier->DataChangedNotify(item, role);
    }
    return result;
  }

  void Reset(std::unique_ptr<SessionItem>& old_root_item,
             std::unique_ptr<SessionItem> new_root_item) override
  {
    m_notifier->ModelAboutToBeResetNotify(T::GetModel());
    T::Reset(old_root_item, std::move(new_root_item));
    m_notifier->ModelResetNotify(T::GetModel());
  }

private:
  std::unique_ptr<ModelEventNotifierInterface> m_notifier;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_
