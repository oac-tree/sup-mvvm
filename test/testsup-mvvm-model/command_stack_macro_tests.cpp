/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/macro_command.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/test/mock_command.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Extended tests foor CommandStack populated with macro commands.
class CommandStackMacroTest : public ::testing::Test
{
};

//! Begin and end empty macro.
TEST_F(CommandStackMacroTest, BeginEndEmptyMacro)
{
  CommandStack stack;

  // it is not possible to end macro without starting it first
  EXPECT_THROW(stack.EndMacro(), RuntimeException);

  EXPECT_FALSE(stack.IsMacroMode());

  // starting macro
  stack.BeginMacro("test");
  EXPECT_TRUE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetCommandCount(), 1);

  // it is not possible to undo or redo while in macro mode
  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());

  // closing macro will change the mode, the command remains
  stack.EndMacro();
  EXPECT_FALSE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetCommandCount(), 1);
}

//! Recording two commands in single macro.
TEST_F(CommandStackMacroTest, AddTwoCommandsToMacro)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  stack.BeginMacro("test");

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
  }

  // adding two commands to macro will immediately execute them
  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));

  // two commands are seen as a single macro command
  EXPECT_TRUE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);

  // undo/redo is possible after closing macro
  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  stack.EndMacro();
  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());

  // single undo will undo two commands one after another
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnUndo(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr1)).Times(1);
  }

  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
}
