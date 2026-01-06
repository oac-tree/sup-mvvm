/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/commands/command_stack.h"

#include <mvvm/commands/default_command_processor.h>
#include <mvvm/commands/insert_item_command.h>
#include <mvvm/commands/remove_item_command.h>
#include <mvvm/commands/set_value_command.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/test/mock_model_listener.h>
#include <mvvm/test/test_container_helper.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for CommandStack with real stack and real commands.
 *
 * Commands operate on ApplicationModel, notifications are expected.
 */
class CommandStackExtendedNotifyingTests : public ::testing::Test
{
public:
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockModelListener>;
  ApplicationModel m_model;
};

TEST_F(CommandStackExtendedNotifyingTests, SetValueCommand)
{
  const role_t role = DataRole::kData;
  const variant_t initial_value(43);

  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(initial_value, role);

  mock_listener_t listener(&m_model);
  CommandStack stack;

  const variant_t new_value(44);
  auto [command, command_ptr] = CreateUnique<SetValueCommand>(item, new_value, role);

  const auto expected_event = DataChangedEvent{item, role};
  EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(1);
  stack.Execute(std::move(command));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item->Data(), new_value);

  EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(1);
  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(item->Data(), initial_value);

  // attempt to set the same initial data
  auto [command2, command2_ptr] = CreateUnique<SetValueCommand>(item, initial_value, role);

  EXPECT_CALL(listener, OnDataChanged(expected_event)).Times(0);
  stack.Execute(std::move(command2));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
}

TEST_F(CommandStackExtendedNotifyingTests, InsertItemCommand)
{
  CommandStack stack;

  const TagIndex expected_tag_index{"tag1", 0};
  auto child = std::make_unique<CompoundItem>();
  auto child_ptr = child.get();
  auto child_identifier = child->GetIdentifier();
  auto grandchild = child->AddProperty("width", 42);
  auto grandchild_identifier = grandchild.GetIdentifier();

  auto parent = m_model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  mock_listener_t listener(&m_model);

  auto [command, _] = CreateUnique<InsertItemCommand>(std::move(child), parent, expected_tag_index);

  const auto expected_event1 = AboutToInsertItemEvent{parent, expected_tag_index};
  const auto expected_event2 = ItemInsertedEvent{parent, expected_tag_index};
  const auto expected_event3 = AboutToRemoveItemEvent{parent, expected_tag_index};
  const auto expected_event4 = ItemRemovedEvent{parent, expected_tag_index};

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  stack.Execute(std::move(command));

  ASSERT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(parent->GetAllItems().at(0), child_ptr);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event3)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event4)).Times(1);
  }

  stack.Undo();
  ASSERT_EQ(parent->GetTotalItemCount(), 0);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }

  stack.Redo();

  ASSERT_EQ(parent->GetTotalItemCount(), 1);
  auto restored_child = parent->GetAllItems().at(0);
  EXPECT_EQ(restored_child->GetIdentifier(), child_identifier);

  ASSERT_EQ(restored_child->GetTotalItemCount(), 1);
  auto restored_grandchild = restored_child->GetAllItems().at(0);
  EXPECT_EQ(restored_grandchild->GetIdentifier(), grandchild_identifier);
  EXPECT_EQ(restored_grandchild->GetDisplayName(), "width");
  EXPECT_EQ(restored_grandchild->Data<int>(), 42);
}

TEST_F(CommandStackExtendedNotifyingTests, RemoveItemCommand)
{
  CommandStack stack;

  auto parent = m_model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  const TagIndex expected_tag_index{"tag1", 0};
  auto child = parent->InsertItem<SessionItem>(expected_tag_index);

  mock_listener_t listener(&m_model);

  auto [command, _] = CreateUnique<RemoveItemCommand>(parent, expected_tag_index);

  const auto expected_event1 = AboutToRemoveItemEvent{parent, expected_tag_index};
  const auto expected_event2 = ItemRemovedEvent{parent, expected_tag_index};
  const auto expected_event3 = AboutToInsertItemEvent{parent, expected_tag_index};
  const auto expected_event4 = ItemInsertedEvent{parent, expected_tag_index};

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }

  stack.Execute(std::move(command));
  ASSERT_EQ(parent->GetTotalItemCount(), 0);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event3)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event4)).Times(1);
  }
  stack.Undo();
  ASSERT_EQ(parent->GetTotalItemCount(), 1);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }

  stack.Redo();
  ASSERT_EQ(parent->GetTotalItemCount(), 0);
}

