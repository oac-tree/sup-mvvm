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

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/test/mock_item_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class ItemListenerTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockItemListener>;
};

TEST_F(ItemListenerTest, InitialState)
{
  EXPECT_THROW(ItemListener{nullptr}, RuntimeException);

  // item without the model is not allowed
  {
    SessionItem item;
    EXPECT_THROW(ItemListener{&item}, RuntimeException);
  }

  // model without notifications is not allowed
  {
    SessionModel session_model;
    auto item = session_model.InsertItem<PropertyItem>();
    EXPECT_THROW(ItemListener{item}, RuntimeException);
  }

  {
    ApplicationModel app_model;
    auto item = app_model.InsertItem<PropertyItem>();
    ItemListener listener(item);
    EXPECT_EQ(listener.GetItem(), item);
  }
}

//! Single call OnDataChanged expected when data was changed.
TEST_F(ItemListenerTest, OnDataChanged)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  {
    mock_listener_t listener(item);

    EXPECT_EQ(listener.GetItem(), item);

    DataChangedEvent expected_event{expected_item, expected_role};
    EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(1);

    item->SetData(45, expected_role);  // trigger expectations
  }

  // listener was deleted, signals disconnected
  EXPECT_NO_FATAL_FAILURE(item->SetData(46, expected_role));
}

//! Single call OnDataChanged expected when data was changed.
//! Same as before, external callback is used.
TEST_F(ItemListenerTest, OnDataChangedCallback)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  testing::MockFunction<void(const event_variant_t&)> widget;

  {
    mock_listener_t listener(item);

    listener.Connect<DataChangedEvent>(widget.AsStdFunction());

    EXPECT_EQ(listener.GetItem(), item);

    DataChangedEvent expected_event{expected_item, expected_role};
    EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(1);
    EXPECT_CALL(widget, Call(event_variant_t(expected_event))).Times(1);

    // trigger calls
    item->SetData(45, expected_role);
  }

  // no calls after listener destruction
  EXPECT_CALL(widget, Call(_)).Times(0);
  item->SetData(46, expected_role);
}

//! Single call OnDataChanged expected when data was changed.
//! Same as before, external event_variant_t callback is used.
TEST_F(ItemListenerTest, LambdaOnEventVariant)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  ItemListener listener(item);
  testing::MockFunction<void(const event_variant_t&)> widget;

  listener.Connect<DataChangedEvent>(widget.AsStdFunction());

  EXPECT_EQ(listener.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(widget, Call(event_variant_t(expected_event))).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Single call OnDataChanged expected when data was changed.
//! Same as before, external callback based on concrete event is used.
TEST_F(ItemListenerTest, LambdaOnConcreteEvent)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  ItemListener listener(item);
  testing::MockFunction<void(const DataChangedEvent&)> widget;

  listener.Connect<DataChangedEvent>(widget.AsStdFunction());

  EXPECT_EQ(listener.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(widget, Call(expected_event)).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Single call OnPropertyChanged expected when item's property was changed.
TEST_F(ItemListenerTest, OnPropertyChanged)
{
  const std::string property_name("height");

  ApplicationModel model;
  auto item = model.InsertItem<CompoundItem>();
  auto& property = item->AddProperty(property_name, 42.0);

  mock_listener_t listener(item);

  const auto expected_item = item;
  PropertyChangedEvent expected_event{expected_item, property_name};
  EXPECT_CALL(listener, OnPropertyChanged(expected_event)).Times(1);

  // trigger calls
  item->SetProperty(property_name, 43.0);
}

//! Inserting item to item.
TEST_F(ItemListenerTest, OnItemInserted)
{
  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  mock_listener_t listener(compound);

  const TagIndex expected_tagindex{"tag1", 0};
  ItemInsertedEvent expected_event{compound, expected_tagindex};
  EXPECT_CALL(listener, OnItemInserted(expected_event)).Times(1);

  // perform action
  model.InsertItem<CompoundItem>(compound, expected_tagindex);
}

//! Removing item.
TEST_F(ItemListenerTest, OnItemRemoved)
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
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    ItemRemovedEvent expected_event2{compound, expected_tagindex};
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }

  // perform action
  model.RemoveItem(child);
}
