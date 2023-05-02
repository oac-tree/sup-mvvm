/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/signals/model_event_handler.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_event_listener.h>

#include <variant>

using namespace mvvm;
using ::testing::_;

//! Tests for ModelEventHandlerTests class.

class ModelEventHandlerTests : public ::testing::Test
{
public:
  ModelEventHandlerTests() { m_listener.SubscribeAll(&m_event_handler); }

  ModelEventHandler m_event_handler;
  MockEventListener m_listener;
};

//! Checking listener methods when AboutToInsertItem is fired.

TEST_F(ModelEventHandlerTests, AboutToInsertItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  AboutToInsertItemEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
}

//! Checking listener methods when ItemInserted is fired.

TEST_F(ModelEventHandlerTests, ItemInserted)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  ItemInsertedEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
}

//! Checking listener methods when AboutToRemoveItem is fired.

TEST_F(ModelEventHandlerTests, AboutToRemoveItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  AboutToRemoveItemEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
}

//! Checking listener methods when ItemRemoved is fired.

TEST_F(ModelEventHandlerTests, ItemRemoved)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  ItemRemovedEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

//! Checking listener methods when DataChanged is fired.

TEST_F(ModelEventHandlerTests, DataChanged)
{
  mvvm::SessionItem item;
  int role{42};

  DataChangedEvent event{&item, role};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<DataChangedEvent>(&item, role);
}

//! Checking listener methods when ModelAboutToBeResetEvent is fired.

TEST_F(ModelEventHandlerTests, OnModelAboutToBeReset)
{
  mvvm::SessionModel model;

  ModelAboutToBeResetEvent event{&model};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
}

//! Checking listener methods when ModelResetEvent is fired.

TEST_F(ModelEventHandlerTests, OnModelResetEvent)
{
  mvvm::SessionModel model;

  ModelResetEvent event{&model};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ModelResetEvent>(&model);
}

//! Checking listener methods when ModelAboutToBeDestroyedEvent is fired.

TEST_F(ModelEventHandlerTests, OnModelAboutToBeDestroyed)
{
  mvvm::SessionModel model;

  ModelAboutToBeDestroyedEvent event{&model};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

TEST_F(ModelEventHandlerTests, Unsubscribe)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventHandler event_handler;
  MockEventListener listener;

  listener.SubscribeAll(&event_handler);

  EXPECT_CALL(listener, OnEvent(_)).Times(0);

  listener.Unsubscribe();

  // triggering action
  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  m_event_handler.Notify<DataChangedEvent>(&item, role);
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  m_event_handler.Notify<ModelResetEvent>(&model);
  m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

//! Two listeners are subscribed to different events.
TEST_F(ModelEventHandlerTests, TwoSubscriptions)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventHandler event_handler;
  MockEventListener listener1;
  MockEventListener listener2;

  event_handler.Connect<DataChangedEvent>(&listener1, &MockEventListener::OnEvent,
                                          listener1.m_slot.get());
  event_handler.Connect<ItemRemovedEvent>(&listener2, &MockEventListener::OnEvent,
                                          listener2.m_slot.get());

  DataChangedEvent data_changed_event{&item, role};
  ItemRemovedEvent item_removed_event{&item, tag_index};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener1, OnEvent(event_variant_t(data_changed_event))).Times(1);
    EXPECT_CALL(listener2, OnEvent(event_variant_t(item_removed_event))).Times(1);
  }

  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

TEST_F(ModelEventHandlerTests, UnsubscribeOne)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventHandler event_handler;
  MockEventListener listener1;
  MockEventListener listener2;

  event_handler.Connect<DataChangedEvent>(&listener1, &MockEventListener::OnEvent,
                                          listener1.m_slot.get());
  event_handler.Connect<ItemRemovedEvent>(&listener2, &MockEventListener::OnEvent,
                                          listener2.m_slot.get());

  DataChangedEvent data_changed_event{&item, role};
  ItemRemovedEvent item_removed_event{&item, tag_index};

  EXPECT_CALL(listener1, OnEvent(event_variant_t(data_changed_event))).Times(1);
  EXPECT_CALL(listener2, OnEvent(event_variant_t(item_removed_event))).Times(1);

  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);

  EXPECT_CALL(listener1, OnEvent(_)).Times(0);
  EXPECT_CALL(listener2, OnEvent(event_variant_t(item_removed_event))).Times(1);

  listener1.Unsubscribe();

  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}
