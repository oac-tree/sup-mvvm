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

#ifndef MVVM_SIGNALS_MODEL_LISTENER_BASE_H_
#define MVVM_SIGNALS_MODEL_LISTENER_BASE_H_

#include <mvvm/signals/model_event_handler.h>

#include <memory>

namespace mvvm
{

class SessionModelInterface;
class ModelEventHandler;

/**
 * @brief The ModelListenerBase is a base for all objects willing to listen for changes in
 * ApplicationModel.
 *
 * The time of life of all callbacks, or connected objects, should exceed the time of life of this
 * ModelListener. When ModelListener is deleted, it safely unsubscribes everything from the model
 * notifications.
 */
class MVVM_MODEL_EXPORT ModelListenerBase
{
public:
  explicit ModelListenerBase(const SessionModelInterface* model);
  virtual ~ModelListenerBase();

  ModelListenerBase& operator=(const ModelListenerBase& other) = delete;
  ModelListenerBase(const ModelListenerBase& other) = delete;

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
  void Connect(const std::function<void(const EventT&)>& func)
  {
    auto adapter = [func](const event_variant_t& event_variant)
    { func(std::get<EventT>(event_variant)); };
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

protected:
  const SessionModelInterface* GetModelBase() const;

private:
  ModelEventHandler* GetEventHandler();
  Slot* GetSlot() const;

  struct ModelListenerBaseImpl;
  std::unique_ptr<ModelListenerBaseImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_LISTENER_BASE_H_
