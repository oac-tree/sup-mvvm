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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/signals/signal_slot.h"

#include <mvvm/model/session_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

class SignalSlotTests : public ::testing::Test
{
public:
  class MockWidget
  {
  public:
    MOCK_METHOD(void, onItemDestroy, (mvvm::SessionItem * item));
    MOCK_METHOD(void, onDataChange, (mvvm::SessionItem * item, int role));
  };

  class SlotWidget : public mvvm::Slot
  {
  public:
    MOCK_METHOD(void, onItemDestroy, (mvvm::SessionItem * item));
    MOCK_METHOD(void, onDataChange, (mvvm::SessionItem * item, int role));
  };
};

TEST_F(SignalSlotTests, MockWidgetConnectAndDisconnect)
{
  MockWidget widget;
  mvvm::Signal<void(mvvm::SessionItem*)> signal;

  auto connection = signal.connect(&widget, &MockWidget::onItemDestroy);

  mvvm::SessionItem item;

  EXPECT_CALL(widget, onItemDestroy(&item)).Times(1);

  signal(&item);  // perform action

  // removing client
  signal.disconnect(connection);
  //  connection.disconnect(); FIXME connection.disconnect() doesn't work

  EXPECT_CALL(widget, onItemDestroy(_)).Times(0);
  signal(&item);  // perform action
}

TEST_F(SignalSlotTests, MockWidgetConnectAndLock)
{
  MockWidget widget;
  mvvm::Signal<void(mvvm::SessionItem*)> signal;

  auto connection = signal.connect(&widget, &MockWidget::onItemDestroy);

  mvvm::SessionItem item;

  EXPECT_CALL(widget, onItemDestroy(&item)).Times(1);

  signal(&item);  // perform action

  // disabling connection
  connection.set_lock(true);

  EXPECT_CALL(widget, onItemDestroy(_)).Times(0);
  signal(&item);  // perform action
}

//! Callback container notifies two widgets. Check if one widget is removed,
//! the second is still notified.

TEST_F(SignalSlotTests, TwoWidgetsNotifiedThenOneRemoved)
{
  MockWidget widget1;
  MockWidget widget2;
  mvvm::Signal<void(mvvm::SessionItem*)> signal;

  auto connection1 = signal.connect([&](auto item) { widget1.onItemDestroy(item); });
  auto connection2 = signal.connect([&](auto item) { widget2.onItemDestroy(item); });

  mvvm::SessionItem item;
  EXPECT_CALL(widget1, onItemDestroy(&item)).Times(1);
  EXPECT_CALL(widget2, onItemDestroy(&item)).Times(1);

  // perform action
  signal(&item);

  // removing one of client
  signal.disconnect(connection1);
  EXPECT_CALL(widget1, onItemDestroy(_)).Times(0);
  EXPECT_CALL(widget2, onItemDestroy(&item)).Times(1);

  // perform action
  signal(&item);
}

//! Callback function with two parameters.

TEST_F(SignalSlotTests, SignalWithTwoParameters)
{
  MockWidget widget1;
  MockWidget widget2;
  mvvm::Signal<void(mvvm::SessionItem*, int)> signal;

  auto connection1 = signal.connect([&](auto item, int role) { widget1.onDataChange(item, role); });
  auto connection2 = signal.connect([&](auto item, int role) { widget2.onDataChange(item, role); });

  int expected_role = 42;
  mvvm::SessionItem item;
  EXPECT_CALL(widget1, onDataChange(&item, expected_role)).Times(1);
  EXPECT_CALL(widget2, onDataChange(&item, expected_role)).Times(1);

  // perform action
  signal(&item, expected_role);

  // removing one of client
  signal.disconnect(connection1);
  EXPECT_CALL(widget1, onDataChange(_, _)).Times(0);
  EXPECT_CALL(widget2, onDataChange(&item, expected_role)).Times(1);

  // perform action
  signal(&item, expected_role);
}

