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

#include "mvvm/signals/model_listener.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionmodel.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class ModelListenerTests : public ::testing::Test
{
public:
  class TestListener : public ModelListener<ApplicationModel>
  {
  public:
    explicit TestListener(ApplicationModel* model) : ModelListener(model)
    {
      auto on_about_to_insert = [this](auto item, auto tagindex)
      { OnAboutToInsertItem(item, tagindex); };
      SetOnAboutToInsertItem(on_about_to_insert);

      auto on_item_inserted = [this](auto item, auto tagindex) { OnItemInserted(item, tagindex); };
      SetOnItemInserted(on_item_inserted);

      auto on_about_to_remove = [this](auto item, auto tagindex)
      { OnAboutToRemoveItem(item, tagindex); };
      SetOnAboutToRemoveItem(on_about_to_remove);

      auto on_item_removed = [this](auto item, auto tagindex) { OnItemRemoved(item, tagindex); };
      SetOnItemRemoved(on_item_removed);

      auto on_data_changed = [this](auto item, auto role) { OnDataChanged(item, role); };
      SetOnDataChanged(on_data_changed);

      auto on_model_about_reset = [this](auto model) { OnModelAboutToBeReset(model); };
      SetOnModelAboutToBeReset(on_model_about_reset);

      auto on_model_reset = [this](auto model) { OnModelReset(model); };
      SetOnModelReset(on_model_reset);

      auto on_model_about_destroyed = [this](auto model) { OnModelAboutToBeDestroyed(model); };
      SetOnModelAboutToBeDestroyed(on_model_about_destroyed);
    }

    MOCK_METHOD2(OnAboutToInsertItem,
                 void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

    MOCK_METHOD2(OnItemInserted, void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

    MOCK_METHOD2(OnAboutToRemoveItem,
                 void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

    MOCK_METHOD2(OnItemRemoved, void(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index));

    MOCK_METHOD2(OnDataChanged, void(mvvm::SessionItem* item, int role));

    MOCK_METHOD1(OnModelAboutToBeReset, void(mvvm::SessionModelInterface* model));

    MOCK_METHOD1(OnModelReset, void(mvvm::SessionModelInterface* model));

    MOCK_METHOD1(OnModelAboutToBeDestroyed, void(mvvm::SessionModelInterface* model));
  };

  ApplicationModel m_model;
  TagIndex m_tag_index;
};

//! Setting data through the model and checking the result.

TEST_F(ModelListenerTests, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();

  TestListener listener(&m_model);

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

TEST_F(ModelListenerTests, SetDataThroughItem)
{
  auto item = m_model.InsertItem<PropertyItem>();

  TestListener listener(&m_model);

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

TEST_F(ModelListenerTests, SetSameData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42, DataRole::kData);

  TestListener listener(&m_model);

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

//! Inserting item using templated insertion.

TEST_F(ModelListenerTests, InsertItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TestListener listener(&m_model);
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

TEST_F(ModelListenerTests, InsertItemInDefaultTag)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TestListener listener(&m_model);
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

//! Inserting item through the composer into another parent using move insertion.

TEST_F(ModelListenerTests, InsertItemViaMove)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);

  TestListener listener(&m_model);
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

TEST_F(ModelListenerTests, TakeItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  TestListener listener(&m_model);
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

TEST_F(ModelListenerTests, RemoveItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);

  TestListener listener(&m_model);
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

//! Removing item.

TEST_F(ModelListenerTests, MoveItem)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent1);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);

  TestListener listener(&m_model);
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

//! Clearing the model.

TEST_F(ModelListenerTests, Clear)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  TestListener listener(&m_model);

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

TEST_F(ModelListenerTests, Destroy)
{
  auto model = std::make_unique<ApplicationModel>();

  auto parent = model->InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  model->InsertItem<PropertyItem>(parent);

  TestListener listener(model.get());

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
