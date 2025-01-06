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

#include "mvvm/commands/command_stack.h"

#include <mvvm/commands/abstract_command.h>
#include <mvvm/core/exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>

using namespace mvvm;
using ::testing::_;

class CommandStackTest : public ::testing::Test
{
public:
};

TEST_F(CommandStackTest, InitialState)
{
  const CommandStack stack;

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 0);
}

//! Execute single command.
TEST_F(CommandStackTest, SingleCommandExecution)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command, command_ptr] = CreateCommand(listener);

  EXPECT_CALL(listener, OnExecute(_)).Times(1);
  EXPECT_CALL(listener, OnUndo(_)).Times(0);

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);
}

//! Execute single command which is obsolete already before the execution.
TEST_F(CommandStackTest, SingleCommandIsObsoleteBeforeExecution)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command, command_ptr] = CreateCommand(listener);
  command->SetIsObsolete(true);

  EXPECT_CALL(listener, OnExecute(_)).Times(0);
  EXPECT_CALL(listener, OnUndo(_)).Times(0);

  EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 0);
}

//! Execute single command which gets obsolete after execution.
TEST_F(CommandStackTest, SingleCommandIsObsoleteAfterExecution)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command, command_ptr] = CreateCommand(listener);
  command->SetMakeObsoleteAfterExecution();

  EXPECT_CALL(listener, OnExecute(_)).Times(1);
  EXPECT_CALL(listener, OnUndo(_)).Times(0);

  EXPECT_EQ(stack.Execute(std::move(command)), nullptr);

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 0);
}

//! Execute single command, then undo, then redo again
TEST_F(CommandStackTest, SingleCommandExecuteUndoRedo)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command, command_ptr] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(_)).Times(1);
    EXPECT_CALL(listener, OnUndo(_)).Times(1);
    EXPECT_CALL(listener, OnExecute(_)).Times(1);
  }

  stack.Execute(std::move(command));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);

  stack.Undo();
  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 1);

  stack.Redo();
  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);
}

//! Execute two commands, then two undo, then two redo.
TEST_F(CommandStackTest, TwoCommandsExecution)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
  }

  stack.Execute(std::move(command1));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);

  stack.Execute(std::move(command2));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetCommandCount(), 2);

  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 2);

  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 2);

  stack.Redo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 2);

  stack.Redo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetCommandCount(), 2);
}

//! Inserts three commands, make undo twice, and then insert new command. It should replace two last
//! commands.
TEST_F(CommandStackTest, InsertInTheMiddleOfUndo)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);
  auto [command3, command_ptr3] = CreateCommand(listener);
  auto [command4, command_ptr4] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr4)).Times(1);
  }

  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.Execute(std::move(command3));

  stack.Undo();
  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 3);

  // insertion of a new command should remove command2 and command3 from the stack
  stack.Execute(std::move(command4));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetCommandCount(), 2);
}

//! Inserts three commands, make undo twice, and then insert new command. It should replace two last
//! commands.
TEST_F(CommandStackTest, CleanCommands)
{
  test::MockTestCommandListener listener;
  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);
  auto [command3, command_ptr3] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr3)).Times(1);
  }

  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.Execute(std::move(command3));

  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetCommandCount(), 3);

  stack.Clear();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 0);
}

//! Checking that number of commands do not exeed undo limit. Undo limit is set at the very
//! beginning.
TEST_F(CommandStackTest, UndoLimit)
{
  using mvvm::test::CreateCommand;

  CommandStack stack;
  stack.SetUndoLimit(2);

  auto [command1, command_ptr1] = CreateCommand();
  auto [command2, command_ptr2] = CreateCommand();
  auto [command3, command_ptr3] = CreateCommand();
  auto [command4, command_ptr4] = CreateCommand();

  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.Execute(std::move(command3));
  stack.Execute(std::move(command4));

  EXPECT_EQ(stack.GetCommandCount(), 2);
  EXPECT_EQ(stack.GetCommands(), std::vector<const ICommand *>({command_ptr3, command_ptr4}));
}

//! Undo limit is set when stack already got 4 commands
TEST_F(CommandStackTest, SetUndoLimitAfterExecution)
{
  using mvvm::test::CreateCommand;

  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand();
  auto [command2, command_ptr2] = CreateCommand();
  auto [command3, command_ptr3] = CreateCommand();
  auto [command4, command_ptr4] = CreateCommand();

  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.Execute(std::move(command3));
  stack.Execute(std::move(command4));

  EXPECT_EQ(stack.GetIndex(), 4);

  stack.SetUndoLimit(2);

  // command1 and command2 should be removed, index should point "after" command4
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetCommands(), std::vector<const ICommand *>({command_ptr3, command_ptr4}));
}

TEST_F(CommandStackTest, SetUndoLimitInTheMiddleOfUndo)
{
  using mvvm::test::CreateCommand;

  CommandStack stack;

  auto [command1, command_ptr1] = CreateCommand();
  auto [command2, command_ptr2] = CreateCommand();
  auto [command3, command_ptr3] = CreateCommand();
  auto [command4, command_ptr4] = CreateCommand();

  stack.Execute(std::move(command1));
  stack.Execute(std::move(command2));
  stack.Execute(std::move(command3));
  stack.Execute(std::move(command4));

  stack.Undo();
  stack.Undo();

  // at this point command4, command3 are undone, index is pointing after command2
  EXPECT_EQ(stack.GetIndex(), 2);

  // Setting undo limit to 1 item only. Current algorithm only allow to erase items which are in
  // "AfterExecution" state. This means that command3 should stay.
  stack.SetUndoLimit(1);

  EXPECT_EQ(stack.GetIndex(), 0);

  // Thus, despite of undo limit 1, we still have 2 commands in the list.
  EXPECT_EQ(stack.GetCommandCount(), 2);
  EXPECT_EQ(stack.GetCommands(), std::vector<const ICommand *>({command_ptr3, command_ptr4}));

  // Lets redo and check the list again.
  stack.Redo();

  // There is only one single command4, and it is in undone state
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetCommandCount(), 1);
  EXPECT_EQ(stack.GetCommands(), std::vector<const ICommand *>({command_ptr4}));

  stack.Redo();
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetCommandCount(), 1);
  EXPECT_EQ(stack.GetCommands(), std::vector<const ICommand *>({command_ptr4}));
}
