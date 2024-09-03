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

#ifndef MVVM_SIGNALS_ITEM_LISTENER_H_
#define MVVM_SIGNALS_ITEM_LISTENER_H_

#include <mvvm/signals/item_listener_base.h>

namespace mvvm
{

class SessionItem;

/**
 * @brief The ItemListener class is a templated class to listen for changes in concrete SessionItem.
 *
 * It can be used as a base class, or as a standalone listener. Objects connect to an item via the
 * listener with their callbacks. The lifetime of connected objects should exceed the lifetime
 * of this listener. The callbacks will be called on the item change as long as the listener is
 * alive.
 *
 * When used as a base class, the user can override Subscribe Unsubscribe methods to provide
 * automatic connections and additional bookkeeping on new item changes. In this case, the listener
 * can play the role of the single controller, capable of working with multiple items (one item at a
 * time).
 *
 * @tparam ItemT Type of the concrete SessionItem to listen.
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
 * VectorItem item;
 *
 * ItemListener<VectorItem> listener;
 * listener.SetItem(&item);
 *
 * listener.Connect<DataChangedEvent>(&widget, &Widget::OnAnyEvent);
 * listener.Connect<ItemInsertedEvent>(&widget, &Widget::OnAnyEvent);
 * listener.Connect<ItemInsertedEvent>(&widget, &Widget::OnItemInsertedEvent);
 * @endcode
 */

template <typename ItemT>
class ItemController : public ItemListenerBase
{
public:
  /**
   * @brief Default c-tor when no item is set for listening yet.
   */
  ItemController() = default;

  /**
   * @brief c-tor that starts listening item from the beginning.
   */
  explicit ItemController(ItemT* item)
  {
    SetItem(item);
  }

  /**
   * @brief Returns current item which was set for listening.
   */
  ItemT* GetItem() const { return static_cast<ItemT*>(GetCurrentItem()); }
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEM_LISTENER_H_
