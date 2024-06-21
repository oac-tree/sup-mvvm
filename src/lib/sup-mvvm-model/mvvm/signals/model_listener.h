/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SIGNALS_MODEL_LISTENER_H_
#define MVVM_SIGNALS_MODEL_LISTENER_H_

#include <mvvm/signals/model_listener_base.h>
#include <mvvm/signals/model_listener_fwd.h>  // to pick up the default template parameter

namespace mvvm
{

/**
 * @brief The ModelListener class is a templated class to listen for changes in concrete model.
 *
 * It can be used as a base class, or as a standalone listener. Objects connect to the model via the
 * listener with their callbacks. The lifetime of connected objects should exceed the lifetime
 * of this listener. The callbacks will be called on the model change as long as the listener is
 * alive.
 *
 * @tparam ModelT Type of the concrete SessionModel to listen.
 *
 * @code{.cpp}
 *
 * class Widget
 * {
 *   void OnAnyEvent(const event_variant_t& event)
 *   {
 *     ...
 *   }
 *
 *   void OnItemInsertedEvent(const ItemInsertedEvent& event)
 *   {
 *     ...
 *   }
 * };
 *
 * Widget widget;
 *
 * ModelListener<SessionModel> listener;
 * listener.Connect<DataChangedEvent>(&widget, &Widget::OnAnyEvent);
 * listener.Connect<ItemInsertedEvent>(&widget, &Widget::OnAnyEvent);
 * listener.Connect<ItemInsertedEvent>(&widget, &Widget::OnItemInsertedEvent);
 * @endcode
 */
template <typename ModelT>
class ModelListener : public ModelListenerBase
{
public:
  explicit ModelListener(const ModelT* model) : ModelListenerBase(model) {}
  const ModelT* GetModel() const { return static_cast<const ModelT*>(GetModelBase()); }
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_LISTENER_H_
