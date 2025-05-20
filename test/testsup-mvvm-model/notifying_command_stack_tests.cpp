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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_command_stack.h>

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
  // EXPECT_CALL(m_event_listener, Call(::testing::_)).Times(1);

  stack.Undo();
}
