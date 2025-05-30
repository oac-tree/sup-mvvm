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

#include "mvvm/commands/command_stack.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/commands/default_command_processor.h>
#include <mvvm/commands/i_command.h>
#include <mvvm/commands/insert_item_command.h>
#include <mvvm/commands/remove_item_command.h>
#include <mvvm/commands/set_value_command.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>
#include <testutils/mock_command_stack.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

/**
 * @brief Tests for CommandStack with real stack and real commands.
 *
 * Commands are equipped with notifications.
 */
class CommandStackExtendedTests : public ::testing::Test
{
public:
  /**
   * @brief Creates a command and setup it up with notifications.
   */
  template <typename T, class... Args>
  auto CreateCommand(Args&&... args)
  {
    auto uptr = std::make_unique<T>(std::forward<Args>(args)...);
    auto raw_ptr = uptr.get();
    uptr->SetNotificationCallback(m_event_listener.AsStdFunction());
    return std::make_pair(std::move(uptr), raw_ptr);
  }

  ::testing::MockFunction<void(const event_variant_t&)> m_event_listener;
};

TEST_F(CommandStackExtendedTests, SetValueCommand)
{
  const int role = DataRole::kData;
  const variant_t initial_value(43);

  SessionItem item;
  item.SetData(initial_value, role);

  CommandStack stack;

  const variant_t new_value(44);
  auto [command, command_ptr] = CreateCommand<SetValueCommand>(&item, new_value, role);

  const auto expected_event = event_variant_t{DataChangedEvent{&item, role}};
  EXPECT_CALL(m_event_listener, Call(expected_event)).Times(1);
  stack.Execute(std::move(command));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item.Data(), new_value);

  EXPECT_CALL(m_event_listener, Call(expected_event)).Times(1);
  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(item.Data(), initial_value);

  // attempt to set the same initial data
  auto [command2, command2_ptr] = CreateCommand<SetValueCommand>(&item, initial_value, role);

  EXPECT_CALL(m_event_listener, Call(expected_event)).Times(0);
  stack.Execute(std::move(command2));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
}

TEST_F(CommandStackExtendedTests, InsertItemCommand)
{
  CommandStack stack;

  const TagIndex expected_tag_index{"tag1", 0};
  auto child = std::make_unique<CompoundItem>();
  auto child_ptr = child.get();
  auto child_identifier = child->GetIdentifier();
  auto grandchild = child->AddProperty("width", 42);
  auto grandchild_identifier = grandchild.GetIdentifier();

  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto [command, _] =
      CreateCommand<InsertItemCommand>(std::move(child), &parent, expected_tag_index);

  const auto expected_event1 = event_variant_t{AboutToInsertItemEvent{&parent, expected_tag_index}};
  const auto expected_event2 = event_variant_t{ItemInsertedEvent{&parent, expected_tag_index}};
  const auto expected_event3 = event_variant_t{AboutToRemoveItemEvent{&parent, expected_tag_index}};
  const auto expected_event4 = event_variant_t{ItemRemovedEvent{&parent, expected_tag_index}};

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }

  stack.Execute(std::move(command));

  ASSERT_EQ(parent.GetTotalItemCount(), 1);
  EXPECT_EQ(parent.GetAllItems().at(0), child_ptr);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_event_listener, Call(expected_event3)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event4)).Times(1);
  }

  stack.Undo();
  ASSERT_EQ(parent.GetTotalItemCount(), 0);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }

  stack.Redo();

  ASSERT_EQ(parent.GetTotalItemCount(), 1);
  auto restored_child = parent.GetAllItems().at(0);
  EXPECT_EQ(restored_child->GetIdentifier(), child_identifier);

  ASSERT_EQ(restored_child->GetTotalItemCount(), 1);
  auto restored_grandchild = restored_child->GetAllItems().at(0);
  EXPECT_EQ(restored_grandchild->GetIdentifier(), grandchild_identifier);
  EXPECT_EQ(restored_grandchild->GetDisplayName(), "width");
  EXPECT_EQ(restored_grandchild->Data<int>(), 42);
}

TEST_F(CommandStackExtendedTests, RemoveItemCommand)
{
  CommandStack stack;

  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  const TagIndex expected_tag_index{"tag1", 0};
  auto child = parent.InsertItem<SessionItem>(expected_tag_index);

  auto [command, _] = CreateCommand<RemoveItemCommand>(&parent, expected_tag_index);

  const auto expected_event1 = event_variant_t{AboutToRemoveItemEvent{&parent, expected_tag_index}};
  const auto expected_event2 = event_variant_t{ItemRemovedEvent{&parent, expected_tag_index}};
  const auto expected_event3 = event_variant_t{AboutToInsertItemEvent{&parent, expected_tag_index}};
  const auto expected_event4 = event_variant_t{ItemInsertedEvent{&parent, expected_tag_index}};

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }

  stack.Execute(std::move(command));
  ASSERT_EQ(parent.GetTotalItemCount(), 0);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_event_listener, Call(expected_event3)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event4)).Times(1);
  }
  stack.Undo();
  ASSERT_EQ(parent.GetTotalItemCount(), 1);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }

  stack.Redo();
  ASSERT_EQ(parent.GetTotalItemCount(), 0);
}

