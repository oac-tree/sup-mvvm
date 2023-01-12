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

#include "mvvm/viewmodel/abstract_viewmodel_controller.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/signals/model_event_handler.h>

using namespace mvvm;
using ::testing::_;

//! Tests for AbstractViewModelController class.

class AbstractViewModelControllerTests : public ::testing::Test
{
public:
  class TestController : public AbstractViewModelController
  {
  public:
    MOCK_METHOD(void, OnAboutToInsertItem, (const AboutToInsertItemEvent& event), (override));

    MOCK_METHOD(void, OnItemInserted, (const ItemInsertedEvent& event), (override));

    MOCK_METHOD(void, OnAboutToRemoveItem,
                (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

    MOCK_METHOD(void, OnItemRemoved, (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index),
                (override));

    MOCK_METHOD(void, OnDataChanged, (const DataChangedEvent&), (override));

    MOCK_METHOD(void, OnModelAboutToBeReset, (mvvm::SessionModelInterface * model), (override));

    MOCK_METHOD(void, OnModelReset, (mvvm::SessionModelInterface * model), (override));

    MOCK_METHOD(void, OnModelAboutToBeDestroyed, (mvvm::SessionModelInterface * model), (override));
  };

  using mock_controller_t = ::testing::StrictMock<TestController>;
};

//! Controller subscription.

TEST_F(AbstractViewModelControllerTests, SubscribeTo)
{
  mvvm::SessionItem item;
  int role{42};

  mvvm::ModelEventHandler event_handler;

  auto controller = std::make_unique<mock_controller_t>();

  controller->Subscribe(&event_handler);

  DataChangedEvent expected_event{&item, role};
  EXPECT_CALL(*controller, OnDataChanged(expected_event)).Times(1);

  event_handler.Notify<DataChangedEvent>(&item, role);
}

//! Controller unsubscription on deletion.

TEST_F(AbstractViewModelControllerTests, Unsubscribe)
{
  mvvm::SessionItem item;
  int role{42};

  mvvm::ModelEventHandler event_handler;

  auto controller = std::make_unique<mock_controller_t>();

  controller->Subscribe(&event_handler);

  controller.reset();

  ASSERT_NO_FATAL_FAILURE(event_handler.Notify<DataChangedEvent>(&item, role));
}

//! Check the case when EventHandler is destroyed before the controller.

TEST_F(AbstractViewModelControllerTests, DestroyNotifierBefore)
{
  mvvm::SessionItem item;
  int role{42};

  auto event_handler = std::make_unique<mvvm::ModelEventHandler>();

  auto controller = std::make_unique<mock_controller_t>();

  controller->Subscribe(event_handler.get());

  // destroying event_handler
  event_handler.reset();

  ASSERT_NO_FATAL_FAILURE(controller.reset());
}

//! Checking listener methods when AboutToInsertItem is fired.

TEST_F(AbstractViewModelControllerTests, AboutToInsertItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  AboutToInsertItemEvent expected_event{&item, tag_index};
  EXPECT_CALL(controller, OnAboutToInsertItem(expected_event)).Times(1);

  // triggering action
  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
}

//! Checking controller's methods when ItemInserted is fired.

TEST_F(AbstractViewModelControllerTests, ItemInserted)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  ItemInsertedEvent expected_event{&item, tag_index};
  EXPECT_CALL(controller, OnItemInserted(expected_event)).Times(1);

  // triggering action
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
}

//! Checking listener methods when AboutToRemoveItem is fired.

TEST_F(AbstractViewModelControllerTests, AboutToRemoveItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToRemoveItem(&item, tag_index)).Times(1);

  // triggering action
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
}

//! Checking listener methods when ItemRemoved is fired.

TEST_F(AbstractViewModelControllerTests, ItemRemoved)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnItemRemoved(&item, tag_index)).Times(1);

  // triggering action
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

//! Checking listener methods when DataChanged is fired.

TEST_F(AbstractViewModelControllerTests, DataChanged)
{
  mvvm::SessionItem item;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  DataChangedEvent expected_event{&item, role};
  EXPECT_CALL(controller, OnDataChanged(expected_event)).Times(1);

  // triggering action
  event_handler.Notify<DataChangedEvent>(&item, role);
}

TEST_F(AbstractViewModelControllerTests, OnModelAboutToBeReset)
{
  mvvm::SessionModel model;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnModelAboutToBeReset(&model)).Times(1);

  // triggering action
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, OnModelReset)
{
  mvvm::SessionModel model;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnModelReset(&model)).Times(1);

  // triggering action
  event_handler.Notify<ModelResetEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, OnModelAboutToBeDestroyed)
{
  mvvm::SessionModel model;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(1);

  // triggering action
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

// FIXME restore AttemptToEstablishConnectionsTwice
// TEST_F(AbstractViewModelControllerTest, AttemptToEstablishConnectionsTwice)
//{
//  mvvm::ModelEventNotifier notifier;
//  TestController controller;
//  controller.SubscribeTo(&notifier);

//  EXPECT_THROW(controller.SubscribeTo(&notifier), std::runtime_error);
//}

TEST_F(AbstractViewModelControllerTests, UnsubscribeV2)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  mvvm::ModelEventHandler event_handler;
  mock_controller_t controller;
  controller.Subscribe(&event_handler);

  // expecting no signals
  // StricktMock will fail if it s not the case

  // triggering action
  controller.Unsubscribe();

  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  event_handler.Notify<ModelResetEvent>(&model);
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, TwoSubscriptions)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventHandler event_handler;
  mock_controller_t listener1;
  mock_controller_t listener2;

  listener1.Subscribe(&event_handler);
  listener2.Subscribe(&event_handler);

  DataChangedEvent expected_event{&item, role};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener1, OnAboutToInsertItem(_)).Times(1);
    EXPECT_CALL(listener2, OnAboutToInsertItem(_)).Times(1);

    EXPECT_CALL(listener1, OnItemInserted(_)).Times(1);
    EXPECT_CALL(listener2, OnItemInserted(_)).Times(1);

    EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(1);
    EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(1);

    EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(1);
    EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(1);

    EXPECT_CALL(listener1, OnDataChanged(DataChangedEvent{&item, role})).Times(1);
    EXPECT_CALL(listener2, OnDataChanged(DataChangedEvent{&item, role})).Times(1);

    EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(1);
    EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(1);

    EXPECT_CALL(listener1, OnModelReset(_)).Times(1);
    EXPECT_CALL(listener2, OnModelReset(_)).Times(1);

    EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(1);
    EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(1);
  }

  // triggering action
  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  event_handler.Notify<ModelResetEvent>(&model);
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, UnsubscribeOne)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};
  int role{42};

  ModelEventHandler event_handler;
  mock_controller_t listener1;
  mock_controller_t listener2;

  listener1.Subscribe(&event_handler);
  listener2.Subscribe(&event_handler);

  listener1.Unsubscribe();

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener2, OnAboutToInsertItem(_)).Times(1);
    EXPECT_CALL(listener2, OnItemInserted(_)).Times(1);
    EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(1);
    EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(1);
    EXPECT_CALL(listener2, OnDataChanged(DataChangedEvent{&item, role})).Times(1);
    EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(1);
    EXPECT_CALL(listener2, OnModelReset(_)).Times(1);
    EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(1);
  }

  // triggering action
  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  event_handler.Notify<ModelResetEvent>(&model);
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);

  // expecting no signals here
  // StricktMock will fail if it s not the case

  listener2.Unsubscribe();

  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  event_handler.Notify<ModelResetEvent>(&model);
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}
