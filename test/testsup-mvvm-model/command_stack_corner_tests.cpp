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

//! Testing CommandStack.
class CommandStackCornerTest : public ::testing::Test
{
public:
};

//! Special command that tries to work with stack before finishing its own implementation activity.
TEST_F(CommandStackCornerTest, AttemptToBeginMacroBeforeFinishing)
{
  {  // attempt to begin macro before own end
    CommandStack stack;
    auto on_execute = [this, &stack](auto) { stack.BeginMacro("macro"); };
    auto command = std::make_unique<test::TestCommand>(on_execute, [](auto) {});
    EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);
  }

  {  // attempt to end macro before own end
    CommandStack stack;
    stack.BeginMacro("macro");
    auto on_execute = [this, &stack](auto) { stack.EndMacro(); };
    auto command = std::make_unique<test::TestCommand>(on_execute, [](auto) {});
    EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);
  }

  {  // attempt to push new command
    CommandStack stack;
    auto on_execute = [this, &stack](auto)
    { stack.Execute(std::make_unique<test::TestCommand>()); };
    auto command = std::make_unique<test::TestCommand>(on_execute, [](auto) {});
    EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);
  }

  {  // attempt to begin macro in the middle of own undo
    CommandStack stack;
    auto on_undo = [this, &stack](auto) { stack.BeginMacro("macro"); };
    auto command = std::make_unique<test::TestCommand>([](auto) {}, on_undo);
    stack.Execute(std::move(command));
    EXPECT_THROW(stack.Undo(), RuntimeException);
  }

  {  // attempt to end macro in the middle of own undo
    CommandStack stack;
    auto on_undo = [this, &stack](auto) { stack.EndMacro(); };
    auto command = std::make_unique<test::TestCommand>([](auto) {}, on_undo);
    stack.Execute(std::move(command));
    EXPECT_THROW(stack.Undo(), RuntimeException);
  }

  {  // attempt to push new command in the middle of own undo
    CommandStack stack;
    auto on_undo = [this, &stack](auto) { stack.Execute(std::make_unique<test::TestCommand>()); };
    auto command = std::make_unique<test::TestCommand>([](auto) {}, on_undo);
    stack.Execute(std::move(command));
    EXPECT_THROW(stack.Undo(), RuntimeException);
  }
}
