/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/commands/set_value_command.h"

#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/sessionmodel.h>
#include <testutils/mock_event_listener.h>

using namespace mvvm;
using ::testing::_;

//! Testing SetValueCommand.

class SetValueCommandTests : public ::testing::Test
{
public:
  std::unique_ptr<ModelComposerInterface> CreateStandardComposer()
  {
    return std::make_unique<ModelComposer>(m_model);
  }

  std::unique_ptr<ModelComposerInterface> CreateNotifyingComposer()
  {
    return std::make_unique<NotifyingModelComposer<ModelComposer>>(&m_event_handler, m_model);
  }

  SetValueCommandTests() { m_listener.SubscribeAll(&m_event_handler); }

  SessionModel m_model;
  ModelEventHandler m_event_handler;
  MockEventListener m_listener;
};

//! Set item value through SetValueCommand command using ModelComposer.

TEST_F(SetValueCommandTests, SetValueUsingModelComposer)
{
  const int role = DataRole::kData;
  auto composer = CreateStandardComposer();

  auto item = m_model.InsertItem<SessionItem>();

  // initial state
  EXPECT_FALSE(utils::IsValid(item->Data(role)));

  // creating command
  variant_t expected(42);
  auto command = std::make_unique<SetValueCommand>(composer.get(), item, expected, role);

  // data is still unchanged
  EXPECT_FALSE(utils::IsValid(item->Data(role)));

  // executing command
  command->Execute();
  EXPECT_TRUE(command->GetResult());  // value was changed
  EXPECT_EQ(item->Data(role), expected);
  EXPECT_FALSE(command->IsObsolete());

  // undoing command
  command->Undo();
  EXPECT_TRUE(command->GetResult());  // value was changed
  EXPECT_FALSE(utils::IsValid(item->Data(role)));
  EXPECT_FALSE(command->IsObsolete());
}

//! Set same item value through SetValueCommand command.

TEST_F(SetValueCommandTests, SetSameValueCommand)
{
  const int role = DataRole::kData;
  auto composer = CreateStandardComposer();

  // creating an item and setting the data
  auto item = m_model.InsertItem<SessionItem>();
  variant_t expected(42);
  item->SetData(expected, role);

  // command to set same value
  auto command = std::make_unique<SetValueCommand>(composer.get(), item, expected, role);

  // executing command
  command->Execute();
  EXPECT_FALSE(command->GetResult());  // value wasn't changed
  EXPECT_EQ(item->Data(role), expected);
  EXPECT_TRUE(command->IsObsolete());

  // undoing command which is in isObsolete state is not possible
  EXPECT_THROW(command->Undo(), mvvm::RuntimeException);
}

//! Set the value to item

TEST_F(SetValueCommandTests, SetValueUsingNotifyingModelComposer)
{
  const int role = DataRole::kData;
  auto composer = CreateNotifyingComposer();

  auto item0 = m_model.InsertItem<SessionItem>();
  auto item1 = m_model.InsertItem<SessionItem>();
  auto item2 = m_model.InsertItem<SessionItem>();

  // initial state
  EXPECT_FALSE(utils::IsValid(item1->Data(role)));

  // creating command
  variant_t expected(42);
  auto command = std::make_unique<SetValueCommand>(composer.get(), item1, expected, role);

  // data is still unchanged
  EXPECT_FALSE(utils::IsValid(item1->Data(role)));

  DataChangedEvent data_changed_event{item1, role};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(data_changed_event))).Times(1);

  // executing command
  command->Execute();
  EXPECT_TRUE(command->GetResult());  // value was changed
  EXPECT_EQ(item1->Data(role), expected);
  EXPECT_FALSE(command->IsObsolete());

  EXPECT_CALL(m_listener, OnEvent(event_variant_t(data_changed_event))).Times(1);

  // undoing command
  command->Undo();
  EXPECT_TRUE(command->GetResult());  // value was changed
  EXPECT_FALSE(utils::IsValid(item1->Data(role)));
  EXPECT_FALSE(command->IsObsolete());
}
