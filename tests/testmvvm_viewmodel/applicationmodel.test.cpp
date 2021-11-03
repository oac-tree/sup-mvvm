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

#include "mvvm/viewmodel/applicationmodel.h"

#include "mockmodellistener.h"

#include "mvvm/model/compounditem.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace ModelView;
using ::testing::_;

//! Tests for ModelComposer class.
//! Class is used to manipulate the model and generate necessary notifications.

class ApplicationModelTest : public ::testing::Test
{
public:
  ApplicationModelTest() { m_model.Subscribe(&m_listener); }

  MockModelListener m_listener;
  ApplicationModel m_model;
  TagIndex m_tag_index;
};

//! Setting data through the model and checking the result.

TEST_F(ApplicationModelTest, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(item, DataRole::kData)).Times(1);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // changing the data and checking result
  EXPECT_TRUE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);
}

//! Setting data through the item.

TEST_F(ApplicationModelTest, SetDataThroughItem)
{
  auto item = m_model.InsertItem<PropertyItem>();

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(item, DataRole::kData)).Times(1);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // changing the data through the item (should still trigger notifications through the model)
  EXPECT_TRUE(item->SetData(42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);
}

//! Setting same data through the composer and checking the result.
//! No notifications are expected.

TEST_F(ApplicationModelTest, SetSameData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42, DataRole::kData);

  // no notifications are expected
  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(item, DataRole::kData)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // changing to the same value
  EXPECT_FALSE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);
}

//! Inserting new item into the root item through the composer.

TEST_F(ApplicationModelTest, InsertNewItem)
{
  SessionItem* expected_parent = m_model.GetRootItem();
  TagIndex expected_tag_index{"", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(expected_parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemInserted(expected_parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // inserting item into the root
  auto item = m_model.InsertNewItem(PropertyItem::Type, nullptr, {"", -1});
  EXPECT_EQ(item, m_model.GetRootItem()->GetAllItems()[0]);
}

//! Inserting new item through the composer into another parent.

TEST_F(ApplicationModelTest, InsertNewItemIntoParent)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // inserting item
  auto item = m_model.InsertNewItem(PropertyItem::Type, parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));
}

//! Inserting item using templated insertion.

TEST_F(ApplicationModelTest, InsertItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));
}

//! Inserting item through the composer into another parent using move insertion.

TEST_F(ApplicationModelTest, InsertItemViaMove)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // inserting item
  auto to_insert = std::make_unique<PropertyItem>();
  auto to_insert_ptr = to_insert.get();
  auto item = m_model.InsertItem(std::move(to_insert), parent, {"tag", 0});
  EXPECT_EQ(item, to_insert_ptr);
  EXPECT_EQ(item, parent->GetItem("tag"));
}

//! Removing item.

TEST_F(ApplicationModelTest, TakeItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemRemoved(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // removing item
  auto taken = m_model.TakeItem(parent, {"tag", 0});
  EXPECT_EQ(taken.get(), child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);
}

//! Removing item.

TEST_F(ApplicationModelTest, RemoveItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnItemRemoved(parent, expected_tag_index)).Times(1);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // removing item
  m_model.RemoveItem(child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);
}

//! Removing item.

TEST_F(ApplicationModelTest, MoveItem)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent1);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);

  TagIndex expected_tag_index1{"tag1", 0};
  TagIndex expected_tag_index2{"tag2", 0};

  EXPECT_CALL(m_listener, OnAboutToInsertItem(parent2, expected_tag_index2)).Times(1);
  EXPECT_CALL(m_listener, OnItemInserted(parent2, expected_tag_index2)).Times(1);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(parent1, expected_tag_index1)).Times(1);
  EXPECT_CALL(m_listener, OnItemRemoved(parent1, expected_tag_index1)).Times(1);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(m_listener, OnModelReset(_)).Times(0);

  // removing item
  m_model.MoveItem(child, parent2, expected_tag_index2);
  EXPECT_EQ(parent1->GetTotalItemCount(), 0);
  EXPECT_EQ(parent2->GetTotalItemCount(), 1);
}

//! Clearing the model.

TEST_F(ApplicationModelTest, Clear)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  EXPECT_CALL(m_listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(m_listener, OnModelAboutToBeReset(&m_model)).Times(1);
  EXPECT_CALL(m_listener, OnModelReset(&m_model)).Times(1);

  // removing item
  m_model.Clear();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
}
