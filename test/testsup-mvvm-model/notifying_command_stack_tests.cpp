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

#include "mvvm/commands/notifying_command_stack.h"

#include <mvvm/commands/command_stack.h>
#include <mvvm/commands/i_command.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command.h>
#include <testutils/mock_command_stack.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

class NotifyingCommandStackTests : public ::testing::Test
{
public:
  ::testing::MockFunction<void(const event_variant_t&)> m_event_listener;
  test::MockCommandStack m_mock_stack_listener;
  test::MockTestCommandListener m_mock_command_listener;
};

TEST_F(NotifyingCommandStackTests, TriviallyDecoratedMethods)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);
  EXPECT_CALL(m_mock_stack_listener, CanUndo()).Times(1);
  stack.CanUndo();

  EXPECT_CALL(m_mock_stack_listener, CanRedo()).Times(1);
  stack.CanRedo();

  EXPECT_CALL(m_mock_stack_listener, GetIndex()).Times(1);
  stack.GetIndex();

  EXPECT_CALL(m_mock_stack_listener, GetCommandCount()).Times(1);
  stack.GetCommandCount();

  EXPECT_CALL(m_mock_stack_listener, Clear()).Times(1);
  stack.Clear();

  EXPECT_CALL(m_mock_stack_listener, SetUndoLimit(1)).Times(1);
  stack.SetUndoLimit(1);

  EXPECT_CALL(m_mock_stack_listener, BeginMacro("abc")).Times(1);
  stack.BeginMacro("abc");

  EXPECT_CALL(m_mock_stack_listener, EndMacro()).Times(1);
  stack.EndMacro();
}

TEST_F(NotifyingCommandStackTests, AttemptToExecuteCommand)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);

  EXPECT_THROW(stack.Execute(std::unique_ptr<test::TestCommand>()), RuntimeException);
}

TEST_F(NotifyingCommandStackTests, ExecuteWhenCommandDoNotReportEvents)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);

  auto [command, command_ptr] = CreateCommand(m_mock_command_listener);

  {
    // no notification since command by default reports empty events
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_command_listener, OnGetEventBefore(command_ptr)).Times(1);
    EXPECT_CALL(m_mock_stack_listener, Execute(::testing::_)).Times(1);
    EXPECT_CALL(m_mock_command_listener, OnGetEventAfter(command_ptr)).Times(1);
  }

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);
}

TEST_F(NotifyingCommandStackTests, ExecuteWhenCommandReportsOnceAndThenGetsExpired)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);

  auto [command, command_ptr] = CreateCommand(m_mock_command_listener);

  auto event1 = event_variant_t{DataChangedEvent{nullptr, 41}};
  auto event2 = event_variant_t{DataChangedEvent{nullptr, 42}};

  ON_CALL(m_mock_command_listener, OnGetEventBefore(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event1)));
  ON_CALL(m_mock_command_listener, OnGetEventAfter(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event2)));

  // mimick situation when base stack returns nullptr (command was obsolete)
  ON_CALL(m_mock_stack_listener, Execute(::testing::_)).WillByDefault(::testing::Return(nullptr));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_command_listener, OnGetEventBefore(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event1));
    EXPECT_CALL(m_mock_stack_listener, Execute(::testing::_)).Times(1);
  }

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);
}

TEST_F(NotifyingCommandStackTests, ExecuteWhenCommandReportsBeforeAndAfter)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);

  auto [command, command_ptr] = CreateCommand(m_mock_command_listener);

  auto event1 = event_variant_t{DataChangedEvent{nullptr, 41}};
  auto event2 = event_variant_t{DataChangedEvent{nullptr, 42}};

  ON_CALL(m_mock_command_listener, OnGetEventBefore(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event1)));
  ON_CALL(m_mock_command_listener, OnGetEventAfter(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event2)));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_command_listener, OnGetEventBefore(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event1));  // report event before
    EXPECT_CALL(m_mock_stack_listener, Execute(::testing::_)).Times(1);
    EXPECT_CALL(m_mock_command_listener, OnGetEventAfter(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event2));  // report event after
  }

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);
}

TEST_F(NotifyingCommandStackTests, Undo)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);

  auto [command, command_ptr] = CreateCommand(m_mock_command_listener);

  auto event1 = event_variant_t{DataChangedEvent{nullptr, 41}};
  auto event2 = event_variant_t{DataChangedEvent{nullptr, 42}};

  ON_CALL(m_mock_command_listener, OnGetEventBefore(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event1)));
  ON_CALL(m_mock_command_listener, OnGetEventAfter(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event2)));
  ON_CALL(m_mock_stack_listener, GetNextUndoCommand())
      .WillByDefault(::testing::Return(command_ptr));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_stack_listener, GetNextUndoCommand()).Times(1);
    EXPECT_CALL(m_mock_command_listener, OnGetEventBefore(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event1));
    EXPECT_CALL(m_mock_stack_listener, Undo()).Times(1);
    EXPECT_CALL(m_mock_command_listener, OnGetEventAfter(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event2));
  }

  stack.Undo();
}

TEST_F(NotifyingCommandStackTests, Redo)
{
  NotifyingCommandStack<test::MockCommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                               &m_mock_stack_listener);

  auto [command, command_ptr] = CreateCommand(m_mock_command_listener);

  auto event1 = event_variant_t{DataChangedEvent{nullptr, 41}};
  auto event2 = event_variant_t{DataChangedEvent{nullptr, 42}};

  ON_CALL(m_mock_command_listener, OnGetEventBefore(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event1)));
  ON_CALL(m_mock_command_listener, OnGetEventAfter(::testing::_))
      .WillByDefault(::testing::Return(std::optional<event_variant_t>(event2)));
  ON_CALL(m_mock_stack_listener, GetNextRedoCommand())
      .WillByDefault(::testing::Return(command_ptr));

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_stack_listener, GetNextRedoCommand()).Times(1);
    EXPECT_CALL(m_mock_command_listener, OnGetEventBefore(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event1));
    EXPECT_CALL(m_mock_stack_listener, Redo()).Times(1);
    EXPECT_CALL(m_mock_command_listener, OnGetEventAfter(command_ptr)).Times(1);
    EXPECT_CALL(m_event_listener, Call(event2));
  }

  stack.Redo();
}
