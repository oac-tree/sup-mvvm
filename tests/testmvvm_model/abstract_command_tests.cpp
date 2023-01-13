/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/abstract_command.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>

using namespace mvvm;
using ::testing::_;

//! Testing AbstractCommand.

class AbstractCommandTests : public ::testing::Test
{
public:
  class TestCommand : public AbstractCommand
  {
  public:
    MOCK_METHOD(void, ExecuteImpl, ());
    MOCK_METHOD(void, UndoImpl, ());
  };
};

//! Initial state.

TEST_F(AbstractCommandTests, InitialState)
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

TEST_F(AbstractCommandTests, Execute)
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

TEST_F(AbstractCommandTests, UndoOnStart)
{
  TestCommand command;

  EXPECT_CALL(command, ExecuteImpl()).Times(0);
  EXPECT_CALL(command, UndoImpl()).Times(0);

  // triggering action
  // it is not possible to undo the command  that hasn't been executed
  EXPECT_THROW(command.Undo(), RuntimeException);
}

//! Execution and then undo.

TEST_F(AbstractCommandTests, Undo)
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

TEST_F(AbstractCommandTests, ExecuteUndoExecute)
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

TEST_F(AbstractCommandTests, UndoOfObsoleteCommand)
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
