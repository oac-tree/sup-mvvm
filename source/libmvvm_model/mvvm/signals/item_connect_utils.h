/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

//! @file item_connect_utils.h
//! Utility functions to connect user callbacks with events happening with given item.

#include <mvvm/model_export.h>
#include <mvvm/signals/callback_types.h>
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
//! @param widget A pointer to the object interested to receive events.
//! @param method An object's method.
//! @param slot A slot object to specify time of life of the callback.
//! @note If slot is provided, it's lifetime will be coupled with the provided callback. After
//! slot's destruction no callbacks will be called.

template <typename EventT, typename WidgetT>
void Connect(SessionItem* source, WidgetT* widget, void (WidgetT::*method)(const EventT&),
             Slot* slot = nullptr)
{
  // special case for PropertyChangedEvent
  if constexpr (std::is_same_v<EventT, PropertyChangedEvent>)
  {
    // A callback with filtering capabilities to subscribe to the model. Additionally it changes an
    // event type: DataChangedEvent will turn to the PropertyChangedEvent.
    auto adapter = [source, widget, method](const event_variant_t& event)
    {
      auto concrete_event = ConvertToPropertyChangedEvent(source, event);

      if (concrete_event.has_value())
      {
        std::invoke(method, *widget, concrete_event.value());
      }
    };
    // we subscribe to DataChangedEvent an adapter
    GetEventHandler(source)->Connect<DataChangedEvent>(adapter, slot);
  }

  // all other events
  else
  {
    // a callback with filtering capabilities to subscribe to the model
    auto adapter = [source, widget, method](const event_variant_t& event)
    {
      auto concrete_event = std::get<EventT>(event);
      // only events which are coming from the requested source will be
      if (concrete_event.m_item == source)
      {
        std::invoke(method, *widget, concrete_event);
      }
    };
    GetEventHandler(source)->Connect<EventT>(adapter, slot);
  }
}

//! Sets callback to be notified on item insert. The callback will be called with
//! (SessionItem* parent, tag_index), where 'tag_index' denotes child position after insert.
MVVM_MODEL_EXPORT Connection OnItemInserted(SessionItem* source,
                                            const Callbacks::item_tagindex_t& func,
                                            Slot* slot = nullptr);

MVVM_MODEL_EXPORT Connection OnItemInserted(SessionItem* source, const callback_t& func,
                                            Slot* slot = nullptr);

//! Sets callback to be notified when the item is about to be removed. The callback will be
//! called with (SessionItem* parent, tag_index), where 'tag_index' denotes child position being
//! removed.
MVVM_MODEL_EXPORT Connection OnAboutToRemoveItem(SessionItem* source,
                                                 const Callbacks::item_tagindex_t& func,
                                                 Slot* slot = nullptr);

MVVM_MODEL_EXPORT Connection OnAboutToRemoveItem(SessionItem* source, const callback_t& func,
                                                 Slot* slot = nullptr);

//! Sets callback to be notified on item remove. The callback wil be called with
//! (SessionItem* parent, tag_index), where 'tag_index' denotes child position before the removal.
MVVM_MODEL_EXPORT Connection OnItemRemoved(SessionItem* source,
                                           const Callbacks::item_tagindex_t& func,
                                           Slot* slot = nullptr);

MVVM_MODEL_EXPORT Connection OnItemRemoved(SessionItem* source, const callback_t& func,
                                           Slot* slot = nullptr);

//! Sets callback to be notified on item's data change. The callback will be called
//! with (SessionItem*, data_role).
MVVM_MODEL_EXPORT Connection OnDataChanged(SessionItem* source, const Callbacks::item_int_t& func,
                                           Slot* slot = nullptr);

MVVM_MODEL_EXPORT Connection OnDataChanged(SessionItem* source, const callback_t& func,
                                           Slot* slot = nullptr);

//! Sets callback to be notified on item's property change.
//! The callback will be called with (compound_item, property_name).
MVVM_MODEL_EXPORT Connection OnPropertyChanged(SessionItem* source,
                                               const Callbacks::item_str_t& func,
                                               Slot* slot = nullptr);

//! Sets callback to be notified on item's property change.
//! The callback will be called with (compound_item, property_name).
MVVM_MODEL_EXPORT Connection OnPropertyChanged(SessionItem* source, const callback_t& func,
                                               Slot* slot = nullptr);

}  // namespace mvvm::connect

#endif  // MVVM_SIGNALS_ITEM_CONNECT_UTILS_H_
