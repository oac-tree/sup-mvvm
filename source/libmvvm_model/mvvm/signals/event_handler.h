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

#ifndef MVVM_EXPERIMENTAL_EVENT_HANDLER_H_
#define MVVM_EXPERIMENTAL_EVENT_HANDLER_H_

#include <mvvm/core/type_map.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/signals/signal_slot.h>

#include <functional>
#include <memory>
#include <stdexcept>

namespace mvvm
{

//! Provides subscription mechanism to various event types.
//! @code{.cpp}
//! EventHandler<event_t> handler;
//!
//! // to connect to signal
//! event_handler.Connect<DataChangedEvent>(&widget, &Widget::OnEvent);
//! event_handler.Connect<ItemInsertedEvent>(&widget, &Widget::OnEvent);
//!
//! // to notify widgets connected to one signal
//! event_handler.Notify<DataChangedEvent>(role, item);
//! @endcode

template <typename EventVariantT>
class EventHandler
{
  using callback_t = typename std::function<void(const EventVariantT&)>;
  using signal_t = typename ::mvvm::Signal<void(const EventVariantT&)>;

public:
  //! Connect given callback to all events specified by the given event type.
  //! @param callback A callback to be notified.
  //! @param slot A slot object to specify time of life of the callback.
  //! @note If slot is provided, it's lifetime will be coupled with the provided callback. After
  //! slot's destruction no callbacks will be called.

  template <typename EventT>
  Connection Connect(const callback_t& callback, Slot* slot = nullptr)
  {
    return GetSignal<EventT>().connect(callback, slot);
  }

  //! Connect object's method to all events specified by the given event type.
  //! @param callback A pointer to object ti.
  //! @param slot A slot object to specify time of life of the callback.
  //! @note If slot is provided, it's lifetime will be coupled with the provided callback. After
  //! slot's destruction no callbacks will be called.

  template <typename EventT, typename WidgetT, typename Fn>
  Connection Connect(WidgetT* widget, const Fn& method, Slot* slot = nullptr)
  {
    return GetSignal<EventT>().connect(widget, method, slot);
  }

  //! Notifies all slots connected to a given event type.
  //! @note Method constructs event using all provided arguments.

  template <typename EventT, typename... Args>
  void Notify(Args&&... args)
  {
    EventT event{std::forward<Args>(args)...};
    Notify(event);
  }

  //! Notifies all slots connected to a given event.

  template <typename EventT>
  void Notify(const EventT& event)
  {
    GetSignal<EventT>().operator()(event);
  }

  //! Registers given event type for subscription and notifications.
  template <typename EventT>
  void Register()
  {
    auto signal_for_event_type = std::make_unique<signal_t>();
    m_signals.template Put<EventT>(std::move(signal_for_event_type));
  }

private:
  template <typename EventT>
  signal_t& GetSignal()
  {
    auto it = m_signals.template Find<EventT>();
    if (it == m_signals.end())
    {
      throw std::runtime_error("The type is not supported");
    }
    return *it->second.get();
  }

  TypeMap<std::unique_ptr<signal_t>> m_signals;
};

}  // namespace mvvm

#endif  // MVVM_EXPERIMENTAL_EVENT_NOTIFIER_INTERFACE_H_
