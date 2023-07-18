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

#include "mvvm/viewmodel/abstract_viewmodel_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/signals/model_event_handler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Tests for AbstractViewModelController class.

class AbstractViewModelControllerTests : public ::testing::Test
{
public:
  AbstractViewModelControllerTests() : m_model(&m_event_handler) {}

  class TestModel : public SessionModel
  {
  public:
    explicit TestModel(ModelEventHandler* event_handler) : m_event_handler(event_handler) {}

    ModelEventHandler* GetEventHandler() const override { return m_event_handler; }

    mvvm::ModelEventHandler* m_event_handler;
  };

  /**
   * @brief The NotifyingTestController is a mocking class to controll notifications.
   */
  class NotifyingTestController : public AbstractViewModelController
  {
  public:
    MOCK_METHOD(void, OnModelEvent, (const AboutToInsertItemEvent& event), (override));

    MOCK_METHOD(void, OnModelEvent, (const ItemInsertedEvent& event), (override));

    MOCK_METHOD(void, OnModelEvent, (const AboutToRemoveItemEvent& event), (override));

    MOCK_METHOD(void, OnModelEvent, (const ItemRemovedEvent& event), (override));

    MOCK_METHOD(void, OnModelEvent, (const DataChangedEvent&), (override));

    MOCK_METHOD(void, OnModelEvent, (const ModelAboutToBeResetEvent& event), (override));

    MOCK_METHOD(void, OnModelEvent, (const ModelResetEvent& event), (override));

    MOCK_METHOD(void, OnModelEvent, (const ModelAboutToBeDestroyedEvent& event), (override));

    const SessionItem* GetRootItem() const override { return m_root_item; };
    void SetRootItemImpl(SessionItem* root_item) override
    {
      m_root_item = root_item;
      OnSetRootItemImpl(root_item);
    };

    void SubscribeImpl(SessionModelInterface* model) override
    {
      SubscribeAll(model);
      OnSubscribeImpl(model);
    }

    MOCK_METHOD(void, UnsubscribeImpl, (), ());

    MOCK_METHOD(void, OnSubscribeImpl, (SessionModelInterface* root_item), ());
    MOCK_METHOD(void, OnSetRootItemImpl, (SessionItem * item), ());

    SessionItem* m_root_item{nullptr};
  };

  using mock_controller_t = ::testing::StrictMock<NotifyingTestController>;
  TestModel m_model;
  ModelEventHandler m_event_handler;
};

//! Setting the model, checking calls of SetRootItemImpl

