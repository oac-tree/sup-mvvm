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

#include <mvvm/commands/abstract_command.h>
#include "mvvm/commands/command_stack.h"
#include <mvvm/core/exceptions.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>

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

  //! Command decorator to use together with CommandStack. It wraps MockCommand to prevent passing
  //! an ownership to CommandStack. Will help with googletest warnings related to
  //! testing::Mock::AllowLeak.
  class CommandDecorator : public AbstractCommand
  {
  public:
    explicit CommandDecorator(MockCommand& decoratee) : m_decoratee(&decoratee) {}

    bool IsObsolete() const override { return m_decoratee->IsObsolete(); }

    void SetMakeObsoleteAfterExecution() { m_make_obsolete = true; }

  private:
    void ExecuteImpl() override
    {
      m_decoratee->ExecuteImpl();
      if (m_make_obsolete)
      {
        m_decoratee->SetIsObsolete(true);
      }
    }

    void UndoImpl() override { m_decoratee->UndoImpl(); }

    MockCommand* m_decoratee{nullptr};
    bool m_make_obsolete{false};
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
  auto command_ptr = command.get();

  EXPECT_CALL(mock_command, ExecuteImpl()).Times(1);
  EXPECT_CALL(mock_command, UndoImpl()).Times(0);

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 1);
}

//! Execute single command which is obsolete already before the execution.

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

//! Execute single command which gets obsolete after execution.

TEST_F(CommandStackTests, SingleCommandIsObsoleteAfterExecution)
{
  MockCommand mock_command;

  CommandStack stack;

  auto command = std::make_unique<CommandDecorator>(mock_command);
  command->SetMakeObsoleteAfterExecution();

  EXPECT_CALL(mock_command, ExecuteImpl()).Times(1);
  EXPECT_CALL(mock_command, UndoImpl()).Times(0);

  EXPECT_EQ(stack.Execute(std::move(command)), nullptr);

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetSize(), 0);
}

//! Execute single command, then undo, then redo again

TEST_F(CommandStackTests, SingleCommandExecuteUndoRedo)
{
  MockCommand mock_command;

  CommandStack stack;

  auto command = std::make_unique<CommandDecorator>(mock_command);

  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_command, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command, UndoImpl()).Times(1);
    EXPECT_CALL(mock_command, ExecuteImpl()).Times(1);
  }

  stack.Execute(std::move(command));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 1);

  stack.Undo();
  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetSize(), 1);

  stack.Redo();
  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 1);
}

//! Execute two commands, then two undo, then two redo.

TEST_F(CommandStackTests, TwoCommandsExecution)
{
  MockCommand mock_command1;
  MockCommand mock_command2;

  CommandStack stack;

  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_command1, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command2, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command2, UndoImpl()).Times(1);
    EXPECT_CALL(mock_command1, UndoImpl()).Times(1);
    EXPECT_CALL(mock_command1, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command2, ExecuteImpl()).Times(1);
  }

  stack.Execute(std::make_unique<CommandDecorator>(mock_command1));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 1);

  stack.Execute(std::make_unique<CommandDecorator>(mock_command2));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetSize(), 2);

  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 2);

  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetSize(), 2);

  stack.Redo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 2);

  stack.Redo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetSize(), 2);
}

//! Inserts three commands, make undo twice, and then insert new command.
//! It should replace two last commands.

TEST_F(CommandStackTests, InsertInTheMiddleOfUndo)
{
  MockCommand mock_command1;
  MockCommand mock_command2;
  MockCommand mock_command3;
  MockCommand mock_command4;

  CommandStack stack;

  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_command1, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command2, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command3, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command3, UndoImpl()).Times(1);
    EXPECT_CALL(mock_command2, UndoImpl()).Times(1);
    EXPECT_CALL(mock_command4, ExecuteImpl()).Times(1);
  }

  stack.Execute(std::make_unique<CommandDecorator>(mock_command1));
  stack.Execute(std::make_unique<CommandDecorator>(mock_command2));
  stack.Execute(std::make_unique<CommandDecorator>(mock_command3));

  stack.Undo();
  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 1);
  EXPECT_EQ(stack.GetSize(), 3);

  // insertion of a new command should remove command2 and command3 from the stack
  stack.Execute(std::make_unique<CommandDecorator>(mock_command4));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetSize(), 2);
}

//! Inserts three commands, make undo twice, and then insert new command.
//! It should replace two last commands.

TEST_F(CommandStackTests, CleanCommands)
{
  MockCommand mock_command1;
  MockCommand mock_command2;
  MockCommand mock_command3;

  CommandStack stack;

  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_command1, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command2, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command3, ExecuteImpl()).Times(1);
    EXPECT_CALL(mock_command3, UndoImpl()).Times(1);
  }

  stack.Execute(std::make_unique<CommandDecorator>(mock_command1));
  stack.Execute(std::make_unique<CommandDecorator>(mock_command2));
  stack.Execute(std::make_unique<CommandDecorator>(mock_command3));

  stack.Undo();

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 2);
  EXPECT_EQ(stack.GetSize(), 3);

  stack.Clear();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(stack.GetIndex(), 0);
  EXPECT_EQ(stack.GetSize(), 0);
}
