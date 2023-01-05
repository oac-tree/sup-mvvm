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
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionmodel.h>
#include <testutils/mock_model_listener.h>

using namespace mvvm;
using ::testing::_;

class ModelListenerTests : public ::testing::Test
{
public:
  using mock_model_listener_t = ::testing::StrictMock<testutils::MockModelListener>;
  ApplicationModel m_model;
  TagIndex m_tag_index;
};

//! Setting data through the model and checking the result.

TEST_F(ModelListenerTests, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();

  mock_model_listener_t listener(&m_model);

  DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  // changing the data and checking result
  EXPECT_TRUE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Setting data through the item.

TEST_F(ModelListenerTests, SetDataThroughItem)
{
  auto item = m_model.InsertItem<PropertyItem>();

  mock_model_listener_t listener(&m_model);

  DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(listener, OnEvent(event_variant_t(expected_event))).Times(1);

  // changing the data through the item (should still trigger notifications through the model)
  EXPECT_TRUE(item->SetData(42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Setting same data through the composer and checking the result.
//! No notifications are expected.

TEST_F(ModelListenerTests, SetSameData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42, DataRole::kData);

  mock_model_listener_t listener(&m_model);

  // no notifications are expected
  // since we are using StrictMock the test will fail if some notifications follow

  // changing to the same value
  EXPECT_FALSE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion.

TEST_F(ModelListenerTests, InsertItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  TagIndex tag_index{"tag", 0};

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;

    event_variant_t expected_event1 = AboutToInsertItemEvent{parent, tag_index};
    event_variant_t expected_event2 = ItemInsertedEvent{parent, tag_index};

    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion.
//! Using defaut tag (real-life bug).

TEST_F(ModelListenerTests, InsertItemInDefaultTag)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  TagIndex tag_index{"tag", 0};

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;

    event_variant_t expected_event1 = AboutToInsertItemEvent{parent, tag_index};
    event_variant_t expected_event2 = ItemInsertedEvent{parent, tag_index};

    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent);
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item through the composer into another parent using move insertion.

TEST_F(ModelListenerTests, InsertItemViaMove)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  TagIndex tag_index{"tag", 0};

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;

    event_variant_t expected_event1 = AboutToInsertItemEvent{parent, tag_index};
    event_variant_t expected_event2 = ItemInsertedEvent{parent, tag_index};

    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  // inserting item
  auto to_insert = std::make_unique<PropertyItem>();
  auto to_insert_ptr = to_insert.get();
  auto item = m_model.InsertItem(std::move(to_insert), parent, {"tag", 0});
  EXPECT_EQ(item, to_insert_ptr);
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Removing item.

TEST_F(ModelListenerTests, TakeItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);
  TagIndex tag_index{"tag", 0};

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;
    event_variant_t expected_event1 = AboutToRemoveItemEvent{parent, tag_index};
    event_variant_t expected_event2 = ItemRemovedEvent{parent, tag_index};
    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  // removing item
  auto taken = m_model.TakeItem(parent, {"tag", 0});
  EXPECT_EQ(taken.get(), child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Removing item.

TEST_F(ModelListenerTests, RemoveItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);
  TagIndex tag_index{"tag", 0};

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;
    event_variant_t expected_event1 = AboutToRemoveItemEvent{parent, tag_index};
    event_variant_t expected_event2 = ItemRemovedEvent{parent, tag_index};
    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  // removing item
  m_model.RemoveItem(child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Removing item.

TEST_F(ModelListenerTests, MoveItem)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent1);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);
  TagIndex tag_index1{"tag1", 0};
  TagIndex tag_index2{"tag2", 0};

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;

    event_variant_t expected_remove_event1 = AboutToRemoveItemEvent{parent1, tag_index1};
    event_variant_t expected_remove_event2 = ItemRemovedEvent{parent1, tag_index1};
    EXPECT_CALL(listener, OnEvent(expected_remove_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_remove_event2)).Times(1);

    event_variant_t expected_insert_event1 = AboutToInsertItemEvent{parent2, tag_index2};
    event_variant_t expected_insert_event2 = ItemInsertedEvent{parent2, tag_index2};
    EXPECT_CALL(listener, OnEvent(expected_insert_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_insert_event2)).Times(1);
  }

  // removing item
  m_model.MoveItem(child, parent2, tag_index2);
  EXPECT_EQ(parent1->GetTotalItemCount(), 0);
  EXPECT_EQ(parent2->GetTotalItemCount(), 1);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Clearing the model.

TEST_F(ModelListenerTests, Clear)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  mock_model_listener_t listener(&m_model);

  {
    ::testing::InSequence seq;
    event_variant_t expected_event1 = ModelAboutToBeResetEvent{&m_model};
    event_variant_t expected_event2 = ModelResetEvent{&m_model};
    EXPECT_CALL(listener, OnEvent(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnEvent(expected_event2)).Times(1);
  }

  // removing item
  m_model.Clear({});
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Clearing the model.

TEST_F(ModelListenerTests, Destroy)
{
  auto model = std::make_unique<ApplicationModel>();

  auto parent = model->InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  model->InsertItem<PropertyItem>(parent);

  mock_model_listener_t listener(model.get());

  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(model.get())).Times(1);

  // triggering expectations
  model.reset();
}
