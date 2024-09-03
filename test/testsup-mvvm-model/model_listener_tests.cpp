/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include <mvvm/core/exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/item_constants.h>
#include <mvvm/model/property_item.h>
#include <mvvm/signals/model_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Testing ModelListener class.

class ModelListenerTests : public ::testing::Test
{
public:
  //! Mock client to validate callbacks.
  class TestClient
  {
  public:
    MOCK_METHOD(void, OnEvent, (const mvvm::event_variant_t& event));
    MOCK_METHOD(void, OnConcreteEvent, (const mvvm::DataChangedEvent& event));
  };

  class TestClientV2
  {
  public:
    MOCK_METHOD(void, OnEvent, (const mvvm::AboutToInsertItemEvent& event));
    MOCK_METHOD(void, OnEvent, (const mvvm::ItemInsertedEvent& event));
  };
};

TEST_F(ModelListenerTests, InitialState)
{
  const ApplicationModel model;
  const ModelListener listener(&model);
  EXPECT_EQ(listener.GetModel(), &model);
}

TEST_F(ModelListenerTests, AttemptToInitializeToWrongModel)
{
  EXPECT_THROW(ModelListener(nullptr), NullArgumentException);

  const SessionModel model;
  // expect exception in the absence of signaling capabilities
  EXPECT_THROW((ModelListener(&model)), NullArgumentException);
}

//! Creating the model with item and listener attached. The client is a lambda listening for
//! DataChangedEvent in the form of event_variant_t.
TEST_F(ModelListenerTests, LambdaOnVariant)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  ModelListener listener(&model);

  testing::MockFunction<void(const event_variant_t&)> client;

  // the client is setup to receive DataChangedEvent in the form of event_variant_t.
  listener.Connect<DataChangedEvent>(client.AsStdFunction());

  // setting up expectation
  DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(client, Call(event_variant_t(expected_event))).Times(1);

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));
}

TEST_F(ModelListenerTests, DefaultTemplateParameter)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  auto listener = std::make_unique<ModelListener>(&model);

  testing::MockFunction<void(const event_variant_t&)> client;

  // the client is setup to receive DataChangedEvent in the form of event_variant_t.
  listener->Connect<DataChangedEvent>(client.AsStdFunction());

  // setting up expectation
  DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(client, Call(event_variant_t(expected_event))).Times(1);

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));
}

//! Creating the model with item and listener attached. The client is a lambda listening directly
//! for DataChangedEvent (no variant).
TEST_F(ModelListenerTests, LambdaOnConcreteEvent)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  ModelListener listener(&model);

  testing::MockFunction<void(const DataChangedEvent&)> client;

  // the client is setup to receive DataChangedEvent in the form of event_variant_t.
  listener.Connect<DataChangedEvent>(client.AsStdFunction());

  // setting up expectation
  const DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(client, Call(expected_event)).Times(1);

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));
}

//! Creating the model with item and listener attached.
//! The client is setup to receive DataChangedEvents.
TEST_F(ModelListenerTests, SingleClientOnEvent)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  ModelListener listener(&model);

  // the client is setup to receive DataChangedEvent
  TestClient client;
  listener.Connect<DataChangedEvent>(&client, &TestClient::OnEvent);

  // setting up expectation
  DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(client, OnEvent(event_variant_t(expected_event))).Times(1);

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));
}

//! Same test as previous.
//! Additionally we check if we can connect callback methods based on the concrete event type.
TEST_F(ModelListenerTests, SingleClientOnEventVariousConnectionAPI)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  ModelListener listener(&model);

  // the client is setup to receive DataChangedEvent
  // (testing connection to the method based on event_variant_t and on concrete DataChangedEvent)
  TestClient client;
  listener.Connect<DataChangedEvent>(&client, &TestClient::OnEvent);
  listener.Connect<DataChangedEvent>(&client, &TestClient::OnConcreteEvent);
  listener.Connect(&client, &TestClient::OnConcreteEvent);  // template argument deduction

  DataChangedEvent expected_event{item, DataRole::kData};
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(client, OnEvent(event_variant_t(expected_event))).Times(1);
    EXPECT_CALL(client, OnConcreteEvent(expected_event)).Times(1);
    EXPECT_CALL(client, OnConcreteEvent(expected_event)).Times(1);
  }

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));
}

//! Creating the model with item and listener attached.
//! The client is setup to receive DataChangedEvents. We check that after listener destruction, no
//! callbacks are triggered.
TEST_F(ModelListenerTests, ListenerTimeOfLife)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  auto listener = std::make_unique<ModelListener>(&model);

  // the client is setup to receive DataChangedEvent
  TestClient client;
  listener->Connect<DataChangedEvent>(&client, &TestClient::OnEvent);

  // setting up expectation
  DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(client, OnEvent(event_variant_t(expected_event))).Times(1);

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));

  // after listener destruction we expect no calls
  listener.reset();
  EXPECT_CALL(client, OnEvent(_)).Times(0);

  // triggering expectations again
  EXPECT_TRUE(model.SetData(item, 43, DataRole::kData));
}

//! Connecting to the client with two methods with the same name

TEST_F(ModelListenerTests, MethodOverload)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto parent = model.GetRootItem();
  const TagIndex tag_index{constants::kRootItemTag, 0};

  auto listener = std::make_unique<ModelListener>(&model);

  // the client is setup to receive DataChangedEvent
  TestClientV2 client;
  listener->Connect<AboutToInsertItemEvent>(&client, &TestClientV2::OnEvent);
  listener->Connect<ItemInsertedEvent>(&client, &TestClientV2::OnEvent);

  {
    const ::testing::InSequence seq;

    auto expected_event1 = AboutToInsertItemEvent{parent, tag_index};
    auto expected_event2 = ItemInsertedEvent{parent, tag_index};

    EXPECT_CALL(client, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(client, OnEvent(expected_event2)).Times(1);
  }

  auto item = model.InsertItem<SessionItem>();
  EXPECT_EQ(item, model.GetRootItem()->GetAllItems()[0]);
}
