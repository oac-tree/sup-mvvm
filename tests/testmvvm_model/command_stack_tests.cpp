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

#include "mvvm/commands/abstract_command.h"
#include "mvvm/commands/command_stack.h"
#include "mvvm/core/exceptions.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class CommandStackTests : public ::testing::Test
{
public:
  //! Mock class to test AbstractCommand.
  class MockCommand : public AbstractCommand
  {
  public:
    MOCK_METHOD0(ExecuteImpl, void(void));
    MOCK_METHOD0(UndoImpl, void(void));
  };

  //! Command decorator to use together with CommandStack. It shall be used to wrap MockCommand
  //! to prevent passing an ownership to CommandStack. Will help with googletest warnings related to
  //! testing::Mock::AllowLeak.
  class CommandDecorator : public AbstractCommand
  {
  public:
    explicit CommandDecorator(MockCommand& decoratee) : m_decoratee(&decoratee) {}

    bool IsObsolete() const override { return m_decoratee->IsObsolete(); }

  private:
    void ExecuteImpl() override { m_decoratee->ExecuteImpl(); }

    void UndoImpl() override { m_decoratee->UndoImpl(); }

    MockCommand* m_decoratee{nullptr};
  };
};

TEST_F(CommandStackTests, InitialState)
{
  CommandStack stack;

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetSize(), 0);
}

//! Execute single command.

TEST_F(CommandStackTests, SingleCommandExecution)
{
  MockCommand mock_command;

  CommandStack stack;

  auto command = std::make_unique<CommandDecorator>(mock_command);

  EXPECT_CALL(mock_command, ExecuteImpl()).Times(1);
  EXPECT_CALL(mock_command, UndoImpl()).Times(0);

  stack.Execute(std::move(command));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 1);
}

//! Execute single command which is expired already before the execution.

TEST_F(CommandStackTests, SingleCommandIsObsoleteBeforeExecution)
{
  MockCommand mock_command;
  mock_command.SetIsObsolete(true);

  CommandStack stack;

  auto command = std::make_unique<CommandDecorator>(mock_command);

  EXPECT_CALL(mock_command, ExecuteImpl()).Times(0);
  EXPECT_CALL(mock_command, UndoImpl()).Times(0);

  EXPECT_THROW(stack.Execute(std::move(command)), RuntimeException);

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetSize(), 0);
}
