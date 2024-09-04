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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;

class MacroCommandTest : public ::testing::Test
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

  private:
    void ExecuteImpl() override { m_execute_callback(m_command_id); }

    void UndoImpl() override { m_undo_callback(m_command_id); }

    int m_command_id;
    callback_t m_execute_callback;
    callback_t m_undo_callback;
  };

  class MockCommandListener
  {
  public:
    MOCK_METHOD(void, OnExecute, (int));
    MOCK_METHOD(void, OnUndo, (int));
  };

  std::unique_ptr<TestCommand> CreateCommand(int command_id, MockCommandListener& listener)
  {
    auto on_execute = [&listener](int command_id) { listener.OnExecute(command_id); };
    auto on_undo = [&listener](int command_id) { listener.OnUndo(command_id); };
    return std::make_unique<TestCommand>(command_id, on_execute, on_undo);
  }
};

TEST_F(MacroCommandTest, InitialState)
{
  MockCommandListener listener;

  const std::string description("description");
  const int command_id1{1};
  const int command_id2{2};
  MacroCommand macro_command(description);

  EXPECT_EQ(macro_command.GetCommandCount(), 0);
  EXPECT_EQ(macro_command.GetDescription(), description);

  macro_command.Append(CreateCommand(command_id1, listener));
  macro_command.Append(CreateCommand(command_id2, listener));

  EXPECT_EQ(macro_command.GetCommandCount(), 2);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(command_id1)).Times(1);
    EXPECT_CALL(listener, OnExecute(command_id2)).Times(1);
    EXPECT_CALL(listener, OnUndo(_)).Times(0);
  }

  macro_command.Execute();

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(listener, OnExecute(_)).Times(0);
    // undo in reverse order
    EXPECT_CALL(listener, OnUndo(command_id2)).Times(1);
    EXPECT_CALL(listener, OnUndo(command_id1)).Times(1);
  }

  macro_command.Undo();
}
