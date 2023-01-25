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

#include "mvvm/signals/item_connect_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/signals/model_event_handler.h>

namespace mvvm::connect
{

mvvm::ModelEventHandler *GetEventHandler(const mvvm::SessionItem *item)
{
  if (!item)
  {
    throw NullArgumentException("Error in ItemConnectUtils: uninitialised item");
  }

  if (!item->GetModel())
  {
    throw NullArgumentException("Error in ItemConnectUtils: item doesn't have a model");
  }

  if (auto event_handler = item->GetModel()->GetEventHandler(); event_handler)
  {
    return event_handler;
  }
  throw LogicErrorException("The model doesn't have signaling capabilities");
}

std::optional<PropertyChangedEvent> ConvertToPropertyChangedEvent(SessionItem *source,
                                                                  const event_variant_t &event)
{
  // DataChangedEvent happened with property item can be converted to PropertyChangedEvent of its
  // parent.
  auto concrete_event = std::get<DataChangedEvent>(event);
  if (source == concrete_event.m_item->GetParent())
  {
    return PropertyChangedEvent{source, source->TagIndexOfItem(concrete_event.m_item).tag};
  }
  return {};
}

Connection OnItemInserted(SessionItem *source, const Callbacks::item_tagindex_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<ItemInsertedEvent>(event);
    auto item = concrete_event.m_item;
    auto tag_index = concrete_event.m_tag_index;
    if (item == source)
    {
      func(item, tag_index);  // calling user provided callback
    }
  };

  return event_handler->Connect<ItemInsertedEvent>(filtered_callback, slot);
}

Connection OnItemInserted(SessionItem *source, const callback_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<ItemInsertedEvent>(event);
    if (concrete_event.m_item == source)
    {
      func(concrete_event);  // calling user provided callback
    }
  };

  return event_handler->Connect<ItemInsertedEvent>(filtered_callback, slot);
}

Connection OnAboutToRemoveItem(SessionItem *source, const Callbacks::item_tagindex_t &func,
                               Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<AboutToRemoveItemEvent>(event);
    auto item = concrete_event.m_item;
    auto tag_index = concrete_event.m_tag_index;

    if (item == source)
    {
      func(item, tag_index);  // calling user provided callback
    }
  };

  return event_handler->Connect<AboutToRemoveItemEvent>(filtered_callback, slot);
}

Connection OnAboutToRemoveItem(SessionItem *source, const callback_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<AboutToRemoveItemEvent>(event);
    if (concrete_event.m_item == source)
    {
      func(concrete_event);  // calling user provided callback
    }
  };
  return event_handler->Connect<AboutToRemoveItemEvent>(filtered_callback, slot);
}

Connection OnItemRemoved(SessionItem *source, const Callbacks::item_tagindex_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<ItemRemovedEvent>(event);
    auto item = concrete_event.m_item;
    auto tag_index = concrete_event.m_tag_index;

    if (item == source)
    {
      func(item, tag_index);  // calling user provided callback
    }
  };

  return event_handler->Connect<ItemRemovedEvent>(filtered_callback, slot);
}

Connection OnItemRemoved(SessionItem *source, const callback_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<ItemRemovedEvent>(event);
    if (concrete_event.m_item == source)
    {
      func(concrete_event);  // calling user provided callback
    }
  };

  return event_handler->Connect<ItemRemovedEvent>(filtered_callback, slot);
}

Connection OnDataChanged(SessionItem *source, const Callbacks::item_int_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    auto item = concrete_event.m_item;
    auto role = concrete_event.m_data_role;

    if (item == source)
    {
      func(item, role);  // calling user provided callback
    }
  };

  return event_handler->Connect<DataChangedEvent>(filtered_callback, slot);
}

Connection OnDataChanged(SessionItem *source, const callback_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    if (concrete_event.m_item == source)
    {
      func(concrete_event);  // calling user provided callback
    }
  };
  return event_handler->Connect<DataChangedEvent>(filtered_callback, slot);
}

Connection OnPropertyChanged(SessionItem *source, const Callbacks::item_str_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    auto item = concrete_event.m_item;

    if (utils::GetNestlingDepth(source, item) == 1)
    {
      // calling user provided callback, when property of the source has changed
      func(source, source->TagIndexOfItem(item).tag);
    }
  };

  return event_handler->Connect<DataChangedEvent>(filtered_callback, slot);
}

Connection OnPropertyChanged(SessionItem *source, const callback_t &func, Slot *slot)
{
  auto event_handler = GetEventHandler(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](const event_variant_t &event)
  {
    auto concrete_event = std::get<DataChangedEvent>(event);
    if (utils::GetNestlingDepth(source, concrete_event.m_item) == 1)
    {
      // calling user provided callback, when property of the source has changed
      func(PropertyChangedEvent{source, source->TagIndexOfItem(concrete_event.m_item).tag});
    }
  };
  return event_handler->Connect<DataChangedEvent>(filtered_callback, slot);
}

}  // namespace mvvm::connect
