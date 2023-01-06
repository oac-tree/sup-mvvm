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

#ifndef MVVM_SIGNALS_MODEL_LISTENER_BASE_H_
#define MVVM_SIGNALS_MODEL_LISTENER_BASE_H_

#include <mvvm/signals/model_event_handler.h>

#include <memory>

namespace mvvm
{

class SessionModelInterface;
class ModelEventHandler;

//! Base class for all objects willing to listen for changes in ApplicationModel.

class MVVM_MODEL_EXPORT ModelListenerBase
{
public:
  explicit ModelListenerBase(SessionModelInterface* model);
  virtual ~ModelListenerBase();

  ModelListenerBase& operator=(const ModelListenerBase& other) = delete;
  ModelListenerBase(const ModelListenerBase& other) = delete;

  //! Connect callback to all events specified by the given event type.
  //! The callback is expected to be based on event_variant_t.
  template <typename EventT, typename CallbackT>
  void Connect(const CallbackT& callback)
  {
    GetEventHandler()->Connect<EventT>(callback, GetSlot());
  }

  //! Connect object's method to all events specified by the given event type.
  //! The method is expected to be based on event_variant_t.
  template <typename EventT, typename WidgetT, typename Fn>
  void Connect(WidgetT* widget, const Fn& method)
  {
    GetEventHandler()->Connect<EventT>(widget, method, GetSlot());
  }

  //! Connect object's method to all events specified by the given event type.
  //! The method is expected to be based on concrete event.
  template <typename EventT, typename WidgetT>
  void Connect(WidgetT* widget, void (WidgetT::*method)(const EventT&))
  {
    auto adapter = [widget, method](const event_variant_t& event)
    {
      auto concrete_event = std::get<EventT>(event);
      std::invoke(method, *widget, concrete_event);
    };
    GetEventHandler()->Connect<EventT>(adapter, GetSlot());
  }

protected:
  SessionModelInterface* GetModelBase() const;

private:
  ModelEventHandler* GetEventHandler();
  Slot* GetSlot() const;

  struct ModelListenerBaseImpl;
  std::unique_ptr<ModelListenerBaseImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_LISTENER_BASE_H_
