/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SIGNALS_ITEM_CONNECT_H_
#define MVVM_SIGNALS_ITEM_CONNECT_H_

//! @file
//! Utility functions to connect user callbacks with events happening with given item.

#include <mvvm/signals/item_connect_helper.h>
#include <mvvm/signals/model_event_handler.h>

#include <functional>
#include <optional>
#include <type_traits>
#include <variant>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace mvvm::connect
{

namespace detail
{

/**
 * @brief The model event we actually subscribe to in order to serve a requested EventT.
 *
 * For most events this is the event itself. PropertyChangedEvent is special: the model never emits
 * it, so we subscribe to DataChangedEvent and synthesize PropertyChangedEvent on the fly (see
 * PrepareEvent below). This alias is the single place where that substitution of the subscription
 * type is expressed.
 */
template <typename EventT>
using model_event_t =
    std::conditional_t<std::is_same_v<EventT, PropertyChangedEvent>, DataChangedEvent, EventT>;

/**
 * @brief Decides whether a raw model event is relevant to the given source item, and returns the
 * event to deliver to a subscriber (or nullopt if the event should be ignored).
 *
 * This is the single place where event filtering by source and PropertyChangedEvent substitution
 * are expressed. For PropertyChangedEvent the incoming DataChangedEvent of a property child is
 * converted into a PropertyChangedEvent of its parent; for any other event the raw event is
 * forwarded as-is when its source matches.
 */
template <typename EventT>
std::optional<event_variant_t> PrepareEvent(SessionItem* source, const event_variant_t& raw)
{
  if constexpr (std::is_same_v<EventT, PropertyChangedEvent>)
  {
    if (auto concrete_event = ConvertToPropertyChangedEvent(source, raw); concrete_event.has_value())
    {
      return event_variant_t{concrete_event.value()};
    }
    return std::nullopt;
  }
  else
  {
    return GetEventSource(raw) == source ? std::optional<event_variant_t>{raw} : std::nullopt;
  }
}

/**
 * @brief Subscribes the given source to its model events and forwards relevant ones to the sink.
 *
 * This is the single core used by all public Connect() overloads. It subscribes to the appropriate
 * model event (see model_event_t), filters and substitutes via PrepareEvent, and hands the
 * resulting event_variant_t to the sink. The public overloads differ only in how they build the
 * sink.
 *
 * @tparam EventT The type of the event the subscriber is interested in.
 *
 * @param source A pointer to an item that triggers events.
 * @param sink A callback accepting the prepared event_variant_t.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, its lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
void ConnectSink(SessionItem* source, std::function<void(const event_variant_t&)> sink, Slot* slot)
{
  auto adapter = [source, sink = std::move(sink)](const event_variant_t& raw)
  {
    if (auto prepared = PrepareEvent<EventT>(source, raw); prepared.has_value())
    {
      sink(prepared.value());
    }
  };
  (void)GetEventHandler(source)->Connect<model_event_t<EventT>>(adapter, slot);
}

}  // namespace detail

/**
 * @brief Connects a callback to events specified by the given event type.
 *
 * This version is intended for user callbacks accepting event_variant_t.
 *
 * @tparam EventT The type of the event to subscribe.
 *
 * @param source A pointer to an item that triggers events.
 * @param callback A callback based on event_variant_t.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, its lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
void Connect(SessionItem* source, const std::function<void(const event_variant_t&)>& callback,
             Slot* slot = nullptr)
{
  detail::ConnectSink<EventT>(source, callback, slot);
}

/**
 * @brief Connects a callback to events specified by the given event type.
 *
 * This version is intended for user callbacks accepting concrete event.
 *
 * @tparam EventT The type of the event to subscribe.
 *
 * @param source A pointer to an item that triggers events.
 * @param callback A callback based on EventT.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, its lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT>
void Connect(SessionItem* source, const std::function<void(const EventT&)>& callback,
             Slot* slot = nullptr)
{
  detail::ConnectSink<EventT>(
      source, [callback](const event_variant_t& event) { callback(std::get<EventT>(event)); },
      slot);
}

/**
 * @brief Connects object's method to events specified by the given event type.
 *
 * This version is intended for methods accepting concrete event.
 *
 * @tparam EventT The type of the event to subscribe.
 * @tparam ReceiverT The type of the receiver.
 *
 * @param source A pointer to an item that triggers events.
 * @param receiver A pointer to the event receiver.
 * @param method A pointer to receiver's method.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, its lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT, typename ReceiverT>
void Connect(SessionItem* source, ReceiverT* receiver, void (ReceiverT::*method)(const EventT&),
             Slot* slot = nullptr)
{
  detail::ConnectSink<EventT>(
      source,
      [receiver, method](const event_variant_t& event)
      { std::invoke(method, *receiver, std::get<EventT>(event)); },
      slot);
}

/**
 * @brief Connects object's method to events specified by the given event type.
 *
 * This version is intended for methods accepting event_variant_t.
 *
 * @tparam EventT The type of the event to subscribe.
 * @tparam ReceiverT The type of the receiver.
 *
 * @param source A pointer to an item that triggers events.
 * @param receiver A pointer to the event receiver.
 * @param method A pointer to receiver's method.
 * @param slot A slot object to specify time of life of the callback.
 *
 * @note If slot is provided, its lifetime will be coupled with the provided callback. After slot's
 * destruction no callbacks will be called.
 */
template <typename EventT, typename ReceiverT>
void Connect(SessionItem* source, ReceiverT* receiver,
             void (ReceiverT::*method)(const event_variant_t&), Slot* slot = nullptr)
{
  detail::ConnectSink<EventT>(
      source,
      [receiver, method](const event_variant_t& event) { std::invoke(method, *receiver, event); },
      slot);
}

}  // namespace mvvm::connect

#endif  // MVVM_SIGNALS_ITEM_CONNECT_H_
