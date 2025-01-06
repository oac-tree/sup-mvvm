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

#ifndef MVVM_SIGNALS_ITEM_CONNECT_H_
#define MVVM_SIGNALS_ITEM_CONNECT_H_

//! @file
//! Utility functions to connect user callbacks with events happening with given item.

#include <mvvm/model_export.h>
#include <mvvm/signals/item_connect_helper.h>
#include <mvvm/signals/model_event_handler.h>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace mvvm::connect
{

/**
 * @brief Connects a callback to events specified by the given event type.
 *
 * This version is intended for user callbacks accepting event_variant_t (PropertyChangedEvent
 * excluded).
 *
 * @tparam EventT The type of the event to subscribe.
 *
 * @param source A pointer to an item that trigger events.
 * @param callback A callback based on event_variant_t.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
inline typename std::enable_if_t<(not std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, const std::function<void(const event_variant_t&)>& callback,
        Slot* slot = nullptr)
{
  // a callback with filtering capabilities to subscribe to the model
  auto adapter = [source, callback](const event_variant_t& event)
  {
    // only events which are coming from the requested source will be propagated
    if (GetEventSource(event) == source)
    {
      callback(event);
    }
  };
  GetEventHandler(source)->Connect<EventT>(adapter, slot);
}

/**
 * @brief Connects a callback to events specified by the given event type.
 *
 * This version is intended for user callbacks accepting event_variant_t (PropertyChangedEvent
 * only).
 *
 * @tparam EventT The type of the event to subscribe.
 *
 * @param source A pointer to an item that trigger events.
 * @param callback A callback based on event_variant_t.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
inline typename std::enable_if_t<(std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, const std::function<void(const event_variant_t&)>& callback,
        Slot* slot = nullptr)
{
  // A callback with filtering capabilities to subscribe to the model. Additionally it changes an
  // event type: DataChangedEvent will turn to the PropertyChangedEvent.
  auto adapter = [source, callback](const event_variant_t& event)
  {
    auto concrete_event = ConvertToPropertyChangedEvent(source, event);

    if (concrete_event.has_value())
    {
      callback(concrete_event.value());
    }
  };
  // note: subscribe to DataChangedEvent, not PropertyChangedEvent
  GetEventHandler(source)->Connect<DataChangedEvent>(adapter, slot);
}

/**
 * @brief Connects a callback to events specified by the given event type.
 *
 * This version is intended for user callbacks accepting concrete event (except
 * PropertyChangedEvent).
 *
 * @tparam EventT The type of the event to subscribe.
 *
 * @param source A pointer to an item that trigger events.
 * @param callback A callback based on EventT.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
inline typename std::enable_if_t<(not std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, const std::function<void(const EventT&)>& callback,
        Slot* slot = nullptr)
{
  // a callback with filtering capabilities to subscribe to the model
  auto adapter = [source, callback](const event_variant_t& event_variant)
  {
    // only events which are coming from the requested source will be propagated
    if (GetEventSource(event_variant) == source)
    {
      callback(std::get<EventT>(event_variant));
    }
  };
  GetEventHandler(source)->Connect<EventT>(adapter, slot);
}

/**
 * @brief Connects a callback to events specified by the given event type.
 *
 * This version is intended for user callbacks accepting concrete event (PropertyChangedEvent only).
 *
 * @tparam EventT The type of the event to subscribe.
 *
 * @param source A pointer to an item that trigger events.
 * @param callback A callback based on EventT.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
inline typename std::enable_if_t<(std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, const std::function<void(const EventT&)>& callback,
        Slot* slot = nullptr)
{
  // A callback with filtering capabilities to subscribe to the model. Additionally it changes an
  // event type: DataChangedEvent will turn to the PropertyChangedEvent.
  auto adapter = [source, callback](const event_variant_t& event)
  {
    auto concrete_event = ConvertToPropertyChangedEvent(source, event);

    if (concrete_event.has_value())
    {
      callback(concrete_event.value());
    }
  };
  // note: subscribe to DataChangedEvent, not PropertyChangedEvent
  GetEventHandler(source)->Connect<DataChangedEvent>(adapter, slot);
}

/**
 * @brief Connects object's method to events specified by the given event type.
 *
 * This version is intended for methods accepting concrete event (PropertyChangedEvent excluded).
 *
 * @tparam EventT The type of the event to subscribe.
 * @tparam ReceiverT The type of the receiver.
 *
 * @param source A pointer to an item that trigger events.
 * @param receiver A pointer to the event receiver.
 * @param method A pointer to receiver's method.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT, typename ReceiverT>
inline typename std::enable_if_t<(not std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, ReceiverT* receiver, void (ReceiverT::*method)(const EventT&),
        Slot* slot = nullptr)
{
  // a callback with filtering capabilities to subscribe to the model
  auto adapter = [source, receiver, method](const event_variant_t& event)
  {
    auto concrete_event = std::get<EventT>(event);
    // only events which are coming from the requested source will be propagated
    if (concrete_event.item == source)
    {
      std::invoke(method, *receiver, concrete_event);
    }
  };
  GetEventHandler(source)->Connect<EventT>(adapter, slot);
}

/**
 * @brief Connects object's method to events specified by the given event type.
 *
 * This version is intended for methods accepting concrete PropertyChangedEvent only.
 *
 * @tparam EventT The type of the event to subscribe.
 * @tparam ReceiverT The type of the receiver.
 *
 * @param source A pointer to an item that trigger events.
 * @param receiver A pointer to the event receiver.
 * @param method A pointer to receiver's method.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT, typename ReceiverT>
inline typename std::enable_if_t<(std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, ReceiverT* receiver, void (ReceiverT::*method)(const EventT&),
        Slot* slot = nullptr)
{
  // A callback with filtering capabilities to subscribe to the model. Additionally it changes an
  // event type: DataChangedEvent will turn to the PropertyChangedEvent.
  auto adapter = [source, receiver, method](const event_variant_t& event)
  {
    auto concrete_event = ConvertToPropertyChangedEvent(source, event);

    if (concrete_event.has_value())
    {
      std::invoke(method, *receiver, concrete_event.value());
    }
  };
  // note: subscribe to DataChangedEvent, not PropertyChangedEvent
  GetEventHandler(source)->Connect<DataChangedEvent>(adapter, slot);
}

/**
 * @brief Connects object's method to events specified by the given event type.
 *
 * This version is intended for methods accepting event_variant_t (PropertyChangedEvent excluded).
 *
 * @tparam EventT The type of the event to subscribe.
 * @tparam ReceiverT The type of the receiver.
 *
 * @param source A pointer to an item that trigger events.
 * @param receiver A pointer to the event receiver.
 * @param method A pointer to receiver's method.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT, typename ReceiverT>
inline typename std::enable_if_t<(not std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, ReceiverT* receiver, void (ReceiverT::*method)(const event_variant_t&),
        Slot* slot = nullptr)
{
  auto adapter = [source, receiver, method](const event_variant_t& event)
  {
    // only events which are coming from the requested source will be propagated
    if (GetEventSource(event) == source)
    {
      std::invoke(method, *receiver, event);
    }
  };
  GetEventHandler(source)->Connect<EventT>(adapter, slot);
}

/**
 * @brief Connects object's method to events specified by the given event type.
 *
 * This version is intended for methods accepting event_variant_t (PropertyChangedEvent only).
 *
 * @tparam EventT The type of the event to subscribe.
 * @tparam ReceiverT The type of the receiver.
 *
 * @param source A pointer to an item that trigger events.
 * @param receiver A pointer to the event receiver.
 * @param method A pointer to receiver's method.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, it's lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT, typename ReceiverT>
inline typename std::enable_if_t<(std::is_same_v<EventT, mvvm::PropertyChangedEvent>), void>
Connect(SessionItem* source, ReceiverT* receiver, void (ReceiverT::*method)(const event_variant_t&),
        Slot* slot = nullptr)
{
  // A callback with filtering capabilities to subscribe to the model. Additionally it changes an
  // event type: DataChangedEvent will turn to the PropertyChangedEvent.
  auto adapter = [source, receiver, method](const event_variant_t& event)
  {
    auto concrete_event = ConvertToPropertyChangedEvent(source, event);

    if (concrete_event.has_value())
    {
      std::invoke(method, *receiver, concrete_event.value());
    }
  };
  // note: subscribe to DataChangedEvent, not PropertyChangedEvent
  GetEventHandler(source)->Connect<DataChangedEvent>(adapter, slot);
}

}  // namespace mvvm::connect

#endif  // MVVM_SIGNALS_ITEM_CONNECT_H_
