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

#include <mvvm/test/mock_command.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class MacroCommandTest : public ::testing::Test
{
};

TEST_F(MacroCommandTest, InitialState)
{
  test::MockTestCommandListener listener;

  const std::string description("description");
  MacroCommand macro_command(description);

  EXPECT_EQ(macro_command.GetCommandCount(), 0);
  EXPECT_EQ(macro_command.GetDescription(), description);

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);

  macro_command.Append(std::move(command1));
  macro_command.Append(std::move(command2));

  EXPECT_EQ(macro_command.GetCommandStatus(), CommandStatus::Initial);

  EXPECT_EQ(macro_command.GetCommandCount(), 2);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(_)).Times(0);
  }

  macro_command.Execute();
  EXPECT_EQ(macro_command.GetCommandStatus(), CommandStatus::AfterExecute);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(_)).Times(0);
    // undo in reverse order
    EXPECT_CALL(listener, OnUndo(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr1)).Times(1);
  }

  macro_command.Undo();
  EXPECT_EQ(macro_command.GetCommandStatus(), CommandStatus::AfterUndo);
}

//! This reproduces how MacroCommand works inside CommandStack.
//! It gets its children after they have been already executed
TEST_F(MacroCommandTest, AddCommandAfterExecution)
{
  test::MockTestCommandListener listener;

  const std::string description("description");
  MacroCommand macro_command(description);

  EXPECT_EQ(macro_command.GetCommandCount(), 0);
  EXPECT_EQ(macro_command.GetDescription(), description);

  auto [command1, command_ptr1] = CreateCommand(listener);
  auto [command2, command_ptr2] = CreateCommand(listener);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_ptr1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(_)).Times(0);
  }

  command1->Execute();
  command2->Execute();

  macro_command.Append(std::move(command1));
  macro_command.Append(std::move(command2));

  EXPECT_EQ(macro_command.GetCommandStatus(), CommandStatus::AfterExecute);
  EXPECT_EQ(macro_command.GetCommandCount(), 2);
  EXPECT_EQ(macro_command.GetCommands(), std::vector<const ICommand*>({command_ptr1, command_ptr2}));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(_)).Times(0);
    // undo in reverse order
    EXPECT_CALL(listener, OnUndo(command_ptr2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_ptr1)).Times(1);
  }

  macro_command.Undo();
  EXPECT_EQ(macro_command.GetCommandStatus(), CommandStatus::AfterUndo);
}
