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
#include <testutils/mock_command_stack.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

class NotifyingCommandStackTests : public ::testing::Test
{
public:
  ::testing::MockFunction<void(const event_variant_t&)> m_event_listener;
};

TEST_F(NotifyingCommandStackTests, InitialState)
{
  NotifyingCommandStack<test::MockCommandStack> stack(m_event_listener.AsStdFunction());

  EXPECT_CALL(stack, Undo()).Times(1);

  stack.Undo();
}

TEST_F(NotifyingCommandStackTests, NotifyingCommandStackWithSetValueCommand)
{
  const int role = DataRole::kData;
  const variant_t initial_value(43);

  SessionItem item;
  item.SetData(initial_value, role);

  NotifyingCommandStack<CommandStack> stack(m_event_listener.AsStdFunction());

  const variant_t new_value1(44);
  auto [command1, command1_ptr] = test::CreateTestData<SetValueCommandV2>(&item, new_value1, role);

  const auto expected_event1 = event_variant_t{DataChangedEvent{&item, role}};
  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  stack.Execute(std::move(command1));

  EXPECT_TRUE(stack.CanUndo());
  EXPECT_FALSE(stack.CanRedo());
  EXPECT_EQ(item.Data(), new_value1);

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(1);
  stack.Undo();

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
  EXPECT_EQ(item.Data(), initial_value);

  // attempt to set the same initial data
  auto [command2, command2_ptr] = test::CreateTestData<SetValueCommandV2>(&item, initial_value, role);

  EXPECT_CALL(m_event_listener, Call(expected_event1)).Times(0);
  stack.Execute(std::move(command2));

  EXPECT_FALSE(stack.CanUndo());
  EXPECT_TRUE(stack.CanRedo());
}
