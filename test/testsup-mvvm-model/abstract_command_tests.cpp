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

#include "mvvm/commands/abstract_command.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>

using namespace mvvm;

/**
 * @brief Tests for AbsractCommand class.
 */
class AbstractCommandTests : public ::testing::Test
{
};

TEST_F(AbstractCommandTests, InitialState)
{
  test::MockCommand command;

  EXPECT_TRUE(command.GetDescription().empty());
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(command.GetCommandStatus(), CommandStatus::Initial);
  EXPECT_FALSE(command.GetEventBefore().has_value());
  EXPECT_FALSE(command.GetEventAfter().has_value());

  command.SetDescription("abc");
  EXPECT_EQ(command.GetDescription(), std::string("abc"));

  command.SetIsObsolete(true);
  EXPECT_TRUE(command.IsObsolete());
}

TEST_F(AbstractCommandTests, Execute)
{
  test::MockCommand command;

  EXPECT_CALL(command, ExecuteImpl()).Times(1);
  EXPECT_CALL(command, UndoImpl()).Times(0);

  // triggering action
  command.Execute();

  EXPECT_EQ(command.GetCommandStatus(), CommandStatus::AfterExecute);

  // second execution is not possible
  EXPECT_THROW(command.Execute(), RuntimeException);
}

TEST_F(AbstractCommandTests, UndoOnStart)
{
  test::MockCommand command;

  EXPECT_CALL(command, ExecuteImpl()).Times(0);
  EXPECT_CALL(command, UndoImpl()).Times(0);

  // triggering action
  // it is not possible to undo the command  that hasn't been executed
  EXPECT_THROW(command.Undo(), RuntimeException);
}

TEST_F(AbstractCommandTests, ExecuteAndUndo)
{
  test::MockCommand command;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
    EXPECT_CALL(command, UndoImpl()).Times(1);
  }

  // triggering action
  command.Execute();

  command.Undo();

  EXPECT_EQ(command.GetCommandStatus(), CommandStatus::AfterUndo);

  // second undo is not possible
  EXPECT_THROW(command.Undo(), RuntimeException);
}

TEST_F(AbstractCommandTests, ExecuteUndoExecute)
{
  test::MockCommand command;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
    EXPECT_CALL(command, UndoImpl()).Times(1);
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
  }

  command.Execute();
  command.Undo();
  command.Execute();
}

TEST_F(AbstractCommandTests, AttemptToUndoObsoleteCommand)
{
  test::MockCommand command;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(command, ExecuteImpl()).Times(1);
    EXPECT_CALL(command, UndoImpl()).Times(0);
  }

  // triggering action
  command.Execute();

  command.SetIsObsolete(true);

  // second undo is not possible
  EXPECT_THROW(command.Undo(), RuntimeException);
}
