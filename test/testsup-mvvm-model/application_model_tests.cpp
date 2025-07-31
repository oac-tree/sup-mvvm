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

#include "mvvm/model/application_model.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/commands/default_command_processor.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_constants.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/test/mock_model_listener.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/toy_items.h>

namespace mvvm::test
{

/**
 * @brief Tests of ApplicationModel class.
 */
class ApplicationModelTest : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListener>;

  /**
   * @brief The TestModel class to validate clone mechanism.
   */
  class TestModel : public ApplicationModel
  {
  public:
    TestModel() : ApplicationModel("TestModel") {}

    std::unique_ptr<ISessionModel> Clone() const { return std::make_unique<TestModel>(*this); }
  };

  ApplicationModel m_model;
};

TEST_F(ApplicationModelTest, InitialState)
{
  EXPECT_TRUE(m_model.GetType().empty());
  EXPECT_EQ(m_model.GetRootItem()->GetModel(), &m_model);
  EXPECT_EQ(m_model.GetRootItem()->GetParent(), nullptr);
  EXPECT_NE(m_model.GetEventHandler(), nullptr);
  EXPECT_NE(m_model.GetCommandStack(), nullptr);
}

TEST_F(ApplicationModelTest, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();

  mock_listener_t listener(&m_model);

  const DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(1);

  // changing the data and checking result
  EXPECT_TRUE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, SetDataThroughItem)
{
  auto item = m_model.InsertItem<PropertyItem>();

  mock_listener_t listener(&m_model);

  const DataChangedEvent expected_event{item, DataRole::kData};
  EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(1);

  // changing the data through the item (should still trigger notifications through the model)
  EXPECT_TRUE(item->SetData(42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, SetSameData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42, DataRole::kData);

  mock_listener_t listener(&m_model);

  // Expecting no signals. StrictMock will fail if it is not the case.

  // changing to the same value
  EXPECT_FALSE(m_model.SetData(item, 42, DataRole::kData));
  EXPECT_EQ(item->Data<int>(), 42);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, InsertItemIntoRoot)
{
  auto parent = m_model.GetRootItem();
  const TagIndex tag_index{constants::kRootItemTag, 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToInsertItemEvent expected_event1{parent, tag_index};
    const ItemInsertedEvent expected_event2{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  // inserting item into the root
  auto item = m_model.InsertItem<SessionItem>();
  EXPECT_EQ(item, m_model.GetRootItem()->GetAllItems()[0]);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, InsertItemIntoRootViaMove)
{
  auto parent = m_model.GetRootItem();
  const TagIndex tag_index{constants::kRootItemTag, 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToInsertItemEvent expected_event1{parent, tag_index};
    const ItemInsertedEvent expected_event2{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  // inserting item into the root
  auto item = std::make_unique<PropertyItem>();
  auto item_ptr = item.get();

  auto inserted = m_model.InsertItem(std::move(item), m_model.GetRootItem(), TagIndex::First());
  EXPECT_EQ(item_ptr, m_model.GetRootItem()->GetAllItems()[0]);
  EXPECT_TRUE(item == nullptr);
  EXPECT_EQ(inserted, item_ptr);
  EXPECT_EQ(inserted->GetParent(), m_model.GetRootItem());
  EXPECT_EQ(inserted->GetModel(), &m_model);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, InsertItemIntoParentUsingTagAndIndex)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), false);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToInsertItemEvent expected_event1{parent, tag_index};
    const ItemInsertedEvent expected_event2{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, InsertItemIntoParentUsingTagAndIndexViaGetModel)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), false);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToInsertItemEvent expected_event1{parent, tag_index};
    const ItemInsertedEvent expected_event2{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  // inserting item (pretending that we do not have direct access to the model)
  auto item = parent->GetModel()->InsertItem<PropertyItem>(parent, {"tag", 0});
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion via defaut tag (real-life bug).
TEST_F(ApplicationModelTest, InsertItemInDefaultTag)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToInsertItemEvent expected_event1{parent, tag_index};
    const ItemInsertedEvent expected_event2{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  // inserting item
  auto item = m_model.InsertItem<PropertyItem>(parent);
  EXPECT_EQ(item, parent->GetItem("tag"));

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

//! Inserting item using templated insertion.
//! Using defaut tag (real-life bug) when where is no default tag defined.
TEST_F(ApplicationModelTest, InsertItemInDefaultTagWhenNoDefaultIsPresent)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), false);

  mock_listener_t listener(&m_model);

  // Expecting no signals. StrictMock will fail if it is not the case.

  // inserting item
  EXPECT_THROW(m_model.InsertItem<PropertyItem>(parent), InvalidOperationException);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, InsertItemInPropertyTag)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->AddProperty("thickness", 42);

  mock_listener_t listener(&m_model);

  // Expecting no signals. StrictMock will fail if it is not the case.

  // It shouldn't be allowed to insert another item in the already existing property tag
  EXPECT_THROW(m_model.InsertItem<PropertyItem>(parent, {"thickness", 0}),
               InvalidOperationException);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, InsertItemViaMove)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToInsertItemEvent expected_event1{parent, tag_index};
    const ItemInsertedEvent expected_event2{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
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

TEST_F(ApplicationModelTest, TakeItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;
    const AboutToRemoveItemEvent expected_event1{parent, tag_index};
    const ItemRemovedEvent expected_event2{parent, tag_index};
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }

  // removing item
  auto taken = m_model.TakeItem(parent, {"tag", 0});
  EXPECT_EQ(taken.get(), child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, RemoveItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;
    const AboutToRemoveItemEvent expected_event1{parent, tag_index};
    const ItemRemovedEvent expected_event2{parent, tag_index};
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }

  // removing item
  m_model.RemoveItem(child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, RemoveItemUsingHelperMethod)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;
    const AboutToRemoveItemEvent expected_event1{parent, tag_index};
    const ItemRemovedEvent expected_event2{parent, tag_index};
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }

  // removing item
  utils::RemoveItem(*child);
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, ReplaceItemUsingHelperMethod)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent);
  const TagIndex tag_index{"tag", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToRemoveItemEvent expected_event1{parent, tag_index};
    const ItemRemovedEvent expected_event2{parent, tag_index};
    const AboutToInsertItemEvent expected_event3{parent, tag_index};
    const ItemInsertedEvent expected_event4{parent, tag_index};

    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event3)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event4)).Times(1);
  }

  // inserting item (pretending that we do not have direct access to the model)
  auto new_child = std::make_unique<PropertyItem>();
  auto new_child_ptr = new_child.get();

  auto item = utils::ReplaceItem(std::move(new_child), parent, tag_index);

  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(item, parent->GetItem("tag"));
  EXPECT_EQ(item, new_child_ptr);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, MoveItem)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  parent1->RegisterTag(TagInfo::CreateUniversalTag("tag1"), true);
  auto child = m_model.InsertItem<PropertyItem>(parent1);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);
  const TagIndex tag_index1{"tag1", 0};
  const TagIndex tag_index2{"tag2", 0};

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;

    const AboutToRemoveItemEvent expected_event1{parent1, tag_index1};
    const ItemRemovedEvent expected_event2{parent1, tag_index1};
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);

    const AboutToInsertItemEvent expected_event3{parent2, tag_index2};
    const ItemInsertedEvent expected_event4{parent2, tag_index2};
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event3)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event4)).Times(1);
  }

  // removing item
  m_model.MoveItem(child, parent2, tag_index2);
  EXPECT_EQ(parent1->GetTotalItemCount(), 0);
  EXPECT_EQ(parent2->GetTotalItemCount(), 1);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, AttemptToMovePropertyItemFromCompoundItem)
{
  auto parent1 = m_model.InsertItem<CompoundItem>();
  auto& property = parent1->AddProperty("thickness", 42);
  auto parent2 = m_model.InsertItem<CompoundItem>();
  parent2->RegisterTag(TagInfo::CreateUniversalTag("tag2"), true);

  mock_listener_t listener(&m_model);

  // Expecting no signals. StrictMock will fail if it is not the case.

  // removing item
  EXPECT_THROW(m_model.MoveItem(&property, parent2, {"tag2", 0}), InvalidOperationException);
  EXPECT_EQ(parent1->GetTotalItemCount(), 1);
  EXPECT_EQ(parent2->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, Clear)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;
    const ModelAboutToBeResetEvent expected_event1{&m_model};
    const ModelResetEvent expected_event2{&m_model};
    EXPECT_CALL(listener, OnModelAboutToBeReset(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnModelReset(expected_event2)).Times(1);
  }

  // removing item
  m_model.Clear();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, ReplaceRootItem)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  m_model.InsertItem<PropertyItem>(parent);

  auto new_root_item = utils::CreateEmptyRootItem();
  auto new_root_item_ptr = new_root_item.get();

  mock_listener_t listener(&m_model);

  {
    const ::testing::InSequence seq;
    const ModelAboutToBeResetEvent expected_event1{&m_model};
    const ModelResetEvent expected_event2{&m_model};
    EXPECT_CALL(listener, OnModelAboutToBeReset(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnModelReset(expected_event2)).Times(1);
  }

  m_model.ReplaceRootItem(std::move(new_root_item));
  EXPECT_EQ(m_model.GetRootItem(), new_root_item_ptr);

  // verify here, and not on MockModelListener destruction (to mute OnModelAboutToBeDestroyed)
  testing::Mock::VerifyAndClearExpectations(&listener);
}

