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

#include "mvvm/model/modelcomposer.h"

#include "mockmodelnotifier.h"

#include "mvvm/model/compounditem.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Tests for ModelComposer class.
//! Class is used to manipulate the model and generate necessary notifications.
//! FIXME tests duplicates tests from applicationmodel.test.cpp. Remove them after
//! the removal of ModelComposer itself.

class ModelComposerTest : public ::testing::Test
{
public:
  ModelComposerTest() : m_composer(&m_model, &m_notifier) {}

  SessionModel m_model;
  MockModelNotifier m_notifier;
  ModelComposer m_composer;
  TagIndex m_tag_index;
};

//! Setting data through the composer and checking the result.

TEST_F(ModelComposerTest, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();

  EXPECT_CALL(m_notifier, AboutToInsertItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemInsertedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, DataChangedNotify(item, DataRole::kData)).Times(1);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // changing the data and checking result
  EXPECT_TRUE(m_composer.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);
}

//! Setting same data through the composer and checking the result.
//! No notifications are expected.

TEST_F(ModelComposerTest, SetSameData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42, DataRole::kData);

  // no notifications are expected
  EXPECT_CALL(m_notifier, AboutToInsertItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemInsertedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, DataChangedNotify(item, DataRole::kData)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // changing to the same value
  EXPECT_FALSE(m_composer.SetData(item, 42, DataRole::kData));
}

//! Inserting new item into the root item through the composer.
// FIXME remove commented code

//TEST_F(ModelComposerTest, InsertNewItem)
//{
//  SessionItem* expected_parent = m_model.GetRootItem();
//  TagIndex expected_tag_index{"rootTag", 0};

//  {
//    ::testing::InSequence seq;
//    EXPECT_CALL(m_notifier, AboutToInsertItemNotify(expected_parent, expected_tag_index)).Times(1);
//    EXPECT_CALL(m_notifier, ItemInsertedNotify(expected_parent, expected_tag_index)).Times(1);
//  }
//  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
//  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
//  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
//  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
//  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
//  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

//  // inserting item into the root
//  auto item = m_composer.InsertNewItem(PropertyItem::Type, nullptr, {"", -1});
//  EXPECT_EQ(item, m_model.GetRootItem()->GetAllItems()[0]);
//}

////! Inserting new item through the composer into another parent.

//TEST_F(ModelComposerTest, InsertNewItemIntoParent)
//{
//  auto parent = m_model.InsertItem<CompoundItem>();
//  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

//  TagIndex expected_tag_index{"tag", 0};

//  {
//    ::testing::InSequence seq;
//    EXPECT_CALL(m_notifier, AboutToInsertItemNotify(parent, expected_tag_index)).Times(1);
//    EXPECT_CALL(m_notifier, ItemInsertedNotify(parent, expected_tag_index)).Times(1);
//  }
//  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
//  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
//  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
//  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
//  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
//  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

//  // inserting item
//  auto item = m_composer.InsertNewItem(PropertyItem::Type, parent, {"tag", 0});
//  EXPECT_EQ(item, parent->GetItem("tag"));
//}

//! Inserting item through the composer into another parent using templated insertion.

TEST_F(ModelComposerTest, InsertItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TagIndex expected_tag_index{"tag", 0};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_notifier, AboutToInsertItemNotify(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(m_notifier, ItemInsertedNotify(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // inserting item
  auto item = m_composer.InsertItem<PropertyItem>(parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));
}

//! Inserting item through the composer into another parent using move insertion.

TEST_F(ModelComposerTest, InsertItemViaMove)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TagIndex expected_tag_index{"tag", 0};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_notifier, AboutToInsertItemNotify(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(m_notifier, ItemInsertedNotify(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // inserting item
  auto to_insert = std::make_unique<PropertyItem>();
  auto to_insert_ptr = to_insert.get();
  auto item = m_composer.InsertItem(std::move(to_insert), parent, {"tag", 0});
  EXPECT_EQ(item, to_insert_ptr);
  EXPECT_EQ(item, parent->GetItem("tag"));
}

//! Take item through the composer.

TEST_F(ModelComposerTest, TakeItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_notifier, AboutToInsertItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemInsertedNotify(_, _)).Times(0);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(m_notifier, ItemRemovedNotify(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // removing item
  m_composer.TakeItem(parent, {"tag", 0});
}

//! Removing item through the composer.

TEST_F(ModelComposerTest, RemoveItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_notifier, AboutToInsertItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemInsertedNotify(_, _)).Times(0);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(parent, expected_tag_index)).Times(1);
    EXPECT_CALL(m_notifier, ItemRemovedNotify(parent, expected_tag_index)).Times(1);
  }
  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelResetNotify(_)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // removing item
  m_composer.RemoveItem(child);
}

//! Clearing the model.

TEST_F(ModelComposerTest, Clear)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  TagIndex expected_tag_index{"tag", 0};

  EXPECT_CALL(m_notifier, AboutToInsertItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemInsertedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, AboutToRemoveItemNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ItemRemovedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
  EXPECT_CALL(m_notifier, ModelAboutToBeResetNotify(&m_model)).Times(1);
  EXPECT_CALL(m_notifier, ModelResetNotify(&m_model)).Times(1);
  EXPECT_CALL(m_notifier, ModelAboutToBeDestroyedNotify(_)).Times(0);

  // removing item
  m_composer.Clear({});
}
