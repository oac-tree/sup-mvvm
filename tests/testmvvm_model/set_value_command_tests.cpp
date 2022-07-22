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

#include "mvvm/commands/set_value_command.h"

#include <gtest/gtest.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/core/exceptions.h>

using namespace mvvm;

//! Testing SetValueCommand.

class SetValueCommandTests : public ::testing::Test
{
public:
  std::unique_ptr<ModelComposerInterface> CreateComposer()
  {
    return std::make_unique<ModelComposer>(m_model);
  }
  SessionModel m_model;
};

//! Set item value through SetValueCommand command using ModelComposer.

TEST_F(SetValueCommandTests, SetValueUsingModelComposer)
{
  const int role = DataRole::kData;
  auto composer = CreateComposer();

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
  auto composer = CreateComposer();

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
