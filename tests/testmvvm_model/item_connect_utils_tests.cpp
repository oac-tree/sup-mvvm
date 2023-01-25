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

#include "mvvm/signals/item_connect_utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/signals/signal_slot.h>
#include <mvvm/standarditems/standard_item_includes.h>

#include <memory>

using namespace mvvm;
using ::testing::_;

class ItemConnectUtilsTests : public ::testing::Test
{
public:
  class MockWidget
  {
  public:
    explicit MockWidget(SessionItem* item) : m_slot(std::make_unique<Slot>()) { SetItem(item); }

    void SetItem(mvvm::SessionItem* item)
    {
      // item inserted

      auto on_item_inserted = [this](SessionItem* item, const TagIndex& tag_index)
      { OnItemInserted(item, tag_index); };
      connect::OnItemInserted(item, on_item_inserted, m_slot.get());

      auto on_item_inserted_event = [this](const event_variant_t& event) { OnItemInserted(event); };
      connect::OnItemInserted(item, on_item_inserted_event, m_slot.get());

      connect::Connect<ItemInsertedEvent>(item, this, &MockWidget::OnConcreteEvent, m_slot.get());

      // about to remove item

      auto on_about_to_remove_item = [this](SessionItem* item, const TagIndex& tag_index)
      { OnAboutToRemoveItem(item, tag_index); };
      connect::OnAboutToRemoveItem(item, on_about_to_remove_item, m_slot.get());

      auto on_about_to_remove_item_event = [this](const event_variant_t& event)
      { OnAboutToRemoveItem(event); };
      connect::OnAboutToRemoveItem(item, on_about_to_remove_item_event, m_slot.get());

      connect::Connect<AboutToRemoveItemEvent>(item, this, &MockWidget::OnConcreteEvent,
                                               m_slot.get());

      // item removed

      auto on_item_removed = [this](SessionItem* item, const TagIndex& tag_index)
      { OnItemRemoved(item, tag_index); };
      connect::OnItemRemoved(item, on_item_removed, m_slot.get());

      auto on_item_removed_event = [this](const event_variant_t& event) { OnItemRemoved(event); };
      connect::OnItemRemoved(item, on_item_removed_event, m_slot.get());

      connect::Connect<ItemRemovedEvent>(item, this, &MockWidget::OnConcreteEvent, m_slot.get());

      // data changed

      auto on_data_changed = [this](SessionItem* item, int role) { OnDataChanged(item, role); };
      connect::OnDataChanged(item, on_data_changed, m_slot.get());

      auto on_data_changed_event = [this](const event_variant_t& event) { OnDataChanged(event); };
      connect::OnDataChanged(item, on_data_changed_event, m_slot.get());

      connect::Connect<DataChangedEvent>(item, this, &MockWidget::OnConcreteEvent, m_slot.get());

      // property changed

      auto on_property_changed = [this](SessionItem* item, const std::string& name)
      { OnPropertyChanged(item, name); };
      connect::OnPropertyChanged(item, on_property_changed, m_slot.get());

      auto on_property_changed_event = [this](const event_variant_t& event)
      { OnPropertyChanged(event); };
      connect::OnPropertyChanged(item, on_property_changed_event, m_slot.get());

      connect::Connect<PropertyChangedEvent>(item, this, &MockWidget::OnConcreteEvent,
                                             m_slot.get());
    }

    MOCK_METHOD(void, OnItemInserted, (SessionItem * item, TagIndex tagindex));
    MOCK_METHOD(void, OnItemInserted, (const mvvm::event_variant_t& event));
    MOCK_METHOD(void, OnConcreteEvent, (const ItemInsertedEvent& event));

    MOCK_METHOD(void, OnAboutToRemoveItem, (SessionItem * item, TagIndex tagindex));
    MOCK_METHOD(void, OnAboutToRemoveItem, (const mvvm::event_variant_t& event));
    MOCK_METHOD(void, OnConcreteEvent, (const AboutToRemoveItemEvent& event));

    MOCK_METHOD(void, OnItemRemoved, (SessionItem * item, TagIndex tagindex));
    MOCK_METHOD(void, OnItemRemoved, (const mvvm::event_variant_t& event));
    MOCK_METHOD(void, OnConcreteEvent, (const ItemRemovedEvent& event));

    MOCK_METHOD(void, OnDataChanged, (SessionItem * item, int role));
    MOCK_METHOD(void, OnDataChanged, (const mvvm::event_variant_t& event));
    MOCK_METHOD(void, OnConcreteEvent, (const DataChangedEvent& event));

    MOCK_METHOD(void, OnPropertyChanged, (SessionItem * item, std::string name));
    MOCK_METHOD(void, OnPropertyChanged, (const mvvm::event_variant_t& event));
    MOCK_METHOD(void, OnConcreteEvent, (const PropertyChangedEvent& event));

    std::unique_ptr<Slot> m_slot;
  };

  using mock_listener_t = ::testing::StrictMock<MockWidget>;
};

TEST_F(ItemConnectUtilsTests, GetEventHandler)
{
  EXPECT_THROW(connect::GetEventHandler(nullptr), NullArgumentException);

  // item without the model doesn't have an event handler
  const SessionItem item;
  EXPECT_THROW(connect::GetEventHandler(&item), NullArgumentException);

  // item belonging to SessionModel doesn't have an event handler
  const SessionModel model;
  EXPECT_THROW(connect::GetEventHandler(model.GetRootItem()), LogicErrorException);

  // item belonging to ApplicationModel does have an event handler
  const ApplicationModel application_model;
  EXPECT_NO_THROW(connect::GetEventHandler(application_model.GetRootItem()));
  EXPECT_NE(connect::GetEventHandler(application_model.GetRootItem()), nullptr);
}


