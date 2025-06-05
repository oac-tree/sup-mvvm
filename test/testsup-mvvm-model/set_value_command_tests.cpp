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

#include "mvvm/commands/set_value_command.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for SetValueCommand.
 */
class SetValueCommandTest : public ::testing::Test
{
};

TEST_F(SetValueCommandTest, CreateCommand)
{
  const int role = DataRole::kData;
  SessionItem item;

  EXPECT_THROW(SetValueCommand(nullptr, {}, role), RuntimeException);

  EXPECT_FALSE(utils::IsValid(item.Data(role)));
  const variant_t expected(42);
  const SetValueCommand command(&item, expected, role);
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_FALSE(command.GetResult());
  EXPECT_FALSE(command.GetEventAfter().has_value());
  EXPECT_FALSE(command.GetEventAfter().has_value());
  EXPECT_TRUE(command.GetDescription().empty());

  EXPECT_FALSE(utils::IsValid(item.Data(role)));
}

TEST_F(SetValueCommandTest, SetItemValueWhenItemHasNoData)
{
  const int role = DataRole::kData;
  SessionItem item;

  const variant_t expected(42);
  auto command = std::make_unique<SetValueCommand>(&item, expected, role);

  EXPECT_FALSE(command->GetEventAfter().has_value());
  command->Execute();
  EXPECT_TRUE(command->GetEventAfter().has_value());

  const auto expected_event = std::optional{event_variant_t{DataChangedEvent{&item, role}}};
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item.Data(role), expected);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Undo();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item.Data(role), variant_t{});
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Execute();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item.Data(role), expected);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());
}

TEST_F(SetValueCommandTest, SetItemValue)
{
  const int role = DataRole::kData;
  const variant_t initial_value(42);

  SessionItem item;
  item.SetData(initial_value, role);

  const variant_t new_value(43);
  auto command = std::make_unique<SetValueCommand>(&item, new_value, role);

  const auto expected_event = std::optional{event_variant_t{DataChangedEvent{&item, role}}};
  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Execute();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item.Data(role), new_value);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Undo();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item.Data(role), initial_value);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Execute();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item.Data(role), new_value);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());
}

TEST_F(SetValueCommandTest, SetSameValue)
{
  const int role = DataRole::kData;
  const variant_t initial_value(43);

  SessionItem item;
  item.SetData(initial_value, role);

  auto command = std::make_unique<SetValueCommand>(&item, initial_value, role);

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Execute();
  EXPECT_FALSE(command->GetEventAfter().has_value());

  EXPECT_EQ(item.Data(role), initial_value);
  EXPECT_TRUE(command->IsObsolete());
  EXPECT_FALSE(command->GetResult());

  EXPECT_THROW(command->Undo(), RuntimeException);
  EXPECT_THROW(command->Execute(), RuntimeException);
}

TEST_F(SetValueCommandTest, WhenPartOfModelSetItemValue)
{
  const int role = DataRole::kData;
  const variant_t initial_value(42);

  SessionModel model;
  auto item = model.InsertItem<SessionItem>();

  item->SetData(initial_value, role);

  const variant_t new_value(43);
  auto command = std::make_unique<SetValueCommand>(item, new_value, role);

  const auto expected_event = std::optional{event_variant_t{DataChangedEvent{item, role}}};
  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Execute();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item->Data(role), new_value);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Undo();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item->Data(role), initial_value);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());

  EXPECT_FALSE(command->GetEventBefore().has_value());
  command->Execute();
  EXPECT_EQ(command->GetEventAfter(), expected_event);

  EXPECT_EQ(item->Data(role), new_value);
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_TRUE(command->GetResult());
}

}  // namespace mvvm::test
