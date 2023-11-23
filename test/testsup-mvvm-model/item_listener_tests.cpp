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

#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/test/mock_callback_listener.h>
#include <mvvm/test/mock_item_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;
using mock_listener_t = ::testing::StrictMock<testutils::MockItemListener>;

class ItemListenerTests : public ::testing::Test
{
};

TEST_F(ItemListenerTests, InitialState)
{
  const ItemListener<SessionItem> listener;
  EXPECT_EQ(listener.GetItem(), nullptr);
}

TEST_F(ItemListenerTests, SetItem)
{
  ItemListener<SessionItem> listener;

  // listener can't listen an item without the model
  SessionItem item1;
  EXPECT_THROW(listener.SetItem(&item1), std::runtime_error);

  // listener can't listen an item belonging to the model without notifications
  SessionModel session_model;
  auto item2 = session_model.InsertItem<PropertyItem>();
  EXPECT_THROW(listener.SetItem(item2), std::runtime_error);

  // set proper item
  ApplicationModel app_model;
  auto item3 = app_model.InsertItem<PropertyItem>();
  EXPECT_NO_THROW(listener.SetItem(item3));
  EXPECT_EQ(listener.GetItem(), item3);
}

//! Single call OnDataChanged expected when data was changed.

TEST_F(ItemListenerTests, OnDataChanged)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t listener(item);

  EXPECT_EQ(listener.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Single call OnDataChanged expected when data was changed (double subscription).

TEST_F(ItemListenerTests, OnDataChangedSubscribeTwice)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t listener(item);

  listener.SetItem(item);  // intenionally set item second time to see that no double subscription
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Expect no calls OnDataChanged after disconnection.

TEST_F(ItemListenerTests, OnDataChangedAfterDisconnection)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t listener(item);

  EXPECT_EQ(listener.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  // expect notification
  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  EXPECT_CALL(listener, Unsubscribe()).Times(1);
  item->SetData(45, expected_role);

  // disconnect widget
  listener.SetItem(nullptr);
  EXPECT_EQ(listener.GetItem(), nullptr);

  // expect no notifications here

  item->SetData(46, expected_role);
}

//! Single call OnDataChanged expected when data was changed.
//! Same as before, external callback is used.

TEST_F(ItemListenerTests, OnDataChangedCallback)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t listener(item);
  testutils::MockCallbackListener<event_variant_t> widget;

  listener.Connect<DataChangedEvent>(widget.CreateCallback());

  EXPECT_EQ(listener.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);
  EXPECT_CALL(widget, OnCallback(event_variant_t(expected_event))).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Single call OnPropertyChanged expected when item's property was changed.

TEST_F(ItemListenerTests, OnPropertyChanged)
{
  const std::string property_name("height");

  ApplicationModel model;
  auto item = model.InsertItem<CompoundItem>();
  auto& property = item->AddProperty(property_name, 42.0);

  mock_listener_t listener(item);

  const auto expected_item = item;
  PropertyChangedEvent expected_event{expected_item, property_name};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  // trigger calls
  item->SetProperty(property_name, 43.0);
}

//! Inserting item to item.

TEST_F(ItemListenerTests, OnItemInserted)
{
  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  mock_listener_t listener(compound);

  const TagIndex expected_tagindex{"tag1", 0};
  ItemInsertedEvent expected_event{compound, expected_tagindex};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  // perform action
  model.InsertItem<CompoundItem>(compound, expected_tagindex);
}

//! Removing item.

TEST_F(ItemListenerTests, OnItemRemoved)
{
  const TagIndex expected_tagindex{"tag1", 0};

  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = model.InsertItem<CompoundItem>(compound, expected_tagindex);

  mock_listener_t listener(compound);

  {
    ::testing::InSequence seq;
    AboutToRemoveItemEvent expected_event1{compound, expected_tagindex};
    EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event1))).Times(1);
    ItemRemovedEvent expected_event2{compound, expected_tagindex};
    EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event2))).Times(1);
  }

  // perform action
  model.RemoveItem(child);
}

//! Setting another item.

TEST_F(ItemListenerTests, SetAnotherItem)
{
  const TagIndex expected_tagindex{"tag1", 0};

  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  mock_listener_t listener(compound);

  ItemInsertedEvent expected_event{compound, expected_tagindex};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  auto child = model.InsertItem<CompoundItem>(compound, expected_tagindex);

  EXPECT_CALL(listener, Unsubscribe()).Times(1);

  listener.SetItem(child);  // switching to another item

  // No other calls expected. StrictMock will fail if it's not the case.

  // perform action, complete silence is expected
  model.RemoveItem(child);
}

//! Removing item.

TEST_F(ItemListenerTests, OnControllerDelete)
{
  const TagIndex expected_tagindex{"tag1", 0};

  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = model.InsertItem<CompoundItem>(compound, expected_tagindex);

  {
    testutils::MockItemListener listener(compound);
  }

  // controller was deleted, signals disconnected
  EXPECT_NO_FATAL_FAILURE(model.RemoveItem(child));
}
