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

#ifndef MVVM_EXPERIMENTAL_EVENT_HANDLER_H_
#define MVVM_EXPERIMENTAL_EVENT_HANDLER_H_

#include <mvvm/core/type_map.h>
#include <mvvm/experimental/event_notifier_interface.h>
#include <mvvm/experimental/event_subscriber_interface.h>
#include <mvvm/experimental/event_types.h>
#include <mvvm/signals/signal_slot.h>

#include <functional>
#include <memory>

namespace mvvm::experimental
{

class EventHandler
{
  using callback_t = std::function<void(const event_t&)>;
  using signal_t = Signal<void(const event_t&)>;

public:
  EventHandler();

  //  void Notify(const event_t& event);

  template <typename T, typename... Args>
  void Notify(Args&&... args)
  {
    auto it = m_signals.find<T>();
    T event(std::forward<Args>(args)...);
    it->second->operator()(event);
    //    Notify(T(std::forward<Args>(args)...));
  }

  template <typename T>
  Connection Connect(const callback_t& callback, Slot* slot = nullptr)
  {
    auto it = m_signals.find<T>();
    return it->second->connect(callback, slot);
  }

  template <typename T, typename U, typename Fn>
  Connection Connect(U* p, const Fn& fn, Slot* slot = nullptr)
  {
    auto it = m_signals.find<T>();
    return it->second->connect(p, fn, slot);
  }

private:
  TypeMap<std::unique_ptr<signal_t>> m_signals;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_EVENT_NOTIFIER_INTERFACE_H_
