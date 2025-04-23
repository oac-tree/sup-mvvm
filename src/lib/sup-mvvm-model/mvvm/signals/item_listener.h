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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SIGNALS_ITEM_LISTENER_H_
#define MVVM_SIGNALS_ITEM_LISTENER_H_

#include <mvvm/model_export.h>
#include <mvvm/signals/item_connect.h>
#include <mvvm/signals/signal_slot.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The ItemListener class allows to subscribe to different event notifications related to
 * given item.
 *
 * All subscribers should outlive ItemListener. On ItemListener's desrtuctions all connections will
 * be terminated.
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
 * VectorItem vector_item;
 *
 * ItemListener<VectorItem> listener(&vector_item);
 *
 * // connect single method accepting event variant to two different events
 * listener.Connect<DataChangedEvent>(&widget, &Widget::OnAnyEvent);
 * listener.Connect<ItemInsertedEvent>(&widget, &Widget::OnAnyEvent);
 *
 * // connect method with concrete event signature
 * listener.Connect<ItemInsertedEvent>(&widget, &Widget::OnItemInsertedEvent);
 *
 * // lambda to event variant
 * auto on_event1 = [](const event_variant_t& event_variant) { ...};
 * listener.Connect<ItemInsertedEvent>(on_event1);
 *
 * // lambda to concrete event
 * auto on_event2 = [](const ItemInsertedEvent& event) { ...};
 * listener.Connect<ItemInsertedEvent>(on_event2);
 *
 * @endcode
 */
class MVVM_MODEL_EXPORT ItemListener
{
public:
  explicit ItemListener(SessionItem* item);
  virtual ~ItemListener();

  ItemListener(const ItemListener&) = delete;
  ItemListener& operator=(const ItemListener&) = delete;
  ItemListener(ItemListener&&) = delete;
  ItemListener& operator=(ItemListener&&) = delete;

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
    connect::Connect<EventT>(GetItem(), callback, GetSlot());
  }

  /**
   * @brief Connects a callback to events specified by the given event type.
   *
   * This version is intended for user callbacks accepting concrete event.
   *
   * @tparam EventT The type of the event to subscribe.
   * @param callback A callback based on EventT.
   */
  template <typename EventT>
  void Connect(const std::function<void(const EventT&)>& callback)
  {
    connect::Connect<EventT>(GetItem(), callback, GetSlot());
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
    connect::Connect<EventT>(GetItem(), receiver, method, GetSlot());
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
    connect::Connect<EventT>(GetItem(), receiver, method, GetSlot());
  }

  /**
   * @brief Returns current served item.
   */
  SessionItem* GetItem() const;

private:
  Slot* GetSlot() const;

  SessionItem* m_item{nullptr};
  std::unique_ptr<Slot> m_slot;  //!< slot used to define time-of-life of all connections
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEM_LISTENER_H_
