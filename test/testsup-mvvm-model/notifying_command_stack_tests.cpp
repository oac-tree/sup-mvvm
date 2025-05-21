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
};

TEST_F(NotifyingCommandStackTests, TriviallyDecoratedMethods)
{
  NotifyingCommandStack<test::CommandStackDecorator> stack(m_event_listener.AsStdFunction(),
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

TEST_F(NotifyingCommandStackTests, Execute)
{
  test::MockTestCommandListener mock_command_listener;

  // It is not possible to derive from mocking object directly. We are deriving from the decorator
  // which looks to moking object.
  test::MockCommandStack mock_stack;
  NotifyingCommandStack<test::CommandStackDecorator> stack(m_event_listener.AsStdFunction(),
                                                           &mock_stack);

  auto [command, command_ptr] = CreateCommand(mock_command_listener);

  // EXPECT_CALL(mock_command_listener, GetEventBefore());
  // EXPECT_CALL(mock_stack, Execute(::testing::_)).Times(1);
  // EXPECT_CALL(mock_command_listener, OnExecute(::testing::_)).Times(1);
  // EXPECT_CALL(mock_command_listener, OnUndo(::testing::_)).Times(0);

  EXPECT_EQ(stack.Execute(std::move(command)), command_ptr);
}
