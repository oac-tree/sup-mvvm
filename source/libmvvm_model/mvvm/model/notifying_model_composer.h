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
#include <mvvm/signals/model_event_notifier.h>

namespace mvvm
{

class SessionModelInterface;

//! The composer to build the model with notification abilities.

template<typename T>
class NotifyingModelComposer : public T
{
public:
  static_assert(std::is_base_of<ModelComposerInterface, T>::value, "Invalid template argument");

  template <typename... Args>
  explicit NotifyingModelComposer(Args &&... args) : T(std::forward<Args>(args)...) { }

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override
  {
    return T::InsertItem(std::move(item), parent, tag_index);
  }

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override
  {
    return T::TakeItem(parent, tag_index);
  }

  bool SetData(SessionItem* item, const variant_t& value, int role) override
  {
    return T::SetData(item, value, role);
  }

private:
  SessionModelInterface& m_model;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_NOTIFYING_MODEL_COMPOSER_H_
