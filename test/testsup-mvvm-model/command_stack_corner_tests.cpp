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

#include "mvvm/commands/command_stack.h"

#include <mvvm/commands/abstract_command.h>
#include <mvvm/core/mvvm_exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>

namespace mvvm::test
{

/**
 * @brief Testing CommandStack for corner cases.
 *
 * Various scenario when command tries to work with stack before finishing its own implementation
 * activity.
 */
class CommandStackCornerTest : public ::testing::Test
{
};

//!
TEST_F(CommandStackCornerTest, AttemptToBeginMacroBeforeFinishing)
{
  {  // attempt to begin macro before own end
    CommandStack stack;

    test::TestCommandContext context;
    context.execute_impl = [this, &stack](auto) { stack.BeginMacro("macro"); };
    context.undo_impl = [](auto) {};
    auto command = std::make_unique<test::TestCommand>(context);
    EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);
  }

  {  // attempt to end macro before own end
    CommandStack stack;
    stack.BeginMacro("macro");
    test::TestCommandContext context;
    context.execute_impl = [this, &stack](auto) { stack.EndMacro(); };
    context.undo_impl = [](auto) {};
    auto command = std::make_unique<test::TestCommand>(context);
    EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);
  }

  {  // attempt to push new command
    CommandStack stack;
    test::TestCommandContext context;
    context.execute_impl = [this, &stack](auto)
    { stack.Execute(std::make_unique<test::TestCommand>()); };
    context.undo_impl = [](auto) {};
    auto command = std::make_unique<test::TestCommand>(context);
    EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);
  }

  {  // attempt to begin macro in the middle of own undo
    CommandStack stack;
    test::TestCommandContext context;
    context.execute_impl = [](auto) {};
    context.undo_impl = [this, &stack](auto) { stack.BeginMacro("macro"); };
    auto command = std::make_unique<test::TestCommand>(context);
    stack.Execute(std::move(command));
    EXPECT_THROW(stack.Undo(), RuntimeException);
  }

  {  // attempt to end macro in the middle of own undo
    CommandStack stack;
    test::TestCommandContext context;
    context.execute_impl = [](auto) {};
    context.undo_impl = [this, &stack](auto) { stack.EndMacro(); };
    auto command = std::make_unique<test::TestCommand>(context);
    stack.Execute(std::move(command));
    EXPECT_THROW(stack.Undo(), RuntimeException);
  }

  {  // attempt to push new command in the middle of own undo
    CommandStack stack;
    test::TestCommandContext context;
    context.execute_impl = [](auto) {};
    context.undo_impl = [this, &stack](auto)
    { stack.Execute(std::make_unique<test::TestCommand>()); };
    auto command = std::make_unique<test::TestCommand>(context);
    stack.Execute(std::move(command));
    EXPECT_THROW(stack.Undo(), RuntimeException);
  }
}

}  // namespace mvvm::test
