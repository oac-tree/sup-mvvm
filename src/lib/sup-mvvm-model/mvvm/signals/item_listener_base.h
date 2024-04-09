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

#ifndef MVVM_SIGNALS_ITEM_LISTENER_BASE_H_
#define MVVM_SIGNALS_ITEM_LISTENER_BASE_H_

#include <mvvm/model_export.h>
#include <mvvm/signals/item_connect_utils.h>
#include <mvvm/signals/signal_slot.h>

#include <memory>

namespace mvvm
{

/**
 * @brief The ItemListenerBase class is a base for all objects willing to receive various signals
 * on SessionItem change.
 */
class MVVM_MODEL_EXPORT ItemListenerBase
{
public:
  ItemListenerBase();
  virtual ~ItemListenerBase();

  ItemListenerBase& operator=(const ItemListenerBase& other) = delete;
  ItemListenerBase(const ItemListenerBase& other) = delete;

  /**
   * @brief Sets the item for listening.
   *
   * The call of this method will automatically unsubscribe from all notifications of the previous
   * item, and then call the Unsubscribe method to let the derived class do additional bookkeeping.
   * After that, it will call the Subscribe method to let the user subscribe to new signals.
   */
  void SetItem(SessionItem* item);

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
    connect::Connect<EventT>(GetCurrentItem(), callback, GetSlot());
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
    connect::Connect<EventT>(GetCurrentItem(), receiver, method, GetSlot());
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
    connect::Connect<EventT>(GetCurrentItem(), receiver, method, GetSlot());
  }

protected:
  SessionItem* GetCurrentItem() const;

  /**
   * @brief Subscribe to a new item.
   *
   * Will be called on new item set. This method should be overridden in the derived class if the
   * user wants to perform certain actions for the new item. For example, this is the place to
   * subscribe other methods or lambdas to certain event types.
   */
  virtual void Subscribe() {}

  /**
   * @brief Unsubscribe from the previous item.
   *
   * Will be called on new item set. This method should be overridden in the derived class if the
   * user wants to perform additional unsubscription-related activity for the previous item. For
   * example, it can be the cleanup of some view items, or controllers. Please note, that
   * unsubscription from signals of the previous item will be performed automatically.
   */
  virtual void Unsubscribe() {}

private:
  Slot* GetSlot() const;

  struct ItemListenerBaseImpl;
  std::unique_ptr<ItemListenerBaseImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_ITEM_LISTENER_BASE_H_
