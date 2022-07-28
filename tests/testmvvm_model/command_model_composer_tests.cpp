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

#include "mock_model_event_notifier.h"
#include "mvvm/commands/command_model_composer.h"

#include <gtest/gtest.h>
#include <mvvm/commands/command_stack.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/sessionmodel.h>

using namespace mvvm;
using ::testing::_;

class CommandModelComposerTests : public ::testing::Test
{
public:
  //! create CommandModelComposer relying on standard ModelComposer
  std::unique_ptr<CommandModelComposer> CreateComposer()
  {
    return std::make_unique<CommandModelComposer>(&m_commands,
                                                  std::make_unique<ModelComposer>(m_model));
  }

  //! CreateModelComposer relying on NotifyingModelComposer
  std::unique_ptr<CommandModelComposer> CreateNotifyingComposer()
  {
    auto notifying_composer =
        std::make_unique<NotifyingModelComposer<ModelComposer>>(&m_notifier, m_model);
    return std::make_unique<CommandModelComposer>(&m_commands, std::move(notifying_composer));
  }

  SessionModel m_model;
  CommandStack m_commands;
  MockModelEventNotifier m_notifier;
};

TEST_F(CommandModelComposerTests, InitialState)
{
  auto composer = CreateComposer();
  EXPECT_EQ(composer->GetModel(), &m_model);
  EXPECT_EQ(m_commands.GetSize(), 0);
}

TEST_F(CommandModelComposerTests, SetData)
{
  const int role = DataRole::kData;
  auto composer = CreateComposer();

  // inserting item without data
  auto item = m_model.InsertItem<SessionItem>();
  EXPECT_FALSE(utils::IsValid(item->Data(role)));

  // setting data via composer
  variant_t expected_data(42);
  EXPECT_TRUE(composer->SetData(item, 42, DataRole::kData));

  // checking the data and status of command stack
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 1);
  EXPECT_EQ(m_commands.GetSize(), 1);

  // undoing the command
  m_commands.Undo();

  // checking the data and status of command stack
  EXPECT_FALSE(utils::IsValid(item->Data(role)));
  EXPECT_FALSE(m_commands.CanUndo());
  EXPECT_TRUE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 0);
  EXPECT_EQ(m_commands.GetSize(), 1);

  // redoing command
  m_commands.Redo();

  // checking the data and status of command stack
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 1);
  EXPECT_EQ(m_commands.GetSize(), 1);
}

TEST_F(CommandModelComposerTests, SetSameData)
{
  const int role = DataRole::kData;
  variant_t expected_data(42);

  // creating item and setting initial data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(expected_data);

  auto composer = CreateComposer();

  // setting same data
  EXPECT_FALSE(composer->SetData(item, 42, DataRole::kData));

  // no data change
  EXPECT_EQ(item->Data(), variant_t(42));

  // no commands should be in the stack
  EXPECT_FALSE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 0);
  EXPECT_EQ(m_commands.GetSize(), 0);
}

//! Setting the data several times.

TEST_F(CommandModelComposerTests, SetDataSeveralTimes)
{
  const int role = DataRole::kData;

  // creating item and setting initial data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(10);

  auto composer = CreateComposer();

  // setting data several times
  EXPECT_TRUE(composer->SetData(item, 11, DataRole::kData));
  EXPECT_TRUE(composer->SetData(item, 12, DataRole::kData));
  EXPECT_TRUE(composer->SetData(item, 13, DataRole::kData));

  // checking data and stack
  EXPECT_EQ(item->Data(), variant_t(13));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 3);
  EXPECT_EQ(m_commands.GetSize(), 3);

  // undoing twice and checking status
  m_commands.Undo();
  m_commands.Undo();
  EXPECT_EQ(item->Data(), variant_t(11));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_TRUE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 1);
  EXPECT_EQ(m_commands.GetSize(), 3);

  // while being in partly undone state, make a new command
  // this will remove two last commands
  EXPECT_TRUE(composer->SetData(item, 14, DataRole::kData));

  EXPECT_EQ(item->Data(), variant_t(14));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 2);
  EXPECT_EQ(m_commands.GetSize(), 2);

  // two undo should give us initial value
  m_commands.Undo();
  m_commands.Undo();

  EXPECT_EQ(item->Data(), variant_t(10));
  EXPECT_FALSE(m_commands.CanUndo());
  EXPECT_TRUE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 0);
  EXPECT_EQ(m_commands.GetSize(), 2);

  m_commands.Redo();
  EXPECT_EQ(item->Data(), variant_t(11));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_TRUE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 1);

  // back on top of the stack
  m_commands.Redo();
  EXPECT_EQ(item->Data(), variant_t(14));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 2);
}

//! Set data when composer is capable of notifications.
TEST_F(CommandModelComposerTests, SetDataWithNotifyingComposer)
{
  const int role = DataRole::kData;
  auto composer = CreateNotifyingComposer();

  // inserting item without data
  auto item = m_model.InsertItem<SessionItem>();
  EXPECT_FALSE(utils::IsValid(item->Data(role)));

  // setting data via composer
  EXPECT_CALL(m_notifier, DataChangedNotify(item, role)).Times(1);
  variant_t expected_data(42);
  EXPECT_TRUE(composer->SetData(item, 42, DataRole::kData));

  // checking the data and status of command stack
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 1);
  EXPECT_EQ(m_commands.GetSize(), 1);

  // undoing the command
  EXPECT_CALL(m_notifier, DataChangedNotify(item, role)).Times(1);
  m_commands.Undo();

  // checking the data and status of command stack
  EXPECT_FALSE(utils::IsValid(item->Data(role)));
  EXPECT_FALSE(m_commands.CanUndo());
  EXPECT_TRUE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 0);
  EXPECT_EQ(m_commands.GetSize(), 1);

  // redoing command
  EXPECT_CALL(m_notifier, DataChangedNotify(item, role)).Times(1);
  m_commands.Redo();

  // checking the data and status of command stack
  EXPECT_EQ(item->Data(), variant_t(42));
  EXPECT_TRUE(m_commands.CanUndo());
  EXPECT_FALSE(m_commands.CanRedo());
  EXPECT_EQ(m_commands.GetIndex(), 1);
  EXPECT_EQ(m_commands.GetSize(), 1);

  // setting same data and checking absence of notifications
  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);
  EXPECT_FALSE(composer->SetData(item, 42, DataRole::kData));
}
