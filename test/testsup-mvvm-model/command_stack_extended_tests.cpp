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
#include <mvvm/commands/i_command.h>
#include <mvvm/commands/insert_item_command.h>
#include <mvvm/commands/remove_item_command.h>
#include <mvvm/commands/set_value_command.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/test/test_container_helper.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for CommandStack with real stack and real commands.
 *
 * Commands operate on SessionModel, notifications absent.
 */
class CommandStackExtendedTests : public ::testing::Test
{
public:
  SessionModel m_model;
};

TEST_F(CommandStackExtendedTests, SetValueCommand)
{
  const role_t role = DataRole::kData;
  const variant_t initial_value(43);

  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(initial_value, role);

  CommandStack stack;

  const variant_t new_value(44);
  auto [command, command_ptr] = CreateUnique<SetValueCommand>(item, new_value, role);

  stack.Execute(std::move(command));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item->Data(), new_value);

  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(item->Data(), initial_value);

  // attempt to set the same initial data
  auto [command2, command2_ptr] = CreateUnique<SetValueCommand>(item, initial_value, role);

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
  auto& grandchild = child->AddProperty("width", 42);
  auto grandchild_identifier = grandchild.GetIdentifier();

  auto parent = m_model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto [command, _] = CreateUnique<InsertItemCommand>(std::move(child), parent, expected_tag_index);

  stack.Execute(std::move(command));

  ASSERT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(parent->GetAllItems().at(0), child_ptr);

  stack.Undo();
  ASSERT_EQ(parent->GetTotalItemCount(), 0);

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

TEST_F(CommandStackExtendedTests, RemoveItemCommand)
{
  CommandStack stack;

  auto parent = m_model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  const TagIndex expected_tag_index{"tag1", 0};
  auto child = parent->InsertItem<SessionItem>(expected_tag_index);

  auto [command, _] = CreateUnique<RemoveItemCommand>(parent, expected_tag_index);

  stack.Execute(std::move(command));
  ASSERT_EQ(parent->GetTotalItemCount(), 0);

  stack.Undo();
  ASSERT_EQ(parent->GetTotalItemCount(), 1);

  stack.Redo();
  ASSERT_EQ(parent->GetTotalItemCount(), 0);
}

//! Checking together with DefaultCommandProcessor, where we do not have undo/redo,
//! but still want to have notifications.
TEST_F(CommandStackExtendedTests, SetValueCommandWithDefaultProcessor)
{
  const role_t role = DataRole::kData;
  const variant_t initial_value(43);

  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(initial_value, role);

  DefaultCommandProcessor stack;

  const variant_t new_value(44);
  auto [command1, command1_ptr] = CreateUnique<SetValueCommand>(item, new_value, role);

  stack.Execute(std::move(command1));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item->Data(), new_value);

  stack.Undo();
  EXPECT_EQ(item->Data(), new_value);
  stack.Redo();
  EXPECT_EQ(item->Data(), new_value);
}

}  // namespace mvvm::test
