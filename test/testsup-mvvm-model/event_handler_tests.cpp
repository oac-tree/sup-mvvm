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

#include "mvvm/signals/event_handler.h"

#include <mvvm/model/session_item.h>
#include <mvvm/signals/event_types.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class EventHandlerTests : public ::testing::Test
{
public:
  using mock_callback_listener_t = testing::MockFunction<void(const event_variant_t&)>;

  //! Helper class for gmock to validate overload resolution for a concrete event inside variant.
  class MockSpecializedWidget
  {
  public:
    void OnEvent(const event_variant_t& event) { std::visit(*this, event); }

    void operator()(const DataChangedEvent& event) { OnDataChangedEvent(event); }
    MOCK_METHOD(void, OnDataChangedEvent, (const DataChangedEvent& event));

    void operator()(const PropertyChangedEvent& event) { OnPropertyChangedEvent(event); }
    MOCK_METHOD(void, OnPropertyChangedEvent, (const PropertyChangedEvent& event));

    void operator()(const AboutToInsertItemEvent& event) { OnAboutToInsertItemEvent(event); }
    MOCK_METHOD(void, OnAboutToInsertItemEvent, (const AboutToInsertItemEvent& event));

    void operator()(const ItemInsertedEvent& event) { OnItemInsertedEvent(event); }
    MOCK_METHOD(void, OnItemInsertedEvent, (const ItemInsertedEvent& event));

    void operator()(const AboutToRemoveItemEvent& event) { OnAboutToRemoveItemEvent(event); }
    MOCK_METHOD(void, OnAboutToRemoveItemEvent, (const AboutToRemoveItemEvent& event));

    void operator()(const ItemRemovedEvent& event) { OnItemRemovedEvent(event); }
    MOCK_METHOD(void, OnItemRemovedEvent, (const ItemRemovedEvent& event));

    void operator()(const ModelAboutToBeResetEvent& event) { OnModelAboutToBeResetEvent(event); }
    MOCK_METHOD(void, OnModelAboutToBeResetEvent, (const ModelAboutToBeResetEvent& event));

    void operator()(const ModelResetEvent& event) { OnModelResetEvent(event); }
    MOCK_METHOD(void, OnModelResetEvent, (const ModelResetEvent& event));

    void operator()(const ModelAboutToBeDestroyedEvent& event)
    {
      OnModelAboutToBeDestroyedEvent(event);
    }
    MOCK_METHOD(void, OnModelAboutToBeDestroyedEvent, (const ModelAboutToBeDestroyedEvent& event));
  };
};

//! Connecting to event which was never registered.

TEST_F(EventHandlerTests, ConnectToUnregisteredEvent)
{
  mock_callback_listener_t widget;

  const int role{42};
  SessionItem item;
  const DataChangedEvent data_changed_event{&item, role};

  EventHandler<event_variant_t> event_handler;

  EXPECT_THROW(event_handler.Connect<DataChangedEvent>(widget.AsStdFunction()),
               RuntimeException);
}

//! Connecting single callback with DataChangedEvent. Trigerring notification and checking that
//! callback was called.

TEST_F(EventHandlerTests, EventHandlerConnectViaLambda)
{
  mock_callback_listener_t widget;

  const int role{42};
  SessionItem item;
  const DataChangedEvent data_changed_event{&item, role};

  EventHandler<event_variant_t> event_handler;
  event_handler.Register<DataChangedEvent>();

  event_handler.Connect<DataChangedEvent>(widget.AsStdFunction());

  // check notification when triggering Notify via templated method
  EXPECT_CALL(widget, Call(event_variant_t(data_changed_event))).Times(1);
  event_handler.Notify<DataChangedEvent>(&item, role);

  // check notification when triggering Notify via already constructed event
  EXPECT_CALL(widget, Call(event_variant_t(data_changed_event))).Times(1);
  event_handler.Notify(data_changed_event);
}

//! Connecting MockSpecializedWidget with two events. Validating that the notification
//! is calling a method for concrete event type DataChangedEvent via std::visit mechanism.

TEST_F(EventHandlerTests, EventVariantVisitMachinery)
{
  MockSpecializedWidget widget;

  const int role{42};
  SessionItem item;
  const DataChangedEvent data_changed_event{&item, role};

  EventHandler<event_variant_t> event_handler;
  event_handler.Register<DataChangedEvent>();
  event_handler.Register<ItemInsertedEvent>();

  event_handler.Connect<DataChangedEvent>(&widget, &MockSpecializedWidget::OnEvent);
  event_handler.Connect<ItemInsertedEvent>(&widget, &MockSpecializedWidget::OnEvent);

  EXPECT_CALL(widget, OnDataChangedEvent(data_changed_event)).Times(1);
  EXPECT_CALL(widget, OnAboutToInsertItemEvent(_)).Times(0);
  EXPECT_CALL(widget, OnItemInsertedEvent(_)).Times(0);

  event_handler.Notify<DataChangedEvent>(&item, role);
}
