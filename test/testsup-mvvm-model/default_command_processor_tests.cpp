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

#include "mvvm/commands/default_command_processor.h"

#include <mvvm/commands/set_value_command.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>

namespace mvvm::test
{

/**
 * @brief Tests for DefaultCommandProcessor class.
 */
class DefaultCommandProcessorTest : public ::testing::Test
{
public:
  ::testing::MockFunction<void(const event_variant_t&)> m_event_listener;
};

TEST_F(DefaultCommandProcessorTest, ExecuteCommand)
{
  DefaultCommandProcessor stack;

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0U);
  EXPECT_EQ(stack.GetCommandCount(), 0U);
  EXPECT_NO_THROW(stack.Undo());
  EXPECT_NO_THROW(stack.Redo());
  EXPECT_NO_THROW(stack.Clear());
  EXPECT_NO_THROW(stack.SetUndoLimit(1));
  EXPECT_NO_THROW(stack.BeginMacro("abc"));
  EXPECT_NO_THROW(stack.EndMacro());
  EXPECT_EQ(stack.GetNextUndoCommand(), nullptr);
  EXPECT_EQ(stack.GetNextRedoCommand(), nullptr);

  test::MockTestCommandListener listener;
  auto [command, command_ptr] = CreateCommand(listener);
  EXPECT_CALL(listener, OnGetEventBefore(::testing::_)).Times(1);
  EXPECT_CALL(listener, OnExecute(::testing::_)).Times(1);
  EXPECT_CALL(listener, OnGetEventAfter(::testing::_)).Times(1);
  EXPECT_CALL(listener, OnNotify(::testing::_, ::testing::_)).Times(0);

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);
}

TEST_F(DefaultCommandProcessorTest, ExecuteSetValueCommand)
{
  DefaultCommandProcessor stack;

  SessionModel model;

  auto item = model.InsertItem<SessionItem>();
  auto command = std::make_unique<SetValueCommand>(item, int32{42}, DataRole::kData);

  auto result = static_cast<SetValueCommand*>(stack.Execute(std::move(command)));
  ASSERT_NE(result, nullptr);
  EXPECT_TRUE(result->GetResult());
  EXPECT_EQ(item->Data<int32>(), 42);
}

}  // namespace mvvm::test
