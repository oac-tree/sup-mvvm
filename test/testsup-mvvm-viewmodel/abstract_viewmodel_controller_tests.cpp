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

#include "mvvm/viewmodel/abstract_viewmodel_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/signals/model_event_handler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

/**
 * @brief Tests of AbstractViewModelController class.
 */
class AbstractViewModelControllerTest : public ::testing::Test
{
public:
  AbstractViewModelControllerTest() : m_model(&m_event_handler) {}

  class TestModel : public SessionModel
  {
  public:
    explicit TestModel(ModelEventHandler* event_handler) : m_event_handler(event_handler) {}

    ModelEventHandler* GetEventHandler() const override { return m_event_handler; }

    mvvm::ModelEventHandler* m_event_handler;
  };

  class TestController : public AbstractViewModelController
  {
  public:
    const SessionItem* GetRootItem() const override { return nullptr; }

  private:
    virtual void SetRootItemImpl(SessionItem* root_item) override { (void)root_item; }
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

    void SubscribeImpl(ISessionModel* model) override
    {
      SubscribeAll(model);
      OnSubscribeImpl(model);
    }

    MOCK_METHOD(void, UnsubscribeImpl, (), ());

    MOCK_METHOD(void, OnSubscribeImpl, (ISessionModel * root_item), ());
    MOCK_METHOD(void, OnSetRootItemImpl, (SessionItem * item), ());

    SessionItem* m_root_item{nullptr};
  };

  using mock_controller_t = ::testing::StrictMock<NotifyingTestController>;
  TestModel m_model;
  ModelEventHandler m_event_handler;
};

TEST_F(AbstractViewModelControllerTest, TestControllerInitialState)
{
  TestController controller;
  EXPECT_EQ(controller.GetRootItem(), nullptr);
  EXPECT_EQ(controller.GetColumnCount(), 0);
  EXPECT_TRUE(controller.GetHorizontalHeaderLabels().empty());

  SessionItem root_without_model;
  EXPECT_THROW(controller.SetRootItem(&root_without_model), RuntimeException);

  mvvm::SessionItem item;
  mvvm::SessionModel model;
  const int role{42};
  const mvvm::TagIndex tag_index{"tag", 0};
  EXPECT_NO_THROW(controller.OnModelEvent(AboutToInsertItemEvent{&item, tag_index}));
  EXPECT_NO_THROW(controller.OnModelEvent(ItemInsertedEvent{&item, tag_index}));
  EXPECT_NO_THROW(controller.OnModelEvent(AboutToRemoveItemEvent{&item, tag_index}));
  EXPECT_NO_THROW(controller.OnModelEvent(ItemRemovedEvent{&item, tag_index}));
  EXPECT_NO_THROW(controller.OnModelEvent(DataChangedEvent{&item, role}));
  EXPECT_NO_THROW(controller.OnModelEvent(ModelAboutToBeResetEvent{&model}));
  EXPECT_NO_THROW(controller.OnModelEvent(ModelResetEvent{&model}));
  EXPECT_NO_THROW(controller.OnModelEvent(ModelAboutToBeDestroyedEvent{&model}));
}

//! Setting the model, checking calls of SetRootItemImpl
TEST_F(AbstractViewModelControllerTest, SetModel)
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
TEST_F(AbstractViewModelControllerTest, SetRootItem)
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

TEST_F(AbstractViewModelControllerTest, SetModelThenSetSameRootItem)
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

  // setting same root item shouldn't trigger a call
  EXPECT_CALL(controller, OnSubscribeImpl(_)).Times(0);
  EXPECT_CALL(controller, OnSetRootItemImpl(_)).Times(0);
  controller.SetRootItem(model.GetRootItem());

  EXPECT_EQ(controller.GetModel(), &model);
  EXPECT_EQ(controller.GetRootItem(), model.GetRootItem());
}

TEST_F(AbstractViewModelControllerTest, SetRootItemThenAnotherItemFromSameModel)
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

  // setting another item shouldn trigger a call
  EXPECT_CALL(controller, OnSetRootItemImpl(item)).Times(1);
  controller.SetRootItem(item);

  EXPECT_EQ(controller.GetModel(), &model);
  EXPECT_EQ(controller.GetRootItem(), item);
}

TEST_F(AbstractViewModelControllerTest, SetRootItemThenAnotherItemFromDifferentModel)
{
  ApplicationModel model1;
  ApplicationModel model2;

  auto item = model2.InsertItem<SessionItem>();
  mock_controller_t controller;

  EXPECT_EQ(controller.GetModel(), nullptr);
  EXPECT_EQ(controller.GetRootItem(), nullptr);

  EXPECT_CALL(controller, OnSubscribeImpl(&model1)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(model1.GetRootItem())).Times(1);
  controller.SetRootItem(model1.GetRootItem());

  EXPECT_EQ(controller.GetModel(), &model1);
  EXPECT_EQ(controller.GetRootItem(), model1.GetRootItem());

  // setting another item from different model shouldn trigger corresponding unsubscribe/subscribe
  // calls
  EXPECT_CALL(controller, UnsubscribeImpl()).Times(1);
  EXPECT_CALL(controller, OnSubscribeImpl(&model2)).Times(1);
  EXPECT_CALL(controller, OnSetRootItemImpl(item)).Times(1);
  controller.SetRootItem(item);

  EXPECT_EQ(controller.GetModel(), &model2);
  EXPECT_EQ(controller.GetRootItem(), item);
}

TEST_F(AbstractViewModelControllerTest, SubscribeTo)
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

TEST_F(AbstractViewModelControllerTest, UnsubscribeOnDeletion)
{
  mvvm::SessionItem item;
  const int role{42};

  const mvvm::ModelEventHandler event_handler;

  auto controller = std::make_unique<mock_controller_t>();

  EXPECT_CALL(*controller, OnSubscribeImpl(&m_model)).Times(1);
  EXPECT_CALL(*controller, OnSetRootItemImpl(m_model.GetRootItem())).Times(1);

  controller->SetModel(&m_model);

  controller.reset();

  ASSERT_NO_FATAL_FAILURE(m_event_handler.Notify<DataChangedEvent>(&item, role));
}

TEST_F(AbstractViewModelControllerTest, EventHandlerIsDestroyedBeforeController)
{
  const mvvm::SessionItem item;
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

TEST_F(AbstractViewModelControllerTest, OnAboutToInsertItemNotify)
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

TEST_F(AbstractViewModelControllerTest, OnItemInsertedNotify)
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

TEST_F(AbstractViewModelControllerTest, OnAboutToRemoveItemNotify)
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

TEST_F(AbstractViewModelControllerTest, OnItemRemovedNotify)
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

TEST_F(AbstractViewModelControllerTest, OnDataChangedNotify)
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

TEST_F(AbstractViewModelControllerTest, OnModelAboutToBeReset)
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

TEST_F(AbstractViewModelControllerTest, OnModelReset)
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

TEST_F(AbstractViewModelControllerTest, OnModelAboutToBeDestroyed)
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

TEST_F(AbstractViewModelControllerTest, UnsubscribeV2)
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

TEST_F(AbstractViewModelControllerTest, TwoSubscriptions)
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

  const DataChangedEvent expected_event{&item, role};

  {
    const ::testing::InSequence seq;
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

TEST_F(AbstractViewModelControllerTest, UnsubscribeOne)
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
    const ::testing::InSequence seq;
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
