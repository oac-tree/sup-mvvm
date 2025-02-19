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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/macro_command.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/core/exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>

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
  stack.BeginMacro("macro");
  EXPECT_TRUE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetCommandCount(), 1);

  // it is not possible to undo or redo while in macro mode
  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());

  stack.EndMacro();
  EXPECT_FALSE(stack.IsMacroMode());

  // empty macro command was automatically cleaned
  EXPECT_EQ(stack.GetCommandCount(), 0);
}

//! Undo redo of empty macro should be allowed (real-life bug).
TEST_F(CommandStackMacroTest, UndoEmptyMacro)
{
  CommandStack stack;

  // creating empty macro
  stack.BeginMacro("macro");
  EXPECT_TRUE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetCommandCount(), 1);

  // it is not possible to undo or redo while in macro mode
  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());

  stack.EndMacro();
  EXPECT_FALSE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetCommandCount(), 0);

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());

  EXPECT_NO_THROW(stack.Undo());
  EXPECT_NO_THROW(stack.Redo());
}

//! Recording two commands in a single macro.
TEST_F(CommandStackMacroTest, AddTwoCommandsToMacro)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  stack.BeginMacro("macro");

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

//! Inserts three commands: macro, command, and another macro. Make undo twice, and then insert new
//! command. It should replace two last commands.
TEST_F(CommandStackMacroTest, MacroInsertInTheMiddleOfUndo)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);
  auto [command3, command_ptr3] = CreateCommand(listener);
  auto [command4, command_ptr4] = CreateCommand(listener);
  auto [command5, command_ptr5] = CreateCommand(listener);
  auto [command6, command_ptr6] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr3)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr4)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr5)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr6)).Times(1);
  }

  stack.BeginMacro("macro1");
  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.EndMacro();

  stack.Execute(std::move(command3));

  stack.BeginMacro("macro2");
  stack.Execute(std::move(command4));
  stack.Execute(std::move(command5));
  stack.Execute(std::move(command6));
  stack.EndMacro();

  EXPECT_EQ(stack.GetCommandCount(), 3);  // macro, single command, macro
  EXPECT_EQ(stack.GetIndex(), 3);
  EXPECT_FALSE(stack.IsMacroMode());

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnUndo(command_ptr6)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr5)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr4)).Times(1);
  }

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_FALSE(stack.IsMacroMode());

  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetCommandCount(), 3);
  EXPECT_EQ(stack.GetIndex(), 2);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnUndo(command_ptr3)).Times(1);
  }

  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetCommandCount(), 3);
  EXPECT_EQ(stack.GetIndex(), 1);

  // insertion of a new command should remove command3 and macro2 from the stack
  auto [command7, command_ptr7] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr7)).Times(1);
  }

  stack.Execute(std::move(command7));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetCommandCount(), 2);
}

//! Inserts macro inside macro.
TEST_F(CommandStackMacroTest, NestedMacro)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);
  auto [command3, command_ptr3] = CreateCommand(listener);
  auto [command4, command_ptr4] = CreateCommand(listener);
  auto [command5, command_ptr5] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr3)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr4)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr5)).Times(1);
  }

  stack.BeginMacro("macro1");
  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.BeginMacro("macro2");
  stack.Execute(std::move(command3));
  stack.Execute(std::move(command4));
  stack.Execute(std::move(command5));
  stack.EndMacro();
  stack.EndMacro();

  ASSERT_EQ(stack.GetCommands().size(), 1);

  // checking command tree in two macros
  auto macro1 = dynamic_cast<const MacroCommand*>(stack.GetCommands().at(0));
  ASSERT_NE(macro1, nullptr);
  ASSERT_EQ(macro1->GetCommandCount(), 3);  // command1, command2, and macro2
  auto macro2 = dynamic_cast<const MacroCommand*>(macro1->GetCommands().at(2));
  ASSERT_NE(macro2, nullptr);
  EXPECT_EQ(macro1->GetCommands(),
            std::vector<const ICommand*>({command_ptr1, command_ptr2, macro2}));
  EXPECT_EQ(macro2->GetCommands(),
            std::vector<const ICommand*>({command_ptr3, command_ptr4, command_ptr5}));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnUndo(command_ptr5)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr4)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr1)).Times(1);
  }

  // all 5 commands will be undone in one go
  stack.Undo();
}

//! Recording two commands in a single macro. The first command announce, that it is obsolete.
TEST_F(CommandStackMacroTest, AddTwoCommandsToMacroOneIsObsolete)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  stack.BeginMacro("macro");

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);
  command1->SetMakeObsoleteAfterExecution();

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
  }

  // adding two commands to macro will immediately execute them
  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));

  // one commands are seen as a single macro command
  EXPECT_TRUE(stack.IsMacroMode());
  EXPECT_EQ(stack.GetIndex(), 1);
  ASSERT_EQ(stack.GetCommandCount(), 1);

  auto macro_command = dynamic_cast<const MacroCommand*>(stack.GetCommands().at(0));
  ASSERT_NE(macro_command, nullptr);
  EXPECT_EQ(macro_command->GetCommands(), std::vector<const ICommand*>({command_ptr2}));

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
  }

  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
}