//! Callback function with two parameters.

TEST_F(SignalSlotTests, TwoSignalsOneWidget)
{
  MockWidget widget;
  mvvm::Signal<void(mvvm::SessionItem*)> signal1;
  mvvm::Signal<void(mvvm::SessionItem*, int)> signal2;

  auto connection1 = signal1.connect(&widget, &MockWidget::onItemDestroy);
  auto connection2 = signal2.connect(&widget, &MockWidget::onDataChange);

  int expected_role = 42;
  mvvm::SessionItem item;
  EXPECT_CALL(widget, onItemDestroy(&item)).Times(1);
  EXPECT_CALL(widget, onDataChange(&item, expected_role)).Times(1);

  // perform action
  signal1(&item);
  signal2(&item, expected_role);

  // disconnecting first connection
  signal1.disconnect(connection2);  // should be safe to use connection on different signal
  signal2.disconnect(connection1);  // should be safe to use connection on different signal

  signal1.disconnect(connection1);

  EXPECT_CALL(widget, onDataChange(_, _)).Times(0);
  EXPECT_CALL(widget, onDataChange(&item, expected_role)).Times(1);

  // perform action
  signal1(&item);
  signal2(&item, expected_role);
}

TEST_F(SignalSlotTests, OneSignalTwoWidgetsWithSlotsOneDestroyed)
{
  MockWidget widget1;
  MockWidget widget2;
  mvvm::Signal<void(mvvm::SessionItem*)> signal;

  auto slot1 = std::make_unique<mvvm::Slot>();
  auto slot2 = std::make_unique<mvvm::Slot>();

  signal.connect(&widget1, &MockWidget::onItemDestroy, slot1.get());
  signal.connect(&widget2, &MockWidget::onItemDestroy, slot2.get());

  mvvm::SessionItem item;

  EXPECT_CALL(widget1, onItemDestroy(&item)).Times(1);
  EXPECT_CALL(widget2, onItemDestroy(&item)).Times(1);

  signal(&item);  // perform action

  // destroying slot
  slot1.reset();

  // only one signal expected
  EXPECT_CALL(widget1, onItemDestroy(&item)).Times(0);
  EXPECT_CALL(widget2, onItemDestroy(&item)).Times(1);

  signal(&item);  // perform action
}

TEST_F(SignalSlotTests, TwoMockOwnersAndTwoSlots)
{
  auto widget1 = std::make_unique<SlotWidget>();
  auto widget2 = std::make_unique<SlotWidget>();
  mvvm::Signal<void(mvvm::SessionItem*)> signal;

  signal.connect(widget1.get(), &SlotWidget::onItemDestroy, widget1.get());
  signal.connect(widget2.get(), &SlotWidget::onItemDestroy, widget2.get());

  mvvm::SessionItem item;

  EXPECT_CALL(*widget1, onItemDestroy(&item)).Times(1);
  EXPECT_CALL(*widget2, onItemDestroy(&item)).Times(1);

  signal(&item);  // perform action

  // destroying widget2
  widget2.reset();

  // only one signal expected
  EXPECT_CALL(*widget1, onItemDestroy(&item)).Times(1);

  signal(&item);  // perform action
}

//! Lambda subscribes another lambda to the same signal.
//! This checks that there is no dead lock.

TEST_F(SignalSlotTests, SubscribeLambdaRecursive)
{
  mvvm::Signal<void(int num)> signal;

  int x{10};
  int y{20};

  auto func1 = [&x](int num) { x = x + num; };

  auto func2 = [&y, func1, &signal](int num)
  {
    y = y + num;
    signal.connect(func1);
  };

  signal.connect(func2);

  signal(1);
  EXPECT_EQ(x, 10);  // no call, same number as before
  EXPECT_EQ(y, 21);  // was called

  signal(1);
  EXPECT_TRUE(x >= 11);  // was called more than once
  EXPECT_EQ(y, 22);      // was called
}
