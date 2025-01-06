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

#ifndef MVVM_SIGNALS_MODEL_LISTENER_H_
#define MVVM_SIGNALS_MODEL_LISTENER_H_

#include <mvvm/signals/model_event_handler.h>

#include <memory>

namespace mvvm
{

class ISessionModel;
class ModelEventHandler;

/**
 * @brief The ModelListenerBase is a base for all objects willing to listen for changes in
 * ApplicationModel.
 *
 * The time of life of all callbacks, or connected objects, should exceed the time of life of this
 * ModelListener. When ModelListener is deleted, it safely unsubscribes everything from the model
 * notifications.
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
class MVVM_MODEL_EXPORT ModelListener
{
public:
  explicit ModelListener(const ISessionModel* model);
  virtual ~ModelListener();

  ModelListener& operator=(const ModelListener& other) = delete;
  ModelListener(const ModelListener& other) = delete;

  /**
   * @brief Connects a callback to events specified by the given event type.
   *
   * This version is intended for user callbacks accepting event_variant_t.
   *
   * @tparam EventT The type of the event to subscribe.
   * @param callback A callback based on event_variant_t.
   */
  template <typename EventT>
  void Connect(const std::function<void(const event_variant_t&)>& callback)
  {
    GetEventHandler()->Connect<EventT>(callback, GetSlot());
  }

  /**
   * @brief Connects a callback to events specified by the given event type.
   *
   * This version is intended for user callbacks accepting concrete event.
   *
   * @tparam EventT The type of the event to subscribe.
   * @param callback A callback based on event_variant_t.
   */
  template <typename EventT>
  void Connect(const std::function<void(const EventT&)>& callback)
  {
    auto adapter = [callback](const event_variant_t& event_variant)
    { callback(std::get<EventT>(event_variant)); };
    Connect<EventT>(adapter);
  }

  /**
   * @brief Connects object's method to events specified by the given event type.
   *
   * This version is intended for methods accepting event_variant_t.
   *
   * @tparam EventT The type of the event to subscribe.
   * @tparam ReceiverT The type of the receiver.
   *
   * @param receiver A pointer to the event receiver.
   * @param method A pointer to receiver's method.
   */
  template <typename EventT, typename ReceiverT>
  void Connect(ReceiverT* receiver, void (ReceiverT::*method)(const event_variant_t&))
  {
    GetEventHandler()->Connect<EventT>(receiver, method, GetSlot());
  }

  /**
   * @brief Connects object's method to events specified by the given event type.
   *
   * This version is intended for methods accepting concrete event.
   *
   * @tparam EventT The type of the event to subscribe.
   * @tparam ReceiverT The type of the receiver.
   *
   * @param receiver A pointer to the event receiver.
   * @param method A pointer to receiver's method.
   */
  template <typename EventT, typename ReceiverT>
  void Connect(ReceiverT* receiver, void (ReceiverT::*method)(const EventT&))
  {
    auto adapter = [receiver, method](const event_variant_t& event)
    {
      auto concrete_event = std::get<EventT>(event);
      std::invoke(method, *receiver, concrete_event);
    };
    Connect<EventT>(adapter);
  }

  const ISessionModel* GetModel() const;

private:
  ModelEventHandler* GetEventHandler();
  Slot* GetSlot() const;

  const ISessionModel* m_model{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_LISTENER_H_
