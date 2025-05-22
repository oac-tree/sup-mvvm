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

#include "mvvm/commands/notifying_command_stack.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/commands/i_command.h>
#include <mvvm/commands/insert_item_command_v2.h>
#include <mvvm/commands/set_value_command_v2.h>
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
 * @brief Tests for NotifyingCommandStack with real stack and real commands.
 */
class NotifyingCommandStackExtendedTests : public ::testing::Test
{
public:
  ::testing::MockFunction<void(const event_variant_t&)> m_event_listener;
};

TEST_F(NotifyingCommandStackExtendedTests, SetValueCommand)
{
  const int role = DataRole::kData;
  const variant_t initial_value(43);

  SessionItem item;
  item.SetData(initial_value, role);

  NotifyingCommandStack<CommandStack> stack(m_event_listener.AsStdFunction());

  const variant_t new_value1(44);
  auto [command1, command1_ptr] = test::CreateUnique<SetValueCommandV2>(&item, new_value1, role);

  const auto expected_event1 = event_variant_t{DataChangedEvent{&item, role}};
  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  stack.Execute(std::move(command1));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item.Data(), new_value1);

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(item.Data(), initial_value);

  // attempt to set the same initial data
  auto [command2, command2_ptr] =
      test::CreateUnique<SetValueCommandV2>(&item, initial_value, role);

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(0);
  stack.Execute(std::move(command2));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
}

TEST_F(NotifyingCommandStackExtendedTests, InsertItemCommand)
{
  NotifyingCommandStack<CommandStack> stack(m_event_listener.AsStdFunction());

  const TagIndex expected_tag_index{"tag1", 0};
  auto child = std::make_unique<CompoundItem>();
  auto child_ptr = child.get();
  auto child_identifier = child->GetIdentifier();
  auto grandchild = child->AddProperty("width", 42);
  auto grandchild_identifier = grandchild.GetIdentifier();

  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto command =
      std::make_unique<InsertItemCommandV2>(std::move(child), &parent, expected_tag_index);

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
