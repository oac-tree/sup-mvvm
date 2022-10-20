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

#include "mvvm/model/sessionitem.h"
#include "mvvm/signals/event_handler.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <variant>

using namespace mvvm;
using ::testing::_;

class EventHandlerTests : public ::testing::Test
{
public:
  //! Helper class for gmock to check lambda-based callbacks.
  class MockWidget
  {
  public:
    MOCK_METHOD1(OnEvent, void(const event_t& event));

    //! Returns callback that forwards calls to `OnEvent` method.
    std::function<void(const event_t& event)> CreateCallback()
    {
      return [this](const auto& event) { OnEvent(event); };
    }
  };

  //! Helper class for gmock to validate overload resolution for a concrete event inside variant.
  class MockSpecializedWidget
  {
  public:
    void OnEvent(const event_t& event) { std::visit(*this, event); }

    void operator()(const DataChangedEvent& event) { OnDataChangedEvent(event); }
    MOCK_METHOD1(OnDataChangedEvent, void(const DataChangedEvent& event));

    void operator()(const AboutToInsertItemEvent& event) { OnAboutToInsertItemEvent(event); }
    MOCK_METHOD1(OnAboutToInsertItemEvent, void(const AboutToInsertItemEvent& event));

    void operator()(const ItemInsertedEvent& event) { OnItemInsertedEvent(event); }
    MOCK_METHOD1(OnItemInsertedEvent, void(const ItemInsertedEvent& event));

    void operator()(const AboutToRemoveItemEvent& event) { OnAboutToRemoveItemEvent(event); }
    MOCK_METHOD1(OnAboutToRemoveItemEvent, void(const AboutToRemoveItemEvent& event));

    void operator()(const ItemRemovedEvent& event) { OnItemRemovedEvent(event); }
    MOCK_METHOD1(OnItemRemovedEvent, void(const ItemRemovedEvent& event));

    void operator()(const ModelAboutToBeResetEvent& event) { OnModelAboutToBeResetEvent(event); }
    MOCK_METHOD1(OnModelAboutToBeResetEvent, void(const ModelAboutToBeResetEvent& event));

    void operator()(const ModelResetEvent& event) { OnModelResetEvent(event); }
    MOCK_METHOD1(OnModelResetEvent, void(const ModelResetEvent& event));
  };
};

//! Connecting single callback with DataChangedEvent. Trigerring notification and checking that
//! callback was called.

TEST_F(EventHandlerTests, EventHandlerConnectViaLambda)
{
  const int role{42};
  SessionItem item;
  DataChangedEvent data_changed_event{role, &item};

  MockWidget widget;

  EventHandler event_handler;
  event_handler.Connect<DataChangedEvent>(widget.CreateCallback());

  // check notification when triggering Notify via templated method
  EXPECT_CALL(widget, OnEvent(event_t(data_changed_event))).Times(1);
  event_handler.Notify<DataChangedEvent>(role, &item);

  // check notification when triggering Notify via already constructed event
  EXPECT_CALL(widget, OnEvent(event_t(data_changed_event))).Times(1);
  event_handler.Notify(data_changed_event);
}

//! Same as above, connection is established via object and method's pointer.

TEST_F(EventHandlerTests, EventHandlerConnectViaObjectMethod)
{
  const int role{42};
  SessionItem item;
  DataChangedEvent data_changed_event{role, &item};

  MockWidget widget;

  EventHandler event_handler;
  event_handler.Connect<DataChangedEvent>(&widget, &MockWidget::OnEvent);

  EXPECT_CALL(widget, OnEvent(event_t(data_changed_event))).Times(1);
  event_handler.Notify<DataChangedEvent>(role, &item);
}

//! Connecting MockSpecializedWidget with two events. Validating that the notification
//! is calling a method for concrete event type DataChangedEvent.

TEST_F(EventHandlerTests, EventVariantVisitMachinery)
{
  const int role{42};
  SessionItem item;
  DataChangedEvent data_changed_event{role, &item};

  MockSpecializedWidget widget;

  EventHandler event_handler;
  event_handler.Connect<DataChangedEvent>(&widget, &MockSpecializedWidget::OnEvent);
  event_handler.Connect<ItemInsertedEvent>(&widget, &MockSpecializedWidget::OnEvent);

  EXPECT_CALL(widget, OnDataChangedEvent(data_changed_event)).Times(1);
  EXPECT_CALL(widget, OnAboutToInsertItemEvent(_)).Times(0);
  EXPECT_CALL(widget, OnItemInsertedEvent(_)).Times(0);

  event_handler.Notify<DataChangedEvent>(role, &item);
}