TEST_F(CommandStackExtendedNotifyingTests, InsertSetValueRemove)
{
  CommandStack stack;
  auto parent = m_model.GetRootItem();

  mock_listener_t listener(&m_model);

  // insert first parent
  auto [child, child_ptr] = test::CreateUnique<SessionItem>();
  child->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  const auto tag_index1 = TagIndex::First();

  auto [command1, command1_ptr] =
      CreateUnique<InsertItemCommand>(std::move(child), parent, tag_index1);
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = AboutToInsertItemEvent{parent, tag_index1};
    const auto expected_event2 = ItemInsertedEvent{parent, tag_index1};
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }
  stack.Execute(std::move(command1));
  EXPECT_EQ(parent->GetTotalItemCount(), 1);

  // insert property into parent
  auto [grandchild, grandchild_ptr] = test::CreateUnique<PropertyItem>();
  grandchild->SetData(int32{42});
  const auto tag_index2 = TagIndex::First();

  EXPECT_EQ(command1_ptr->GetResult(), child_ptr);

  auto [command2, command2_ptr] =
      CreateUnique<InsertItemCommand>(std::move(grandchild), child_ptr, tag_index1);
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = AboutToInsertItemEvent{child_ptr, tag_index1};
    const auto expected_event2 = ItemInsertedEvent{child_ptr, tag_index1};
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }
  stack.Execute(std::move(command2));
  EXPECT_EQ(child_ptr->GetTotalItemCount(), 1);

  // modifying value of grandchild
  auto [command3, command3_ptr] =
      CreateUnique<SetValueCommand>(grandchild_ptr, int32{43}, DataRole::kData);
  {
    const auto expected_event1 = DataChangedEvent{grandchild_ptr, DataRole::kData};
    EXPECT_CALL(listener, OnDataChanged(expected_event1)).Times(1);
  }
  stack.Execute(std::move(command3));
  EXPECT_EQ(child_ptr->GetTotalItemCount(), 1);
  EXPECT_EQ(grandchild_ptr->Data<int32>(), int32{43});
  EXPECT_TRUE(command3_ptr->GetResult());

  // removing parent
  auto [command4, command4_ptr] = CreateUnique<RemoveItemCommand>(parent, tag_index1);
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = AboutToRemoveItemEvent{parent, tag_index1};
    const auto expected_event2 = ItemRemovedEvent{parent, tag_index1};
    EXPECT_CALL(listener, OnAboutToRemoveItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemRemoved(expected_event2)).Times(1);
  }
  stack.Execute(std::move(command4));
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // undo removal
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = AboutToInsertItemEvent{parent, tag_index1};
    const auto expected_event2 = ItemInsertedEvent{parent, tag_index1};
    EXPECT_CALL(listener, OnAboutToInsertItem(expected_event1)).Times(1);
    EXPECT_CALL(listener, OnItemInserted(expected_event2)).Times(1);
  }
  stack.Undo();

  auto restored_property = parent->GetAllItems().at(0)->GetAllItems().at(0);
  EXPECT_EQ(restored_property->Data<int32>(), 43);

  // undo value change
  {
    const auto expected_event1 = DataChangedEvent{restored_property, DataRole::kData};
    EXPECT_CALL(listener, OnDataChanged(expected_event1)).Times(1);
  }
  stack.Undo();

  // we were able to go back to grandchild original value after parent restore
  EXPECT_EQ(restored_property->Data<int32>(), 42);
}

//! Checking together with DefaultCommandProcessor, where we do not have undo/redo,
//! but still want to have notifications.
TEST_F(CommandStackExtendedNotifyingTests, SetValueCommandWithDefaultProcessor)
{
  const role_t role = DataRole::kData;
  const variant_t initial_value(43);

  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(initial_value, role);
  mock_listener_t listener(&m_model);

  DefaultCommandProcessor stack;

  const variant_t new_value(44);
  auto [command1, command1_ptr] = CreateUnique<SetValueCommand>(item, new_value, role);

  const auto expected_event1 = DataChangedEvent{item, role};
  EXPECT_CALL(listener, OnDataChanged(expected_event1)).Times(1);
  stack.Execute(std::move(command1));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item->Data(), new_value);

  EXPECT_CALL(listener, OnDataChanged(expected_event1)).Times(0);

  stack.Undo();
  EXPECT_EQ(item->Data(), new_value);
  stack.Redo();
  EXPECT_EQ(item->Data(), new_value);
}

}  // namespace mvvm::test
