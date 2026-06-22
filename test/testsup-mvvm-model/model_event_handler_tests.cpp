/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/test/mock_event_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for ModelEventHandlerTests class.
 */
class ModelEventHandlerTests : public ::testing::Test
{
public:
  ModelEventHandlerTests() { m_listener.SubscribeAll(&m_event_handler); }

  ModelEventHandler m_event_handler;
  mvvm::test::MockEventListener m_listener;
};

TEST_F(ModelEventHandlerTests, AboutToInsertItem)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  AboutToInsertItemEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
}

TEST_F(ModelEventHandlerTests, ItemInserted)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  ItemInsertedEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
}

TEST_F(ModelEventHandlerTests, AboutToRemoveItem)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  AboutToRemoveItemEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
}

TEST_F(ModelEventHandlerTests, ItemRemoved)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  ItemRemovedEvent event{&item, tag_index};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

TEST_F(ModelEventHandlerTests, ItemDestroyed)
{
  mvvm::SessionItem item;

  ItemDestroyedEvent event{&item};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ItemDestroyedEvent>(&item);
}

TEST_F(ModelEventHandlerTests, DataChanged)
{
  mvvm::SessionItem item;
  const role_t role{42};

  DataChangedEvent event{&item, role};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<DataChangedEvent>(&item, role);
}

TEST_F(ModelEventHandlerTests, OnModelAboutToBeReset)
{
  mvvm::SessionModel model;

  ModelAboutToBeResetEvent event{&model};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
}

TEST_F(ModelEventHandlerTests, OnModelResetEvent)
{
  mvvm::SessionModel model;

  ModelResetEvent event{&model};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(event))).Times(1);

  // triggering action
  m_event_handler.Notify<ModelResetEvent>(&model);
}

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
  const role_t role{42};

  ModelEventHandler event_handler;
  mvvm::test::MockEventListener listener;

  listener.SubscribeAll(&event_handler);

  EXPECT_CALL(listener, OnEvent(::testing::_)).Times(0);

  listener.Unsubscribe();

  // triggering action
  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  event_handler.Notify<ItemDestroyedEvent>(&item);
  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  event_handler.Notify<ModelResetEvent>(&model);
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

TEST_F(ModelEventHandlerTests, TwoSubscriptions)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  const role_t role{42};

  ModelEventHandler event_handler;
  mvvm::test::MockEventListener listener1;
  mvvm::test::MockEventListener listener2;

  event_handler.Connect<DataChangedEvent>(&listener1, &mvvm::test::MockEventListener::OnEvent,
                                          listener1.m_slot.get());
  event_handler.Connect<ItemRemovedEvent>(&listener2, &mvvm::test::MockEventListener::OnEvent,
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
  const role_t role{42};

  ModelEventHandler event_handler;
  mvvm::test::MockEventListener listener1;
  mvvm::test::MockEventListener listener2;

  event_handler.Connect<DataChangedEvent>(&listener1, &mvvm::test::MockEventListener::OnEvent,
                                          listener1.m_slot.get());
  event_handler.Connect<ItemRemovedEvent>(&listener2, &mvvm::test::MockEventListener::OnEvent,
                                          listener2.m_slot.get());

  DataChangedEvent data_changed_event{&item, role};
  ItemRemovedEvent item_removed_event{&item, tag_index};

  EXPECT_CALL(listener1, OnEvent(event_variant_t(data_changed_event))).Times(1);
  EXPECT_CALL(listener2, OnEvent(event_variant_t(item_removed_event))).Times(1);

  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);

  EXPECT_CALL(listener1, OnEvent(::testing::_)).Times(0);
  EXPECT_CALL(listener2, OnEvent(event_variant_t(item_removed_event))).Times(1);

  listener1.Unsubscribe();

  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

}  // namespace mvvm::test
