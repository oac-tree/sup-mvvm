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

#ifndef MVVM_SIGNALS_EVENT_HANDLER_H_
#define MVVM_SIGNALS_EVENT_HANDLER_H_

#include <mvvm/core/exceptions.h>
#include <mvvm/core/type_map.h>
#include <mvvm/signals/signal_slot.h>

#include <functional>
#include <memory>

namespace mvvm
{

/**
 * @brief The EventHandler class provides a subscription/notification mechanism for various event
 * types.
 *
 * It relies on the functionality, provided by lsignal library.
 *
 * @tparam EventVariantT A variant to store the value of concrete event.
 * @see event_variant_t, ModelEventHandler
 *
 * @code{.cpp}
 *
 * class Widget
 * {
 *   void OnDataChangedEvent(const event_variant_t& event)
 *   {
 *     auto concrete_event = std::get<DataChangedEvent>(event);
 *     ...
 *   }
 *
 *   void OnItemInsertedEvent(const event_variant_t& event)
 *   {
 *     auto concrete_event = std::get<ItemInsertedEvent>(event);
 *     ...
 *   }
 * };
 *
 * Widget widget;
 *
 * EventHandler<event_variant_t> handler;
 * event_handler.Connect<DataChangedEvent>(&widget, &Widget::OnDataChangedEvent);
 * event_handler.Connect<ItemInsertedEvent>(&widget, &Widget::OnItemInsertedEvent);
 *
 * DataChangedEvent new_event{role, item};
 * event_handler.Notify<DataChangedEvent>(new_event);
 * @endcode
 */
template <typename EventVariantT>
class EventHandler
{
  /**
   * A definition of user callback type.
   */
  using callback_t = typename std::function<void(const EventVariantT&)>;

  /**
   * A definition of a signal. It stores a collection of callbacks, and notifies them when
   * necessary.
   */
  using signal_t = typename ::mvvm::Signal<void(const EventVariantT&)>;

public:
  /**
   * @brief Connects given callback to events specified by the given event type.
   *
   * If the slot is provided, its lifetime will be coupled with the provided callback. After the
   * slot's destruction, no callbacks will be called.
   *
   * @tparam EventT Concrete event type to subscribe.
   * @param callback A callback to notify the user.
   * @param slot A slot object to specify time of life of the callback.
   * @return Established connection (not used).
   */
  template <typename EventT>
  Connection Connect(const callback_t& callback, Slot* slot = nullptr)
  {
    return GetSignal<EventT>().connect(callback, slot);
  }

  /**
   * @brief Connects the receiver's method to all events specified by the given event type.
   *
   * If the slot is provided, its lifetime will be coupled with the provided callback. After the
   * slot's destruction, no callbacks will be called.
   *
   * @tparam EventT Concrete event type to subscribe.
   * @tparam ReceiverT The type of receiver object.
   *
   * @param receiver A pointer to the event receiver.
   * @param method A pointer to receiver's method.
   * @param slot A slot object to specify time of life of the callback.
   * @return Established connection (not used).
   */
  template <typename EventT, typename ReceiverT, typename Fn>
  Connection Connect(ReceiverT* receiver, const Fn& method, Slot* slot = nullptr)
  {
    return GetSignal<EventT>().connect(receiver, method, slot);
  }

  /**
   * @brief Notifies all slots connected to a given event type.
   * @param args Variadic arguments to construct concrete event to notify subscribers.
   */
  template <typename EventT, typename... Args>
  void Notify(Args&&... args)
  {
    EventT event{std::forward<Args>(args)...};
    Notify(event);
  }

  /**
   * @brief Notifies all slots connected to a given event type.
   * @param event A concrete event to notify subscribers.
   */
  template <typename EventT>
  void Notify(const EventT& event)
  {
    GetSignal<EventT>().operator()(event);
  }

  /**
   * @brief Registers given event type for subscription and notifications.
   *
   * @tparam EventT Concrete event type to register a signal.
   */
  template <typename EventT>
  void Register()
  {
    auto signal_for_event_type = std::make_unique<signal_t>();
    m_signals.template Put<EventT>(std::move(signal_for_event_type));
  }

private:
  /**
   * @brief Returns reference to a signal, registered for a given event type.
   */
  template <typename EventT>
  signal_t& GetSignal()
  {
    auto iter = m_signals.template Find<EventT>();
    if (iter == m_signals.end())
    {
      throw KeyNotFoundException("The type is not supported");
    }
    return *iter->second.get();
  }

  /**
   * @brief A map of signals, registered for concret event types.
   *
   * A single signal holds a collection of callbacks to notify.
   */
  TypeMap<std::unique_ptr<signal_t>> m_signals;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_EVENT_HANDLER_H_