TEST_F(ApplicationModelTest, Destroy)
{
  auto model = std::make_unique<ApplicationModel>();

  auto parent = model->InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag"), true);
  model->InsertItem<PropertyItem>(parent);

  mock_listener_t listener(model.get());

  const ModelAboutToBeDestroyedEvent expected_event{model.get()};
  EXPECT_CALL(listener, OnModelAboutToBeDestroyed(expected_event)).Times(1);

  // triggering expectations
  model.reset();
}

TEST_F(ApplicationModelTest, SetUndoEnabled)
{
  EXPECT_NE(dynamic_cast<DefaultCommandProcessor*>(m_model.GetCommandStack()), nullptr);

  m_model.SetUndoEnabled(true);
  EXPECT_NE(dynamic_cast<CommandStack*>(m_model.GetCommandStack()), nullptr);

  m_model.SetUndoEnabled(false);
  EXPECT_NE(dynamic_cast<DefaultCommandProcessor*>(m_model.GetCommandStack()), nullptr);
}

TEST_F(ApplicationModelTest, Clone)
{
  const std::string model_type("model_type");
  const ApplicationModel model(model_type);

  auto clone = model.Clone();
  EXPECT_EQ(clone->GetType(), model_type);
  EXPECT_NE(clone->GetRootItem(), model.GetRootItem());
  ASSERT_NE(clone->GetEventHandler(), nullptr);
  // EXPECT_EQ(clone->GetRootItem()->GetIdentifier(), model.GetRootItem()->GetIdentifier());
  EXPECT_NE(dynamic_cast<ApplicationModel*>(clone.get()), nullptr);
}

TEST_F(ApplicationModelTest, CloneForTestModel)
{
  TestModel model;
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42);

  auto clone = model.Clone();
  EXPECT_EQ(clone->GetType(), model.GetType());
  EXPECT_NE(clone->GetRootItem(), model.GetRootItem());
  ASSERT_NE(clone->GetEventHandler(), nullptr);
  // EXPECT_EQ(clone->GetRootItem()->GetIdentifier(), model.GetRootItem()->GetIdentifier());
  EXPECT_NE(dynamic_cast<ApplicationModel*>(clone.get()), nullptr);
  ASSERT_EQ(clone->GetRootItem()->GetAllItems().size(), 1U);
  EXPECT_EQ(clone->GetRootItem()->GetAllItems().at(0)->Data<std::int32_t>(), 42);
}

}  // namespace mvvm::test
