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

#include "mvvm/commands/command_helper.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests for helper methods from command_helper.h
 */
class CommandHelperTests : public ::testing::Test
{
};

TEST_F(CommandHelperTests, GetBeforeEventForInsertCommand)
{
  SessionItem item;
  const TagIndex tag_index{"tag", 42};

  {
    const auto expected_event = event_variant_t{AboutToInsertItemEvent{&item, tag_index}};
    EXPECT_EQ(GetBeforeEventForInsertCommand(CommandStatus::Initial, &item, tag_index),
              expected_event);
  }

  {
    const auto expected_event = event_variant_t{AboutToInsertItemEvent{&item, tag_index}};
    EXPECT_EQ(GetBeforeEventForInsertCommand(CommandStatus::AfterUndo, &item, tag_index),
              expected_event);
  }

  {
    const auto expected_event = event_variant_t{AboutToRemoveItemEvent{&item, tag_index}};
    EXPECT_EQ(GetBeforeEventForInsertCommand(CommandStatus::AfterExecute, &item, tag_index),
              expected_event);
  }
}

TEST_F(CommandHelperTests, GetAfterEventForInsertCommand)
{
  SessionItem item;
  const TagIndex tag_index{"tag", 42};

  {
    const auto expected_event = event_variant_t{ItemInsertedEvent{&item, tag_index}};
    EXPECT_EQ(GetAfterEventForInsertCommand(CommandStatus::AfterExecute, &item, tag_index),
              expected_event);
  }

  {
    const auto expected_event = event_variant_t{ItemRemovedEvent{&item, tag_index}};
    EXPECT_EQ(GetAfterEventForInsertCommand(CommandStatus::AfterUndo, &item, tag_index),
              expected_event);
  }

  {  // we can't be in the initial state after Undo or Execute command
    EXPECT_THROW(GetAfterEventForInsertCommand(CommandStatus::Initial, &item, tag_index),
                 RuntimeException);
  }
}
