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
#include "mvvm/commands/remove_item_command.h"

#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/sessionmodel.h>

using namespace mvvm;
using ::testing::_;

//! Testing RemoveItemCommand.

class RemoveItemCommandTests : public ::testing::Test
{
public:
  std::unique_ptr<ModelComposerInterface> CreateStandardComposer()
  {
    return std::make_unique<ModelComposer>(m_model);
  }

  std::unique_ptr<ModelComposerInterface> CreateNotifyingComposer()
  {
    return std::make_unique<NotifyingModelComposer<ModelComposer>>(&m_notifier, m_model);
  }

  SessionModel m_model;
  MockModelEventNotifier m_notifier;
};

//! Remove item from the model using RemoveItemCommand based on ModelComposer.

TEST_F(RemoveItemCommandTests, RemoveItemFromModel)
{
  auto composer = CreateStandardComposer();

  // inserting item in the model
  auto item = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  auto item_identifier = item->GetIdentifier();

  // command to remove item from the model
  auto command =
      std::make_unique<RemoveItemCommand>(composer.get(), m_model.GetRootItem(), TagIndex{"", 0});

  // item is still there
  EXPECT_EQ(m_model.GetRootItem()->GetItem({"", 0}), item);

  command->Execute();  // removal
  EXPECT_FALSE(command->IsObsolete());

  // the command holds taken item for later reuse, if necessary
  auto taken = command->GetResult();
  EXPECT_EQ(taken.get(), item);

  // attempt to get taken item a second time doesn't work
  EXPECT_EQ(command->GetResult().get(), nullptr);

  // checking root item
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // undo command
  command->Undo();
  EXPECT_FALSE(command->IsObsolete());

  // checking restored item
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto restored = m_model.GetRootItem()->GetItem({"", 0});
  EXPECT_EQ(restored->GetIdentifier(), item_identifier);

  // redoing item removal
  command->Execute();

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
}

