/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/commands/insert_item_command.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for InsertItemCommand class.
 */
class InsertItemCommandTests : public ::testing::Test
{
public:
  SessionModel m_model;
};

TEST_F(InsertItemCommandTests, InitialState)
{
  auto child = std::make_unique<SessionItem>();
  EXPECT_THROW(InsertItemCommand(std::move(child), nullptr, TagIndex::Append()), RuntimeException);

  auto to_insert = std::make_unique<PropertyItem>();
  const TagIndex expected_tag_index{"tag1", 0};

  auto parent = m_model.InsertItem<SessionItem>();
  const InsertItemCommand command(std::move(to_insert), parent, expected_tag_index);

  const auto expected_event1 =
      std::optional{event_variant_t{AboutToInsertItemEvent{parent, expected_tag_index}}};

  EXPECT_EQ(command.GetResult(), nullptr);
  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  // event without actual command execution doesn't exist
  EXPECT_THROW(command.GetEventAfter(), RuntimeException);
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_TRUE(command.GetDescription().empty());
}

TEST_F(InsertItemCommandTests, InsertItemToParent)
{
  auto parent = m_model.InsertItem<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto to_insert = std::make_unique<PropertyItem>();
  auto to_insert_ptr = to_insert.get();
  auto identifier = to_insert->GetIdentifier();
  to_insert->SetData(42);

  const TagIndex expected_tag_index{"tag1", 0};
  InsertItemCommand command(std::move(to_insert), parent, expected_tag_index);

  const auto expected_event1 =
      std::optional{event_variant_t{AboutToInsertItemEvent{parent, expected_tag_index}}};
  const auto expected_event2 =
      std::optional{event_variant_t{ItemInsertedEvent{parent, expected_tag_index}}};
  const auto expected_event3 =
      std::optional{event_variant_t{AboutToRemoveItemEvent{parent, expected_tag_index}}};
  const auto expected_event4 =
      std::optional{event_variant_t{ItemRemovedEvent{parent, expected_tag_index}}};

  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  command.Execute();
  EXPECT_EQ(command.GetEventAfter(), expected_event2);
  EXPECT_FALSE(command.IsObsolete());

  EXPECT_EQ(command.GetResult(), to_insert_ptr);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(parent->FindItem(expected_tag_index), to_insert_ptr);
  EXPECT_EQ(parent->GetItem(expected_tag_index).Data<int>(), 42);
  EXPECT_EQ(command.GetResult()->GetParent(), parent);
  EXPECT_EQ(command.GetResult()->GetModel(), &m_model);

  EXPECT_EQ(command.GetEventBefore(), expected_event3);
  command.Undo();
  EXPECT_EQ(command.GetEventAfter(), expected_event4);
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(parent->GetTotalItemCount(), 0);

  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  command.Execute();
  EXPECT_EQ(command.GetEventAfter(), expected_event2);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetIdentifier(), identifier);
  EXPECT_EQ(parent->GetItem(expected_tag_index).Data<int>(), 42);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetParent(), parent);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetModel(), &m_model);
}

TEST_F(InsertItemCommandTests, InsertItemToParentInBetween)
{
  SessionModel model;
  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto child0 = model.InsertItem<SessionItem>(parent, TagIndex::Append());
  auto child1 = model.InsertItem<SessionItem>(parent, TagIndex::Append());

  auto to_insert = std::make_unique<PropertyItem>();
  auto to_insert_ptr = to_insert.get();
  auto identifier = to_insert->GetIdentifier();
  to_insert->SetData(42);

  const TagIndex expected_tag_index{"tag1", 1};
  InsertItemCommand command(std::move(to_insert), parent, expected_tag_index);

  const auto expected_event1 =
      std::optional{event_variant_t{AboutToInsertItemEvent{parent, expected_tag_index}}};
  const auto expected_event2 =
      std::optional{event_variant_t{ItemInsertedEvent{parent, expected_tag_index}}};
  const auto expected_event3 =
      std::optional{event_variant_t{AboutToRemoveItemEvent{parent, expected_tag_index}}};
  const auto expected_event4 =
      std::optional{event_variant_t{ItemRemovedEvent{parent, expected_tag_index}}};

  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  command.Execute();
  EXPECT_EQ(command.GetEventAfter(), expected_event2);
  EXPECT_FALSE(command.IsObsolete());

  EXPECT_EQ(command.GetResult(), to_insert_ptr);
  EXPECT_EQ(parent->GetTotalItemCount(), 3);
  EXPECT_EQ(parent->FindItem(expected_tag_index), to_insert_ptr);
  EXPECT_EQ(parent->GetItem(expected_tag_index).Data<int>(), 42);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetParent(), parent);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetModel(), &model);

  EXPECT_EQ(command.GetEventBefore(), expected_event3);
  command.Undo();
  EXPECT_EQ(command.GetEventAfter(), expected_event4);
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(parent->GetTotalItemCount(), 2);
  EXPECT_EQ(model.FindItem(identifier), nullptr);

  EXPECT_EQ(command.GetEventBefore(), expected_event1);
  command.Execute();
  EXPECT_EQ(command.GetEventAfter(), expected_event2);
  EXPECT_EQ(parent->GetTotalItemCount(), 3);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetIdentifier(), identifier);
  EXPECT_EQ(parent->GetItem(expected_tag_index).Data<int>(), 42);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetParent(), parent);
  EXPECT_EQ(parent->GetItem(expected_tag_index).GetModel(), &model);
  EXPECT_EQ(model.FindItem(identifier), parent->FindItem(expected_tag_index));
}

TEST_F(InsertItemCommandTests, AttemptToInsertSecondProperty)
{
  SessionModel model;
  auto parent = model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);
  parent->AddProperty("thickness", 42);

  auto to_insert = std::make_unique<PropertyItem>();

  InsertItemCommand command(std::move(to_insert), parent, TagIndex{"thickness", 1});

  EXPECT_THROW(command.Execute(), InvalidOperationException);
}

}  // namespace mvvm::test
