/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/signals/itemlistener.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/standarditems/standarditemincludes.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class ItemListenerTest : public ::testing::Test
{
public:
  class TestController : public ItemListener<SessionItem>
  {
  public:
    TestController(SessionItem* item) { SetItem(item); }
    MOCK_METHOD2(OnItemInserted, void(SessionItem* item, TagIndex tagindex));
    MOCK_METHOD2(OnAboutToRemoveItem, void(SessionItem* item, TagIndex tagindex));
    MOCK_METHOD2(OnItemRemoved, void(SessionItem* item, TagIndex tagindex));
    MOCK_METHOD2(OnDataChanged, void(SessionItem* item, int role));
    MOCK_METHOD2(OnPropertyChanged, void(SessionItem* item, std::string name));

    MOCK_METHOD0(Unsubscribe, void(void));

  protected:
    void Subscribe() override
    {
      auto on_item_inserted = [this](SessionItem* item, const TagIndex& tag_index)
      { OnItemInserted(item, tag_index); };
      SetOnItemInserted(on_item_inserted);

      auto on_about_to_remove_item = [this](SessionItem* item, const TagIndex& tag_index)
      { OnAboutToRemoveItem(item, tag_index); };
      SetOnAboutToRemoveItem(on_about_to_remove_item);

      auto on_item_removed = [this](SessionItem* item, const TagIndex& tag_index)
      { OnItemRemoved(item, tag_index); };
      SetOnItemRemoved(on_item_removed);

      auto on_data_changed = [this](SessionItem* item, int role) { OnDataChanged(item, role); };
      SetOnDataChanged(on_data_changed);

      auto on_property_changed = [this](SessionItem* item, const std::string& name)
      { OnPropertyChanged(item, name); };
      SetOnPropertyChanged(on_property_changed);
    }
  };
};

TEST_F(ItemListenerTest, InitialState)
{
  ItemListener<SessionItem> listener;
  EXPECT_EQ(listener.GetItem(), nullptr);
}

TEST_F(ItemListenerTest, SetItem)
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

TEST_F(ItemListenerTest, OnDataChanged)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  TestController widget(item);
  EXPECT_EQ(widget.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(widget, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);
  EXPECT_CALL(widget, Unsubscribe()).Times(0);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Single call OnDataChanged expected when data was changed (double subscription).

TEST_F(ItemListenerTest, OnDataChangedSubscribeTwice)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  TestController widget(item);
  widget.SetItem(item);  // intenionally set item second time to see that no double subscription
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(widget, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);

  // trigger calls
  item->SetData(45, expected_role);
}

//! Expect no calls OnDataChanged after disconnection.

TEST_F(ItemListenerTest, OnDataChangedAfterDisconnection)
{
  ApplicationModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42, DataRole::kData);

  TestController widget(item);
  EXPECT_EQ(widget.GetItem(), item);
  const auto expected_role = DataRole::kData;
  const auto expected_item = item;

  // expect notification
  EXPECT_CALL(widget, OnDataChanged(expected_item, expected_role)).Times(1);
  EXPECT_CALL(widget, Unsubscribe()).Times(1);
  item->SetData(45, expected_role);

  // disconnect widget, expect no notifications
  widget.SetItem(nullptr);

  EXPECT_EQ(widget.GetItem(), nullptr);

  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  item->SetData(46, expected_role);
}

//! Single call OnPropertyChanged expected when item's property was changed.

TEST_F(ItemListenerTest, OnPropertyChanged)
{
  const std::string property_name("height");

  ApplicationModel model;
  auto item = model.InsertItem<CompoundItem>();
  auto property = item->AddProperty(property_name, 42.0);

  TestController widget(item);
  const auto expected_item = item;

  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(widget, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(expected_item, property_name)).Times(1);

  // trigger calls
  item->SetProperty(property_name, 43.0);
}

//! Inserting item to item.

TEST_F(ItemListenerTest, OnItemInserted)
{
  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  TestController widget(compound);

  const TagIndex expected_tagindex{"tag1", 0};
  EXPECT_CALL(widget, OnItemInserted(compound, expected_tagindex)).Times(1);
  EXPECT_CALL(widget, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(widget, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);

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

  TestController widget(compound);

  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(widget, OnAboutToRemoveItem(compound, expected_tagindex)).Times(1);
    EXPECT_CALL(widget, OnItemRemoved(compound, expected_tagindex)).Times(1);
  }
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);

  // perform action
  model.RemoveItem(child);
}

//! Setting another item.

TEST_F(ItemListenerTest, SetAnotherItem)
{
  const TagIndex expected_tagindex{"tag1", 0};

  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  TestController widget(compound);

  auto child = model.InsertItem<CompoundItem>(compound, expected_tagindex);
  widget.SetItem(child);  // switching to another item

  EXPECT_CALL(widget, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(widget, OnAboutToRemoveItem(compound, expected_tagindex)).Times(0);
  EXPECT_CALL(widget, OnItemRemoved(compound, expected_tagindex)).Times(0);
  EXPECT_CALL(widget, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(widget, OnPropertyChanged(_, _)).Times(0);

  // perform action, complete silence is expected
  model.RemoveItem(child);
}

//! Removing item.

TEST_F(ItemListenerTest, OnControllerDelete)
{
  const TagIndex expected_tagindex{"tag1", 0};

  ApplicationModel model;
  auto compound = model.InsertItem<CompoundItem>();
  compound->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = model.InsertItem<CompoundItem>(compound, expected_tagindex);

  {
    TestController widget(compound);
  }


  EXPECT_NO_FATAL_FAILURE(model.RemoveItem(child));
}
