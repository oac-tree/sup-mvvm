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

#include "mvvm/experimental/event_handler.h"
#include "mvvm/model/sessionitem.h"

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
    MOCK_METHOD1(OnEvent, void(const experimental::event_t& event));

    //! Returns callback that forwards calls to `OnEvent` method.
    std::function<void(const experimental::event_t& event)> CreateCallback()
    {
      return [this](const auto& event) { OnEvent(event); };
    }
  };

  class MockSpecializedWidget
  {
  public:
    void OnEvent(const experimental::event_t& event) { std::visit(*this, event); }

    void operator()(const experimental::DataChangedEvent& event) { OnDataChangedEvent(event); }
    MOCK_METHOD1(OnDataChangedEvent, void(const experimental::DataChangedEvent& event));

    void operator()(const experimental::AboutToInsertItemEvent& event)
    {
      OnAboutToInsertItemEvent(event);
    }
    MOCK_METHOD1(OnAboutToInsertItemEvent, void(const experimental::AboutToInsertItemEvent& event));

    void operator()(const experimental::ItemInsertedEvent& event) { OnItemInsertedEvent(event); }
    MOCK_METHOD1(OnItemInsertedEvent, void(const experimental::ItemInsertedEvent& event));
  };
};

TEST_F(EventHandlerTests, EventHandlerConnectViaLambda)
{
  const int role{42};
  SessionItem item;
  experimental::DataChangedEvent data_changed_event{role, &item};

  MockWidget widget;

  experimental::EventHandler event_handler;
  event_handler.Connect<experimental::DataChangedEvent>(widget.CreateCallback());

  EXPECT_CALL(widget, OnEvent(experimental::event_t(data_changed_event))).Times(1);

  event_handler.Notify<experimental::DataChangedEvent>(role, &item);
}

TEST_F(EventHandlerTests, EventHandlerConnect)
{
  const int role{42};
  SessionItem item;
  experimental::DataChangedEvent data_changed_event{role, &item};

  MockWidget widget;

  experimental::EventHandler event_handler;
  event_handler.Connect<experimental::DataChangedEvent>(&widget, &MockWidget::OnEvent);

  EXPECT_CALL(widget, OnEvent(experimental::event_t(data_changed_event))).Times(1);
  event_handler.Notify<experimental::DataChangedEvent>(role, &item);
}

TEST_F(EventHandlerTests, SpecializedMethod)
{
  const int role{42};
  SessionItem item;
  experimental::DataChangedEvent data_changed_event{role, &item};

  MockSpecializedWidget widget;

  experimental::EventHandler event_handler;
  event_handler.Connect<experimental::DataChangedEvent>(&widget, &MockSpecializedWidget::OnEvent);

  EXPECT_CALL(widget, OnDataChangedEvent(data_changed_event)).Times(1);
  EXPECT_CALL(widget, OnAboutToInsertItemEvent(_)).Times(0);
  EXPECT_CALL(widget, OnItemInsertedEvent(_)).Times(0);
  event_handler.Notify<experimental::DataChangedEvent>(role, &item);
}
