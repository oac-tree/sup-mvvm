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

#ifndef MVVM_SIGNALS_ITEM_CONNECT_UTILS_H_
#define MVVM_SIGNALS_ITEM_CONNECT_UTILS_H_

//! @file
//! Utility functions to connect user callbacks with events happening with given item.

#include <mvvm/model_export.h>
#include <mvvm/signals/event_utils.h>
#include <mvvm/signals/model_event_handler.h>

#include <optional>

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace mvvm::connect
{

using callback_t = std::function<void(const event_variant_t&)>;

ModelEventHandler* GetEventHandler(const mvvm::SessionItem* item);

std::optional<PropertyChangedEvent> ConvertToPropertyChangedEvent(SessionItem* source,
                                                                  const event_variant_t& event);

//! Connect object's method to all events specified by the given event type and event source.
//! @param source A pointer to item that triggers events.
//! @param receiver A pointer to the object interested to receive events.
//! @param method A receiver's method.
//! @param slot A slot object to specify time of life of the callback.
//! @note If slot is provided, it's lifetime will be coupled with the provided callback. After
//! slot's destruction no callbacks will be called.

template <typename EventT, typename ReceiverT>
void Connect(SessionItem* source, ReceiverT* receiver, void (ReceiverT::*method)(const EventT&),
             Slot* slot = nullptr)
{
  // special case for PropertyChangedEvent
  if constexpr (std::is_same_v<EventT, PropertyChangedEvent>)
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

  // all other events
  else
  {
    // a callback with filtering capabilities to subscribe to the model
    auto adapter = [source, receiver, method](const event_variant_t& event)
    {
      auto concrete_event = std::get<EventT>(event);
      // only events which are coming from the requested source will be propagated
      if (concrete_event.m_item == source)
      {
        std::invoke(method, *receiver, concrete_event);
      }
    };
    GetEventHandler(source)->Connect<EventT>(adapter, slot);
  }
}

template <typename EventT, typename ReceiverT>
void Connect(SessionItem* source, ReceiverT* receiver,
             void (ReceiverT::*method)(const event_variant_t&), Slot* slot = nullptr)
{
  // special case for PropertyChangedEvent
  if constexpr (std::is_same_v<EventT, PropertyChangedEvent>)
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

  // all other events
  else
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
}

template <typename EventT>
void Connect(SessionItem* source, const std::function<void(const event_variant_t&)>& callback,
             Slot* slot = nullptr)
{
  // special case for PropertyChangedEvent
  if constexpr (std::is_same_v<EventT, PropertyChangedEvent>)
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

  // all other events
  else
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
}

}  // namespace mvvm::connect

#endif  // MVVM_SIGNALS_ITEM_CONNECT_UTILS_H_
