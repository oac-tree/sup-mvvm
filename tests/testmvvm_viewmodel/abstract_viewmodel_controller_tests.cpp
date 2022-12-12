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
    MOCK_METHOD(void, OnAboutToInsertItem,
                (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

    MOCK_METHOD(void, OnItemInserted, (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index),
                (override));

    MOCK_METHOD(void, OnAboutToRemoveItem,
                (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index), (override));

    MOCK_METHOD(void, OnItemRemoved, (mvvm::SessionItem * parent, const mvvm::TagIndex& tag_index),
                (override));

    MOCK_METHOD(void, OnDataChanged, (mvvm::SessionItem * item, int role), (override));

    MOCK_METHOD(void, OnModelAboutToBeReset, (mvvm::SessionModelInterface * model), (override));

    MOCK_METHOD(void, OnModelReset, (mvvm::SessionModelInterface * model), (override));

    MOCK_METHOD(void, OnModelAboutToBeDestroyed, (mvvm::SessionModelInterface * model), (override));
  };
};

//! Controller subscription.

TEST_F(AbstractViewModelControllerTests, SubscribeTo)
{
  mvvm::SessionItem item;
  int role{42};

  mvvm::ModelEventHandler event_handler;

  auto controller = std::make_unique<TestController>();

  controller->Subscribe(&event_handler);

  EXPECT_CALL(*controller, OnDataChanged(&item, role)).Times(1);

  event_handler.Notify<DataChangedEvent>(&item, role);
}

//! Controller unsubscription on deletion.

TEST_F(AbstractViewModelControllerTests, Unsubscribe)
{
  mvvm::SessionItem item;
  int role{42};

  mvvm::ModelEventHandler event_handler;

  auto controller = std::make_unique<TestController>();

  controller->Subscribe(&event_handler);

  controller.reset();

  ASSERT_NO_FATAL_FAILURE(event_handler.Notify<DataChangedEvent>(&item, role));
}

//! Check the case when EventNotifier is destroyed before the listener

TEST_F(AbstractViewModelControllerTests, DestroyNotifierBefore)
{
  mvvm::SessionItem item;
  int role{42};

  auto event_handler = std::make_unique<mvvm::ModelEventHandler>();

  auto controller = std::make_unique<TestController>();

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
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(&item, tag_index)).Times(1);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
}

//! Checking listener methods when ItemInserted is fired.

TEST_F(AbstractViewModelControllerTests, ItemInserted)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(&item, tag_index)).Times(1);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
}

//! Checking listener methods when AboutToRemoveItem is fired.

TEST_F(AbstractViewModelControllerTests, AboutToRemoveItem)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(&item, tag_index)).Times(1);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
}

//! Checking listener methods when ItemRemoved is fired.

TEST_F(AbstractViewModelControllerTests, ItemRemoved)
{
  mvvm::SessionItem item;
  mvvm::TagIndex tag_index{"tag", 0};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(&item, tag_index)).Times(1);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

//! Checking listener methods when DataChanged is fired.

TEST_F(AbstractViewModelControllerTests, DataChanged)
{
  mvvm::SessionItem item;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<DataChangedEvent>(&item, role);
}

TEST_F(AbstractViewModelControllerTests, OnModelAboutToBeReset)
{
  mvvm::SessionModel model;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(&model)).Times(1);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, OnModelReset)
{
  mvvm::SessionModel model;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(&model)).Times(1);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

  // triggering action
  event_handler.Notify<ModelResetEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, OnModelAboutToBeDestroyed)
{
  mvvm::SessionModel model;
  int role{42};

  mvvm::ModelEventHandler event_handler;
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(&model)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
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
  TestController controller;
  controller.Subscribe(&event_handler);

  EXPECT_CALL(controller, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(controller, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(controller, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(controller, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelReset(_)).Times(0);
  EXPECT_CALL(controller, OnModelAboutToBeDestroyed(_)).Times(0);

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
  TestController listener1;
  TestController listener2;

  listener1.Subscribe(&event_handler);
  listener2.Subscribe(&event_handler);

  EXPECT_CALL(listener1, OnAboutToInsertItem(_, _)).Times(1);
  EXPECT_CALL(listener1, OnItemInserted(_, _)).Times(1);
  EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(1);
  EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(1);
  EXPECT_CALL(listener1, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(1);
  EXPECT_CALL(listener1, OnModelReset(_)).Times(1);
  EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(1);

  EXPECT_CALL(listener2, OnAboutToInsertItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemInserted(_, _)).Times(1);
  EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(1);
  EXPECT_CALL(listener2, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(1);

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
  TestController listener1;
  TestController listener2;

  listener1.Subscribe(&event_handler);
  listener2.Subscribe(&event_handler);

  listener1.Unsubscribe();

  EXPECT_CALL(listener1, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener1, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(0);

  EXPECT_CALL(listener2, OnAboutToInsertItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemInserted(_, _)).Times(1);
  EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(1);
  EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(1);
  EXPECT_CALL(listener2, OnDataChanged(&item, role)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelReset(_)).Times(1);
  EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(1);

  // triggering action
  event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  event_handler.Notify<DataChangedEvent>(&item, role);
  event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  event_handler.Notify<ModelResetEvent>(&model);
  event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);

  EXPECT_CALL(listener1, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener1, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener1, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener1, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener1, OnModelAboutToBeDestroyed(_)).Times(0);

  EXPECT_CALL(listener2, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener2, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener2, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener2, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener2, OnDataChanged(&item, role)).Times(0);
  EXPECT_CALL(listener2, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener2, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener2, OnModelAboutToBeDestroyed(_)).Times(0);

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