TEST_F(AbstractViewModelControllerTests, SetModel)
{
  ApplicationModel model;
  mock_controller_t controller;

  EXPECT_EQ(controller.GetModel(), nullptr);
  EXPECT_EQ(controller.GetRootItem(), nullptr);

  EXPECT_CALL(controller, OnSubscribeImpl(&model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(model.GetRootItem())).Times(1);
  controller.SetModel(&model);

  EXPECT_EQ(controller.GetModel(), &model);
  EXPECT_EQ(controller.GetRootItem(), model.GetRootItem());

  // setting same model shouldn't trigger a call
  EXPECT_CALL(controller, OnSubscribeImpl(_)).Times(0);
  EXPECT_CALL(controller, OnSetRootItemImpl(_)).Times(0);
  controller.SetModel(&model);

  EXPECT_EQ(controller.GetModel(), &model);
  EXPECT_EQ(controller.GetRootItem(), model.GetRootItem());

  // setting nullptr as a model
  EXPECT_CALL(controller, OnSetRootItemImpl(nullptr)).Times(1);
  EXPECT_CALL(controller, UnsubscribeImpl()).Times(1);
  controller.SetModel(nullptr);

  EXPECT_EQ(controller.GetModel(), nullptr);
  EXPECT_EQ(controller.GetRootItem(), nullptr);
}

//! Setting root item, checking calls of SetRootItemImpl.

TEST_F(AbstractViewModelControllerTests, SetRootItem)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  mock_controller_t controller;

  EXPECT_EQ(controller.GetModel(), nullptr);
  EXPECT_EQ(controller.GetRootItem(), nullptr);

  EXPECT_CALL(controller, OnSubscribeImpl(&model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(model.GetRootItem())).Times(1);
  controller.SetRootItem(model.GetRootItem());

  EXPECT_EQ(controller.GetModel(), &model);
  EXPECT_EQ(controller.GetRootItem(), model.GetRootItem());

  // setting same item shouldn't trigger a call
  EXPECT_CALL(controller, OnSetRootItemImpl(_)).Times(0);
  controller.SetRootItem(model.GetRootItem());

  EXPECT_EQ(controller.GetModel(), &model);
  EXPECT_EQ(controller.GetRootItem(), model.GetRootItem());

  // setting nullptr as a root item
  EXPECT_CALL(controller, OnSetRootItemImpl(nullptr)).Times(1);
  EXPECT_CALL(controller, UnsubscribeImpl()).Times(1);
  controller.SetRootItem(nullptr);

  EXPECT_EQ(controller.GetModel(), nullptr);
  EXPECT_EQ(controller.GetRootItem(), nullptr);
}

//! Controller subscription.

TEST_F(AbstractViewModelControllerTests, SubscribeTo)
{
  mvvm::SessionItem item;
  const int role{42};

  auto controller = std::make_unique<mock_controller_t>();

  EXPECT_EQ(controller->GetModel(), nullptr);

  EXPECT_CALL(*controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(*controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller->SetModel(&m_model);

  EXPECT_EQ(controller->GetModel(), &m_model);

  EXPECT_CALL(*controller, OnModelEvent(DataChangedEvent{&item, role})).Times(1);

  m_event_handler.Notify<DataChangedEvent>(&item, role);
}

//! Controller unsubscription on deletion.

TEST_F(AbstractViewModelControllerTests, Unsubscribe)
{
  mvvm::SessionItem item;
  const int role{42};

  mvvm::ModelEventHandler event_handler;

  auto controller = std::make_unique<mock_controller_t>();

  EXPECT_CALL(*controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(*controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller->SetModel(&m_model);

  controller.reset();

  ASSERT_NO_FATAL_FAILURE(m_event_handler.Notify<DataChangedEvent>(&item, role));
}

//! Check the case when EventHandler is destroyed before the controller.

TEST_F(AbstractViewModelControllerTests, DestroyNotifierBefore)
{
  mvvm::SessionItem item;
  const int role{42};

  auto event_handler = std::make_unique<mvvm::ModelEventHandler>();
  TestModel model(event_handler.get());

  auto controller = std::make_unique<mock_controller_t>();

  EXPECT_CALL(*controller, OnSubscribeImpl(&model)).Times(1);
  EXPECT_CALL(*controller, OnSetRootItemImpl(model.GetRootItem())).Times(1);

  controller->SetModel(&model);

  // destroying event_handler
  event_handler.reset();

  ASSERT_NO_FATAL_FAILURE(controller.reset());
}

//! Checking listener methods when AboutToInsertItem is fired.

TEST_F(AbstractViewModelControllerTests, AboutToInsertItem)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(AboutToInsertItemEvent{&item, tag_index})).Times(1);

  // triggering action
  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
}

//! Checking controller's methods when ItemInserted is fired.

TEST_F(AbstractViewModelControllerTests, ItemInserted)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(ItemInsertedEvent{&item, tag_index})).Times(1);

  // triggering action
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
}

//! Checking listener methods when AboutToRemoveItem is fired.

TEST_F(AbstractViewModelControllerTests, AboutToRemoveItem)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(AboutToRemoveItemEvent{&item, tag_index})).Times(1);

  // triggering action
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
}

//! Checking listener methods when ItemRemoved is fired.

TEST_F(AbstractViewModelControllerTests, ItemRemoved)
{
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(ItemRemovedEvent{&item, tag_index})).Times(1);

  // triggering action
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
}

//! Checking listener methods when DataChanged is fired.

TEST_F(AbstractViewModelControllerTests, DataChanged)
{
  mvvm::SessionItem item;
  const int role{42};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(DataChangedEvent{&item, role})).Times(1);

  // triggering action
  m_event_handler.Notify<DataChangedEvent>(&item, role);
}