TEST_F(CommandStackExtendedTests, InsertSetValueRemoveUndoWhenInModel)
{
  const SessionModel model;
  CommandStack stack;
  auto parent = model.GetRootItem();

  // insert first parent
  auto [child, child_ptr] = test::CreateUnique<SessionItem>();
  child->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  const auto tag_index1 = TagIndex::First();

  auto [command1, command1_ptr] =
      CreateCommand<InsertItemCommand>(std::move(child), parent, tag_index1);
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = event_variant_t{AboutToInsertItemEvent{parent, tag_index1}};
    const auto expected_event2 = event_variant_t{ItemInsertedEvent{parent, tag_index1}};
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }
  stack.Execute(std::move(command1));
  EXPECT_EQ(parent->GetTotalItemCount(), 1);

  // insert property into parent
  auto [grandchild, grandchild_ptr] = test::CreateUnique<PropertyItem>();
  grandchild->SetData(int32{42});
  const auto tag_index2 = TagIndex::First();

  EXPECT_EQ(command1_ptr->GetResult(), child_ptr);

  auto [command2, command2_ptr] =
      CreateCommand<InsertItemCommand>(std::move(grandchild), child_ptr, tag_index1);
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = event_variant_t{AboutToInsertItemEvent{child_ptr, tag_index1}};
    const auto expected_event2 = event_variant_t{ItemInsertedEvent{child_ptr, tag_index1}};
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }
  stack.Execute(std::move(command2));
  EXPECT_EQ(child_ptr->GetTotalItemCount(), 1);

  // modifying value of grandchild
  auto [command3, command3_ptr] =
      CreateCommand<SetValueCommand>(grandchild_ptr, int32{43}, DataRole::kData);
  {
    const auto expected_event1 = event_variant_t{DataChangedEvent{grandchild_ptr, DataRole::kData}};
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  }
  stack.Execute(std::move(command3));
  EXPECT_EQ(child_ptr->GetTotalItemCount(), 1);
  EXPECT_EQ(grandchild_ptr->Data<int32>(), int32{43});
  EXPECT_TRUE(command3_ptr->GetResult());

  // removing parent
  auto [command4, command4_ptr] = CreateCommand<RemoveItemCommand>(parent, tag_index1);
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = event_variant_t{AboutToRemoveItemEvent{parent, tag_index1}};
    const auto expected_event2 = event_variant_t{ItemRemovedEvent{parent, tag_index1}};
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }
  stack.Execute(std::move(command4));
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  // undo removal
  {
    const ::testing::InSequence seq;
    const auto expected_event1 = event_variant_t{AboutToInsertItemEvent{parent, tag_index1}};
    const auto expected_event2 = event_variant_t{ItemInsertedEvent{parent, tag_index1}};
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
    EXPECT_CALL(m_event_listener, Call(expected_event2)).Times(1);
  }
  stack.Undo();

  auto restored_property = parent->GetAllItems().at(0)->GetAllItems().at(0);
  EXPECT_EQ(restored_property->Data<int32>(), 43);

  // undo value change
  {
    const auto expected_event1 =
        event_variant_t{DataChangedEvent{restored_property, DataRole::kData}};
    EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  }
  stack.Undo();

  // we were able to go back to grandchild original value after parent restore
  EXPECT_EQ(restored_property->Data<int32>(), 42);
}

//! Checking together with DefaultCommandProcessor, where we do not have undo/redo,
//! but still want to have notifications.
TEST_F(CommandStackExtendedTests, SetValueCommandWithDefaultProcessor)
{
  const int role = DataRole::kData;
  const variant_t initial_value(43);

  SessionItem item;
  item.SetData(initial_value, role);

  DefaultCommandProcessor stack;

  const variant_t new_value(44);
  auto [command1, command1_ptr] = CreateCommand<SetValueCommand>(&item, new_value, role);

  const auto expected_event1 = event_variant_t{DataChangedEvent{&item, role}};
  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  stack.Execute(std::move(command1));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item.Data(), new_value);

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(0);

  stack.Undo();
  EXPECT_EQ(item.Data(), new_value);
  stack.Redo();
  EXPECT_EQ(item.Data(), new_value);
}
