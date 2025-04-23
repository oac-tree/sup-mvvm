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

#include "mvvm/signals/model_event_handler.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/mvvm_types.h>
#include <mvvm/model/property_item.h>

#include <benchmark/benchmark.h>

using namespace mvvm;

//! Testing performance of ModelEventHandler.

class ModelEventHandlerBenchmark : public benchmark::Fixture
{
public:
  class TestListener
  {
  public:
    void OnEvent(const mvvm::event_variant_t &event) {}
  };
};

//! Single notification when nothing is connected.

BENCHMARK_F(ModelEventHandlerBenchmark, SingleEventNoConnections)(benchmark::State &state)
{
  ModelEventHandler m_event_handler;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  int value{0};
  for (auto dummy : state)
  {
    m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  }
}

//! Single notification with single listener.

BENCHMARK_F(ModelEventHandlerBenchmark, SingleEventOneListener)(benchmark::State &state)
{
  ModelEventHandler m_event_handler;
  TestListener listener;

  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  m_event_handler.Connect<AboutToInsertItemEvent>(&listener, &TestListener::OnEvent);

  int value{0};
  for (auto dummy : state)
  {
    m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  }
}

//! Single notification with single listener. Event is constructed upfront.

BENCHMARK_F(ModelEventHandlerBenchmark, SingleEventOneListenerV2)(benchmark::State &state)
{
  ModelEventHandler m_event_handler;
  TestListener listener;

  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  AboutToInsertItemEvent event{&item, tag_index};

  m_event_handler.Connect<AboutToInsertItemEvent>(&listener, &TestListener::OnEvent);

  int value{0};
  for (auto dummy : state)
  {
    m_event_handler.Notify(event);
  }
}

//! Two notifications when nothing is connected.

BENCHMARK_F(ModelEventHandlerBenchmark, TwoEventsNoListeners)(benchmark::State &state)
{
  ModelEventHandler m_event_handler;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  int value{0};
  for (auto dummy : state)
  {
    m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
    m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  }
}

//! Two notifications with two listeners.

BENCHMARK_F(ModelEventHandlerBenchmark, TwoEventsTwoListeners)(benchmark::State &state)
{
  ModelEventHandler m_event_handler;
  TestListener listener;

  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  AboutToInsertItemEvent event0{&item, tag_index};
  ItemInsertedEvent event1{&item, tag_index};

  m_event_handler.Connect<AboutToInsertItemEvent>(&listener, &TestListener::OnEvent);
  m_event_handler.Connect<ItemInsertedEvent>(&listener, &TestListener::OnEvent);

  int value{0};
  for (auto dummy : state)
  {
    m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
    m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  }
}
