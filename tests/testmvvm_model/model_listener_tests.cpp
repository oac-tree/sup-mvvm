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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/signals/model_listener.h>

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
};

TEST_F(ModelListenerTests, InitialState)
{
  ApplicationModel model;
  ModelListener<ApplicationModel> listener(&model);
  EXPECT_EQ(listener.GetModel(), &model);
}

//! Creating the model with item and listener attached.
//! The client is setup to receive DataChangedEvents.
TEST_F(ModelListenerTests, SingleClientOnEvent)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  ModelListener<ApplicationModel> listener(&model);

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
//! Additionally we check if we can connect
TEST_F(ModelListenerTests, SingleClientOnEventVariousConnectionAPI)
{
  // the model with the item and listener attached
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  ModelListener<ApplicationModel> listener(&model);

  // the client is setup to receive DataChangedEvent
  // (testing connection to the method based on event_variant_t and on concrete DataChangedEvent)
  TestClient client;
  listener.Connect<DataChangedEvent>(&client, &TestClient::OnEvent);
  listener.Connect<DataChangedEvent>(&client, &TestClient::OnConcreteEvent);
  listener.Connect(&client, &TestClient::OnConcreteEvent); // template argument deduction

  DataChangedEvent expected_event{item, DataRole::kData};
  {
    ::testing::InSequence seq;
    EXPECT_CALL(client, OnEvent(event_variant_t(expected_event))).Times(1);
    EXPECT_CALL(client, OnConcreteEvent(expected_event)).Times(1);
    EXPECT_CALL(client, OnConcreteEvent(expected_event)).Times(1);
  }

  // triggering expectations by changing the data
  EXPECT_TRUE(model.SetData(item, 42, DataRole::kData));
}
