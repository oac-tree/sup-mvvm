/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/abstractcommand.h"

#include "mvvm/core/exceptions.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

//! Testing AbstractCommand.

class AbstractCommandTest : public ::testing::Test
{
public:
  class TestCommand : public AbstractCommand
  {
  public:
    MOCK_METHOD0(ExecuteImpl, void(void));
    MOCK_METHOD0(UndoImpl, void(void));
  };
};

//! Initial state.

TEST_F(AbstractCommandTest, InitialState)
{
  TestCommand command;

  EXPECT_TRUE(command.GetDescription().empty());
  EXPECT_FALSE(command.IsObsolete());

  command.SetDescription("abc");
  EXPECT_EQ(command.GetDescription(), std::string("abc"));

  command.SetIsObsolete(true);
  EXPECT_TRUE(command.IsObsolete());
}

//! Simple command execution.

TEST_F(AbstractCommandTest, Execute)
{
  TestCommand command;

  EXPECT_CALL(command, ExecuteImpl()).Times(1);
  EXPECT_CALL(command, UndoImpl()).Times(0);

  // triggering action
  command.Execute();

  // second execution is not possible
  EXPECT_THROW(command.Execute(), RuntimeException);
}

//! Attempt to undo without execution.

TEST_F(AbstractCommandTest, UndoOnStart)
{
  TestCommand command;

  EXPECT_CALL(command, ExecuteImpl()).Times(0);
  EXPECT_CALL(command, UndoImpl()).Times(0);

  // triggering action
  // it is not possible to undo the command  that hasn't been executed
  EXPECT_THROW(command.Undo(), RuntimeException);
}

//! Execution and then undo.

TEST_F(AbstractCommandTest, Undo)
{
  TestCommand command;

  {
    ::testing::InSequence seq;
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
    EXPECT_CALL(command, UndoImpl()).Times(1);
  }

  // triggering action
  command.Execute();

  command.Undo();

  // second undo is not possible
  EXPECT_THROW(command.Undo(), RuntimeException);
}

//! Execute, then undo, then execute again.

TEST_F(AbstractCommandTest, ExecuteUndoExecute)
{
  TestCommand command;

  {
    ::testing::InSequence seq;
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
    EXPECT_CALL(command, UndoImpl()).Times(1);
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
  }

  command.Execute();
  command.Undo();
  command.Execute();
}

//! Attempt to undo obsolete command.

TEST_F(AbstractCommandTest, UndoOfObsoleteCommand)
{
  TestCommand command;

  {
    ::testing::InSequence seq;
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
    EXPECT_CALL(command, UndoImpl()).Times(0);
  }

  // triggering action
  command.Execute();

  command.SetIsObsolete(true);

  // second undo is not possible
  EXPECT_THROW(command.Undo(), RuntimeException);
}
