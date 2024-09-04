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

#include "mvvm/commands/command_stack.h"

#include <mvvm/commands/abstract_command.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/test/mock_command.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class CommandStackTest : public ::testing::Test
{
public:
  using callback_t = std::function<void(int)>;

  /**
   * @brief The TestCommand class is intended to test the order of execution of ExecuteImpl and
   * UndoImpl, when several commands are placed in a stack.
   */
  class TestCommand : public AbstractCommand
  {
  public:
    explicit TestCommand(int command_id, callback_t execute_callback, callback_t undo_callback)
        : m_command_id(command_id)
        , m_execute_callback(std::move(execute_callback))
        , m_undo_callback(std::move(undo_callback))
    {
    }

    void SetMakeObsoleteAfterExecution() { m_make_obsolete = true; }

  private:
    void ExecuteImpl() override
    {
      m_execute_callback(m_command_id);
      if (m_make_obsolete)
      {
        SetIsObsolete(true);
      }
    }

    void UndoImpl() override { m_undo_callback(m_command_id); }

    int m_command_id;
    callback_t m_execute_callback;
    callback_t m_undo_callback;
    bool m_make_obsolete{false};
  };

  class MockCommandListener
  {
  public:
    MOCK_METHOD(void, OnExecute, (int));
    MOCK_METHOD(void, OnUndo, (int));
  };

  std::unique_ptr<TestCommand> CreateCommand(MockCommandListener& listener, int command_id = 0)
  {
    auto on_execute = [&listener](int command_id) { listener.OnExecute(command_id); };
    auto on_undo = [&listener](int command_id) { listener.OnUndo(command_id); };
    return std::make_unique<TestCommand>(command_id, on_execute, on_undo);
  }
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
  MockCommandListener listener;
  CommandStack stack;

  auto command = CreateCommand(listener);
  auto command_ptr = command.get();

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
  MockCommandListener listener;
  CommandStack stack;

  auto command = CreateCommand(listener);
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
  MockCommandListener listener;
  CommandStack stack;

  auto command = CreateCommand(listener);
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
  MockCommandListener listener;
  CommandStack stack;

  auto command = CreateCommand(listener);

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
  MockCommandListener listener;
  CommandStack stack;

  const int command_id1{1};
  auto command1 = CreateCommand(listener, command_id1);
  const int command_id2{2};
  auto command2 = CreateCommand(listener, command_id2);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_id1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_id2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_id1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id2)).Times(1);
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
  MockCommandListener listener;
  CommandStack stack;

  const int command_id1{1};
  auto command1 = CreateCommand(listener, command_id1);
  const int command_id2{2};
  auto command2 = CreateCommand(listener, command_id2);
  const int command_id3{3};
  auto command3 = CreateCommand(listener, command_id3);
  const int command_id4{4};
  auto command4 = CreateCommand(listener, command_id4);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_id1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_id3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_id2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id4)).Times(1);
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
  MockCommandListener listener;
  CommandStack stack;

  const int command_id1{1};
  auto command1 = CreateCommand(listener, command_id1);
  const int command_id2{2};
  auto command2 = CreateCommand(listener, command_id2);
  const int command_id3{3};
  auto command3 = CreateCommand(listener, command_id3);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_id1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id2)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id3)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_id3)).Times(1);
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
