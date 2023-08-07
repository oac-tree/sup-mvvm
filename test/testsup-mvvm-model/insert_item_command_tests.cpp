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

#include "mvvm/commands/insert_item_command.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/signals/model_event_handler.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_event_listener.h>

using namespace mvvm;
using ::testing::_;

//! Testing InsertItemCommand.

class InsertItemCommandTests : public ::testing::Test
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

  InsertItemCommandTests() { m_listener.SubscribeAll(&m_event_handler); }

  SessionModel m_model;
  ModelEventHandler m_event_handler;
  MockEventListener m_listener;
};

//! Insert item to root item.

TEST_F(InsertItemCommandTests, InsertItemToRoot)
{
  auto composer = CreateStandardComposer();

  auto to_insert = std::make_unique<PropertyItem>();
  auto identifier = to_insert->GetIdentifier();
  to_insert->SetData(42);
  auto to_insert_ptr = to_insert.get();

  // command to insert item from the model
  InsertItemCommand command(composer.get(), std::move(to_insert), m_model.GetRootItem(),
                            TagIndex::Default(0));

  // model is still empty
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  command.Execute();
  EXPECT_FALSE(command.IsObsolete());

  EXPECT_EQ(command.GetResult(), to_insert_ptr);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);

  auto inserted = utils::ChildAt(m_model.GetRootItem(), 0);
  EXPECT_EQ(inserted, to_insert_ptr);
  EXPECT_EQ(inserted->Data<int>(), 42);

  // undoing command
  command.Undo();
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // redoing back
  command.Execute();
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::ChildAt(m_model.GetRootItem(), 0)->Data<int>(), 42);
  EXPECT_EQ(utils::ChildAt(m_model.GetRootItem(), 0)->GetIdentifier(), identifier);
}

//! Insert item to parent.

TEST_F(InsertItemCommandTests, InsertItemToParent)
{
  auto composer = CreateStandardComposer();

  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  m_model.InsertItem<SessionItem>(parent);
  m_model.InsertItem<SessionItem>(parent);

  auto to_insert = std::make_unique<PropertyItem>();
  auto identifier = to_insert->GetIdentifier();
  to_insert->SetData(42);
  auto to_insert_ptr = to_insert.get();

  // command to insert item from the model
  InsertItemCommand command(composer.get(), std::move(to_insert), parent, TagIndex{"tag1", 1});

  command.Execute();
  EXPECT_FALSE(command.IsObsolete());

  EXPECT_EQ(command.GetResult(), to_insert_ptr);
  EXPECT_EQ(parent->GetTotalItemCount(), 3);
  EXPECT_EQ(parent->GetItem({"tag1", 1}), to_insert_ptr);
  EXPECT_EQ(parent->GetItem({"tag1", 1})->Data<int>(), 42);

  // undoing command
  command.Undo();
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(parent->GetTotalItemCount(), 2);

  // redoing back
  command.Execute();
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(parent->GetTotalItemCount(), 3);
  EXPECT_EQ(parent->GetItem({"tag1", 1})->Data<int>(), 42);
  EXPECT_EQ(parent->GetItem({"tag1", 1})->GetIdentifier(), identifier);
}

//! Insert item to parent.
//! Notifying composer is used.

TEST_F(InsertItemCommandTests, InsertItemToParentWithNotifyingComposer)
{
  const mvvm::TagIndex tag_index{"tag1", 1};

  auto composer = CreateNotifyingComposer();

  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  AboutToInsertItemEvent about_to_insert_event{parent, tag_index};
  ItemInsertedEvent item_inserted_event{parent, tag_index};
  AboutToRemoveItemEvent about_to_remove_event{parent, tag_index};
  ItemRemovedEvent item_removed_event{parent, tag_index};

  m_model.InsertItem<SessionItem>(parent);
  m_model.InsertItem<SessionItem>(parent);

  auto to_insert = std::make_unique<PropertyItem>();
  auto identifier = to_insert->GetIdentifier();
  to_insert->SetData(42);
  auto to_insert_ptr = to_insert.get();

  // command to insert item from the model
  InsertItemCommand command(composer.get(), std::move(to_insert), parent, tag_index);

  // expecting signals related to item insertion
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(about_to_insert_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(item_inserted_event))).Times(1);
  }

  command.Execute();
  EXPECT_FALSE(command.IsObsolete());

  EXPECT_EQ(parent->GetTotalItemCount(), 3);

  // expecting signals related to item insertion
  {
    ::testing::InSequence seq;

    EXPECT_CALL(m_listener, OnEvent(event_variant_t(about_to_remove_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(item_removed_event))).Times(1);
  }

  // undoing command
  command.Undo();
  EXPECT_FALSE(command.IsObsolete());
  EXPECT_EQ(parent->GetTotalItemCount(), 2);
}

//! Insert item to parent.

TEST_F(InsertItemCommandTests, AttemptToInsertSecondProperty)
{
  auto composer = CreateStandardComposer();

  auto compound = m_model.InsertItem<CompoundItem>(m_model.GetRootItem());
  compound->AddProperty("thickness", 42);

  auto to_insert = std::make_unique<PropertyItem>();

  // command to insert item from the model
  InsertItemCommand command(composer.get(), std::move(to_insert), compound,
                            TagIndex{"thickness", 1});

  EXPECT_THROW(command.Execute(), InvalidOperationException);
}