//! Initialisation of the connection with wrong type of the model.

TEST_F(ItemConnectUtilsTests, OnDataChangeWrongModel)
{
  auto on_data_change = [this](SessionItem* item, int role) {};

  SessionItem item1;
  EXPECT_THROW(connect::OnDataChanged(&item1, on_data_change), NullArgumentException);

  SessionModel model;
  auto item2 = model.InsertItem<SessionItem>();
  EXPECT_THROW(connect::OnDataChanged(item2, on_data_change), LogicErrorException);

  ApplicationModel application_model;
  auto item3 = application_model.InsertItem<SessionItem>();
  EXPECT_NO_THROW(connect::OnDataChanged(item3, on_data_change));
}

//! Single call OnDataChanged expected when data was changed.

TEST_F(ItemConnectUtilsTests, OnDataChanged)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t widget(item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(widget, OnDataChanged(event_variant_t(expected_event))).Times(1);
  EXPECT_CALL(widget, OnConcreteEvent(expected_event)).Times(1);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Expect no calls OnDataChanged after disconnection.

TEST_F(ItemConnectUtilsTests, OnDataChangedAfterDisconnection)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t widget(item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  // expect notification
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(widget, OnDataChanged(event_variant_t(expected_event))).Times(1);
  EXPECT_CALL(widget, OnConcreteEvent(expected_event)).Times(1);

  item->SetData(45, expected_role);

  // disconnect widget, expect no notifications
  widget.m_slot.reset();

  // no notifications are expected here, strict mock will notify us if it's not the case

  item->SetData(46, expected_role);
}

//! Expect no calls OnDataChanged when data is the same.

TEST_F(ItemConnectUtilsTests, OnDataChangedSameData)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t widget(item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  // expect notification
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(widget, OnDataChanged(event_variant_t(expected_event))).Times(1);
  EXPECT_CALL(widget, OnConcreteEvent(expected_event)).Times(1);

  item->SetData(45, expected_role);

  // no notifications are expected here, strict mock will notify us if it's not the case

  item->SetData(45, expected_role);
}

//! Expect no calls OnDataChanged when other item is changed.

TEST_F(ItemConnectUtilsTests, OnDataChangedDifferentItem)
{
  ApplicationModel model;
  auto item1 = model.InsertItem<SessionItem>();
  item1->SetData(42, DataRole::kData);
  auto item2 = model.InsertItem<SessionItem>();
  item2->SetData(43, DataRole::kData);

  mock_listener_t widget(item1);

  // expect no notification
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  item2->SetData(45, DataRole::kData);  // other item changed
}

//! Single call OnPropertyChanged expected when item's property was changed.

TEST_F(ItemConnectUtilsTests, OnPropertyChanged)
{
  const std::string property_name("height");

  ApplicationModel model;
  auto item = model.InsertItem<CompoundItem>();
  item->AddProperty(property_name, 42.0);

  mock_listener_t widget(item);
  const auto expected_item = item;

  EXPECT_CALL(widget, OnPropertyChanged(expected_item, property_name)).Times(1);

  PropertyChangedEvent expected_event{expected_item, property_name};
  EXPECT_CALL(widget, OnPropertyChanged(event_variant_t(expected_event))).Times(1);
  EXPECT_CALL(widget, OnConcreteEvent(expected_event)).Times(1);

  // trigger calls
  item->SetProperty(property_name, 43.0);
}

//! Inserting item to item.

TEST_F(ItemConnectUtilsTests, OnItemInserted)
{
  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  mock_listener_t widget(compound);

  const TagIndex expected_tagindex{"tag1", 0};
  EXPECT_CALL(widget, OnItemInserted(compound, expected_tagindex)).Times(1);

  ItemInsertedEvent expected_event{compound, expected_tagindex};
  EXPECT_CALL(widget, OnItemInserted(event_variant_t(expected_event))).Times(1);
  EXPECT_CALL(widget, OnConcreteEvent(expected_event)).Times(1);

  // perform action
  model.InsertItem<CompoundItem>(compound, expected_tagindex);
}

//! Removing item.

TEST_F(ItemConnectUtilsTests, OnItemRemoved)
{
  const TagIndex expected_tagindex{"tag1", 0};

  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = model.InsertItem<CompoundItem>(compound, expected_tagindex);

  mock_listener_t widget(compound);

  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(widget, OnAboutToRemoveItem(compound, expected_tagindex)).Times(1);

    AboutToRemoveItemEvent expected_event1{compound, expected_tagindex};
    EXPECT_CALL(widget, OnAboutToRemoveItem(event_variant_t(expected_event1))).Times(1);
    EXPECT_CALL(widget, OnConcreteEvent(expected_event1)).Times(1);

    EXPECT_CALL(widget, OnItemRemoved(compound, expected_tagindex)).Times(1);

    ItemRemovedEvent expected_event2{compound, expected_tagindex};
    EXPECT_CALL(widget, OnItemRemoved(event_variant_t(expected_event2))).Times(1);
    EXPECT_CALL(widget, OnConcreteEvent(expected_event2)).Times(1);
  }

  // perform action
  model.RemoveItem(child);
}
