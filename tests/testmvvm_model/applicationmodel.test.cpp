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

#include "mvvm/model/applicationmodel.h"

#include "mockmodellistener.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Tests for ModelComposer class.
//! Class is used to manipulate the model and generate necessary notifications.

class ApplicationModelTest : public ::testing::Test
{
public:
  ApplicationModel m_model;
  TagIndex m_tag_index;
};

//! Setting data through the model and checking the result.

TEST_F(ApplicationModelTest, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();

  MockModelListener listener(&m_model);

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(item, DataRole::kData)).Times(1);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // changing the data and checking result
  EXPECT_TRUE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Setting data through the item.

TEST_F(ApplicationModelTest, SetDataThroughItem)
{
  auto item = m_model.InsertItem<PropertyItem>();

  MockModelListener listener(&m_model);

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(item, DataRole::kData)).Times(1);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // changing the data through the item (should still trigger notifications through the model)
  EXPECT_TRUE(item->SetData(42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Setting same data through the composer and checking the result.
//! No notifications are expected.

TEST_F(ApplicationModelTest, SetSameData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42, DataRole::kData);

  MockModelListener listener(&m_model);

  // no notifications are expected
  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(item, DataRole::kData)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // changing to the same value
  EXPECT_FALSE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting new item into the root item through the composer.

TEST_F(ApplicationModelTest, InsertNewItem)
{
  SessionItem* expected_parent = m_model.GetRootItem();
  TagIndex expected_tag_index{"rootTag", 0};  // default tag of root item

  MockModelListener listener(&m_model);

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // inserting item into the root
  auto item = m_model.InsertNewItem(PropertyItem::Type, nullptr, {"", -1});
  EXPECT_EQ(item, m_model.GetRootItem()->GetAllItems()[0]);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting new item through the composer into another parent.

TEST_F(ApplicationModelTest, InsertNewItemIntoParent)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  MockModelListener listener(&m_model);

  TagIndex expected_tag_index{"tag", 0};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // inserting item
  auto item = m_model.InsertNewItem(PropertyItem::Type, parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion.

TEST_F(ApplicationModelTest, InsertItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  MockModelListener listener(&m_model);
  TagIndex expected_tag_index{"tag", 0};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion.
//! Using defaut tag (real-life bug).

TEST_F(ApplicationModelTest, InsertItemInDefaultTag)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  MockModelListener listener(&m_model);
  TagIndex expected_tag_index{"tag", 0};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent);
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion.
//! Using defaut tag (real-life bug) when where is no default tag defined.

//TEST_F(ApplicationModelTest, InsertItemInDefaultTagWhenNoDefaultIsPresent)
//{
//  auto parent = m_model.InsertItem<CompoundItem>();
//  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), false);

//  MockModelListener listener(&m_model);
//  TagIndex expected_tag_index{"tag", 0};

//  {
//    ::testing::InSequence seq;
//    EXPECT_CALL(listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
//    EXPECT_CALL(listener, OnItemInserted(parent, expected_tag_index)).Times(1);
//  }
//  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
//  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
//  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
//  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

//  // inserting item
//  auto item = m_model.InsertItem<PropertyItem>(parent);
//  EXPECT_EQ(item, parent->GetItem("tag"));

//  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
//  testing::Mock::VerifyAndClearExpectations(&listener);
//}


//! Inserting item through the composer into another parent using move insertion.

TEST_F(ApplicationModelTest, InsertItemViaMove)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  MockModelListener listener(&m_model);
  TagIndex expected_tag_index{"tag", 0};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

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

TEST_F(ApplicationModelTest, TakeItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  MockModelListener listener(&m_model);
  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToRemoveItem(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // removing item
  auto taken = m_model.TakeItem(parent, {"tag", 0});
  EXPECT_EQ(taken.get(), child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Removing item.

TEST_F(ApplicationModelTest, RemoveItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  MockModelListener listener(&m_model);
  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToRemoveItem(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // removing item
  m_model.RemoveItem(child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Moving item item.

TEST_F(ApplicationModelTest, MoveItem)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent1);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);

  MockModelListener listener(&m_model);
  TagIndex expected_tag_index1{"tag1", 0};
  TagIndex expected_tag_index2{"tag2", 0};

  EXPECT_CALL(listener, OnAboutToInsertItem(parent2, expected_tag_index2)).Times(1);
  EXPECT_CALL(listener, OnItemInserted(parent2, expected_tag_index2)).Times(1);
  EXPECT_CALL(listener, OnAboutToRemoveItem(parent1, expected_tag_index1)).Times(1);
  EXPECT_CALL(listener, OnItemRemoved(parent1, expected_tag_index1)).Times(1);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // removing item
  m_model.MoveItem(child, parent2, expected_tag_index2);
  EXPECT_EQ(parent1->GetTotalItemCount(), 0);
  EXPECT_EQ(parent2->GetTotalItemCount(), 1);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Attempt to move property item from compound item.
//! The operation should fail via exception throw, no signals should be emitted.

TEST_F(ApplicationModelTest, IvalidItemMove)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  auto property = parent1->AddProperty("thickness", 42);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);

  MockModelListener listener(&m_model);

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // removing item
  EXPECT_THROW(m_model.MoveItem(property, parent2, {"tag2", 0}), InvalidMoveException);
  EXPECT_EQ(parent1->GetTotalItemCount(), 1);
  EXPECT_EQ(parent2->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListenerr destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Clearing the model.

TEST_F(ApplicationModelTest, Clear)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  MockModelListener listener(&m_model);

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(&m_model)).Times(1);
  EXPECT_CALL(listener, OnModelReset(&m_model)).Times(1);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(_)).Times(0);

  // removing item
  m_model.Clear();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Clearing the model.

TEST_F(ApplicationModelTest, Destroy)
{
  auto model = std::make_unique<ApplicationModel>();

  auto parent = model->InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  model->InsertItem<PropertyItem>(parent);

  MockModelListener listener(model.get());

  EXPECT_CALL(listener, OnAboutToInsertItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemInserted(_, _)).Times(0);
  EXPECT_CALL(listener, OnAboutToRemoveItem(_, _)).Times(0);
  EXPECT_CALL(listener, OnItemRemoved(_, _)).Times(0);
  EXPECT_CALL(listener, OnDataChanged(_, _)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelReset(_)).Times(0);
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(model.get())).Times(1);

  // triggering expectations
  model.reset();
}
