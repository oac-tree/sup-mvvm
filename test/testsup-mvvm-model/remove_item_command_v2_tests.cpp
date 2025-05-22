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

#include "mvvm/commands/remove_item_command_v2.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>

using namespace mvvm;

class RemoveItemCommandV2Tests : public ::testing::Test
{
};

TEST_F(RemoveItemCommandV2Tests, InitialState)
{
  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  auto child = parent.InsertItem<SessionItem>(TagIndex::Append());

  EXPECT_THROW(RemoveItemCommandV2(nullptr, TagIndex::Append()), RuntimeException);

  const TagIndex expected_tag_index{"tag1", 0};
  const RemoveItemCommandV2 command(&parent, expected_tag_index);

  const auto expected_event1 =
      std::optional{event_variant_t{AboutToRemoveItemEvent{&parent, expected_tag_index}}};

  EXPECT_EQ(command.GetResult().get(), nullptr);
  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  // event without actual command execution doesn't exist
  EXPECT_THROW(command.GetEventAfter(), RuntimeException);
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_TRUE(command.GetDescription().empty());
}

TEST_F(RemoveItemCommandV2Tests, RemoveItemFromParent)
{
  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto child0 = parent.InsertItem<SessionItem>(TagIndex::Append());
  auto child1 = parent.InsertItem<SessionItem>(TagIndex::Append());
  auto child2 = parent.InsertItem<SessionItem>(TagIndex::Append());

  child1->SetData(42);
  auto child1_identifier = child1->GetIdentifier();

  const TagIndex expected_tag_index{"tag1", 1};
  const auto expected_event1 =
      std::optional{event_variant_t{AboutToRemoveItemEvent{&parent, expected_tag_index}}};
  const auto expected_event2 =
      std::optional{event_variant_t{ItemRemovedEvent{&parent, expected_tag_index}}};
  const auto expected_event3 =
      std::optional{event_variant_t{AboutToInsertItemEvent{&parent, expected_tag_index}}};
  const auto expected_event4 =
      std::optional{event_variant_t{ItemInsertedEvent{&parent, expected_tag_index}}};

  RemoveItemCommandV2 command(&parent, expected_tag_index);

  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  command.Execute();
  EXPECT_EQ(command.GetEventAfter(), expected_event2);
  EXPECT_FALSE(command.IsObsolete());

  auto taken = command.GetResult();
  EXPECT_EQ(taken.get(), child1);
  EXPECT_EQ(parent.GetTotalItemCount(), 2);
  EXPECT_EQ(taken->Data<mvvm::int32>(), 42);
  EXPECT_EQ(taken->GetIdentifier(), child1_identifier);

  EXPECT_EQ(command.GetEventBefore(), expected_event3);
  command.Undo();
  EXPECT_EQ(command.GetEventAfter(), expected_event4);
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(parent.GetTotalItemCount(), 3);
  EXPECT_EQ(command.GetResult().get(), nullptr);
  EXPECT_EQ(parent.GetAllItems().at(1)->Data<mvvm::int32>(), 42);
  EXPECT_EQ(parent.GetAllItems().at(1)->GetIdentifier(), child1_identifier);

  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  command.Execute();
  EXPECT_EQ(command.GetEventAfter(), expected_event2);
  EXPECT_EQ(parent.GetTotalItemCount(), 2);

  auto taken2 = command.GetResult();
  EXPECT_EQ(taken2->Data<mvvm::int32>(), 42);
  EXPECT_EQ(taken2->GetIdentifier(), child1_identifier);
}

// TEST_F(RemoveItemCommandV2Tests, RemoveParentWithChildWhenPartOfTheModel)
// {
//   SessionModel model;
//   auto parent = model.InsertItem<CompoundItem>();
//   parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
//   auto parent_identifier = parent->GetIdentifier();

//   auto child = parent->InsertItem<SessionItem>(TagIndex::Append());
//   child->SetData(42);
//   auto child_identifier = child->GetIdentifier();

//   const TagIndex expected_tag_index = TagIndex::First();
//   const auto expected_event1 =
//       std::optional{event_variant_t{AboutToRemoveItemEvent{parent, expected_tag_index}}};
//   const auto expected_event2 =
//       std::optional{event_variant_t{ItemRemovedEvent{parent, expected_tag_index}}};
//   const auto expected_event3 =
//       std::optional{event_variant_t{AboutToInsertItemEvent{parent, expected_tag_index}}};
//   const auto expected_event4 =
//       std::optional{event_variant_t{ItemInsertedEvent{parent, expected_tag_index}}};

//   RemoveItemCommandV2 command(model.GetRootItem(), expected_tag_index);

//   EXPECT_EQ(command.GetEventBefore(), expected_event1);
//   command.Execute();
//   EXPECT_EQ(command.GetEventAfter(), expected_event2);
//   EXPECT_FALSE(command.IsObsolete());

  // auto taken = command.GetResult();
  // EXPECT_EQ(taken.get(), child1);
  // EXPECT_EQ(parent.GetTotalItemCount(), 2);
  // EXPECT_EQ(taken->Data<mvvm::int32>(), 42);
  // EXPECT_EQ(taken->GetIdentifier(), child1_identifier);

  // EXPECT_EQ(command.GetEventBefore(), expected_event3);
  // command.Undo();
  // EXPECT_EQ(command.GetEventAfter(), expected_event4);
  // EXPECT_FALSE(command.IsObsolete());
  // EXPECT_EQ(parent.GetTotalItemCount(), 3);
  // EXPECT_EQ(command.GetResult().get(), nullptr);
  // EXPECT_EQ(parent.GetAllItems().at(1)->Data<mvvm::int32>(), 42);
  // EXPECT_EQ(parent.GetAllItems().at(1)->GetIdentifier(), child1_identifier);

  // EXPECT_EQ(command.GetEventBefore(), expected_event1);
  // command.Execute();
  // EXPECT_EQ(command.GetEventAfter(), expected_event2);
  // EXPECT_EQ(parent.GetTotalItemCount(), 2);

  // auto taken2 = command.GetResult();
  // EXPECT_EQ(taken2->Data<mvvm::int32>(), 42);
  // EXPECT_EQ(taken2->GetIdentifier(), child1_identifier);
// }
