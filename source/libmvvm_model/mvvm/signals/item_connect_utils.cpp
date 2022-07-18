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

#include "mvvm/signals/item_connect_utils.h"

#include <mvvm/interfaces/model_event_subscriber_interface.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>

namespace
{
mvvm::ModelEventSubscriberInterface *GetSubscriber(const mvvm::SessionItem *item)
{
  if (!item)
  {
    throw std::runtime_error("Error in ItemConnectUtils: uninitialised item");
  }

  if (!item->GetModel())
  {
    throw std::runtime_error("Error in ItemConnectUtils: item doesn't have a model");
  }

  if (auto subscriber = item->GetModel()->GetSubscriber(); subscriber)
  {
    return subscriber;
  }
  throw std::runtime_error(
      "Error in ItemConnectUtils: item's model doesn't have signaling capabilities");
}
}  // namespace

namespace mvvm::connect
{

Connection OnItemInserted(SessionItem *source, const Callbacks::item_tagindex_t &func, Slot *slot)
{
  auto subscriber = GetSubscriber(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](SessionItem *item, const TagIndex &tag_index)
  {
    if (item == source)
    {
      func(item, tag_index);  // calling user provided callback
    }
  };

  return subscriber->SetOnItemInserted(filtered_callback, slot);
}

Connection OnAboutToRemoveItem(SessionItem *source, const Callbacks::item_tagindex_t &func,
                               Slot *slot)
{
  auto subscriber = GetSubscriber(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](SessionItem *item, const TagIndex &tag_index)
  {
    if (item == source)
    {
      func(item, tag_index);  // calling user provided callback
    }
  };

  return subscriber->SetOnAboutToRemoveItem(filtered_callback, slot);
}

Connection OnItemRemoved(SessionItem *source, const Callbacks::item_tagindex_t &func, Slot *slot)
{
  auto subscriber = GetSubscriber(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](SessionItem *item, const TagIndex &tag_index)
  {
    if (item == source)
    {
      func(item, tag_index);  // calling user provided callback
    }
  };

  return subscriber->SetOnItemRemoved(filtered_callback, slot);
}

Connection OnDataChanged(SessionItem *source, const Callbacks::item_int_t &func, Slot *slot)
{
  auto subscriber = GetSubscriber(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](SessionItem *item, int role)
  {
    if (item == source)
    {
      func(item, role);  // calling user provided callback
    }
  };

  return subscriber->SetOnDataChanged(filtered_callback, slot);
}

Connection OnPropertyChanged(SessionItem *source, const Callbacks::item_str_t &func, Slot *slot)
{
  auto subscriber = GetSubscriber(source);

  // Create a callback with filtering capabilities to call user callback only when the event had
  // happened with our source. User callback `func` is passed by copy.
  auto filtered_callback = [func, source](SessionItem *item, int /*role*/)
  {
    if (utils::GetNestlingDepth(source, item) == 1)
    {
      // calling user provided callback, when property of the source has changed
      func(source, source->TagIndexOfItem(item).tag);
    }
  };

  return subscriber->SetOnDataChanged(filtered_callback, slot);
}

}  // namespace mvvm::connect