TEST_F(AbstractViewModelControllerTests, OnModelAboutToBeReset)
{
  mvvm::SessionModel model;
  const int role{42};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(ModelAboutToBeResetEvent{&model})).Times(1);

  // triggering action
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, OnModelReset)
{
  mvvm::SessionModel model;
  const int role{42};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(ModelResetEvent{&model})).Times(1);

  // triggering action
  m_event_handler.Notify<ModelResetEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, OnModelAboutToBeDestroyed)
{
  mvvm::SessionModel model;
  const int role{42};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, OnModelEvent(ModelAboutToBeDestroyedEvent{&model})).Times(1);

  // triggering action
  m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, UnsubscribeV2)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};
  const int role{42};

  mock_controller_t controller;

  EXPECT_CALL(controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller.SetModel(&m_model);

  EXPECT_CALL(controller, UnsubscribeImpl()).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(nullptr)).Times(1);

  // triggering action
  controller.SetModel(nullptr);

  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  m_event_handler.Notify<DataChangedEvent>(&item, role);
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  m_event_handler.Notify<ModelResetEvent>(&model);
  m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}

TEST_F(AbstractViewModelControllerTests, TwoSubscriptions)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};
  const int role{42};

  mock_controller_t controller1;
  mock_controller_t controller2;

  EXPECT_CALL(controller1, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller1, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller1.SetModel(&m_model);

  EXPECT_CALL(controller2, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller2, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller2.SetModel(&m_model);

  DataChangedEvent expected_event{&item, role};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(controller1, OnModelEvent(AboutToInsertItemEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(AboutToInsertItemEvent{&item, tag_index})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(ItemInsertedEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ItemInsertedEvent{&item, tag_index})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(AboutToRemoveItemEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(AboutToRemoveItemEvent{&item, tag_index})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(ItemRemovedEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ItemRemovedEvent{&item, tag_index})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(DataChangedEvent{&item, role})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(DataChangedEvent{&item, role})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(ModelAboutToBeResetEvent{&model})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ModelAboutToBeResetEvent{&model})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(ModelResetEvent{&model})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ModelResetEvent{&model})).Times(1);

    EXPECT_CALL(controller1, OnModelEvent(ModelAboutToBeDestroyedEvent{&model})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ModelAboutToBeDestroyedEvent{&model})).Times(1);
  }

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

TEST_F(AbstractViewModelControllerTests, UnsubscribeOne)
{
  mvvm::SessionModel model;
  mvvm::SessionItem item;
  const mvvm::TagIndex tag_index{"tag", 0};
  const int role{42};

  mock_controller_t controller1;
  mock_controller_t controller2;

  EXPECT_CALL(controller1, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller1, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller1.SetModel(&m_model);

  EXPECT_CALL(controller2, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(controller2, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller2.SetModel(&m_model);

  EXPECT_CALL(controller1, UnsubscribeImpl()).Times(1);
  EXPECT_CALL(controller1, OnSetRootItemImpl(nullptr)).Times(1);
  controller1.SetModel(nullptr);

  {
    ::testing::InSequence seq;
    EXPECT_CALL(controller2, OnModelEvent(AboutToInsertItemEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ItemInsertedEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(AboutToRemoveItemEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ItemRemovedEvent{&item, tag_index})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(DataChangedEvent{&item, role})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ModelAboutToBeResetEvent{&model})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ModelResetEvent{&model})).Times(1);
    EXPECT_CALL(controller2, OnModelEvent(ModelAboutToBeDestroyedEvent{&model})).Times(1);
  }

  // triggering action
  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  m_event_handler.Notify<DataChangedEvent>(&item, role);
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  m_event_handler.Notify<ModelResetEvent>(&model);
  m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);

  EXPECT_CALL(controller2, UnsubscribeImpl()).Times(1);
  EXPECT_CALL(controller2, OnSetRootItemImpl(nullptr)).Times(1);
  controller2.SetModel(nullptr);

  m_event_handler.Notify<AboutToInsertItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemInsertedEvent>(&item, tag_index);
  m_event_handler.Notify<AboutToRemoveItemEvent>(&item, tag_index);
  m_event_handler.Notify<ItemRemovedEvent>(&item, tag_index);
  m_event_handler.Notify<DataChangedEvent>(&item, role);
  m_event_handler.Notify<ModelAboutToBeResetEvent>(&model);
  m_event_handler.Notify<ModelResetEvent>(&model);
  m_event_handler.Notify<ModelAboutToBeDestroyedEvent>(&model);
}
