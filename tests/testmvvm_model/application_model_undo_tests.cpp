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

#include "mock_model_listener.h"
#include "mvvm/model/application_model.h"

#include <gtest/gtest.h>
#include <mvvm/commands/command_stack_interface.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>

using namespace mvvm;
using ::testing::_;

//! Tests for ModelComposer class.
//! Class is used to manipulate the model and generate necessary notifications.

class ApplicationModelUndoTests : public ::testing::Test
{
public:
  ApplicationModel m_model;
};

//! Inserting item, changing the data, removing item, undoing.

TEST_F(ApplicationModelUndoTests, InsertItemSetDataRemoveItem)
{
  m_model.SetUndoEnabled(true);

  auto commands = m_model.GetCommandStack();

  // inserting item and setting the data
  auto item = m_model.InsertItem<SessionItem>();
  item->SetData(42);

  // status of command stack with two commands
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetSize(), 2);

  // removing item
  m_model.RemoveItem(item);

  // status of command stack with three commands
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_FALSE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 3);
  EXPECT_EQ(commands->GetSize(), 3);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // undoing last removal
  commands->Undo();

  // status of command stack
  EXPECT_TRUE(commands->CanUndo());
  EXPECT_TRUE(commands->CanRedo());
  EXPECT_EQ(commands->GetIndex(), 2);
  EXPECT_EQ(commands->GetSize(), 3);

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(m_model.GetRootItem()->GetItem("", 0)->Data(), variant_t(42));
}
