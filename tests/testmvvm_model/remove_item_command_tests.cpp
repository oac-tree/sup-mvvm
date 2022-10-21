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

#include "mock_model_event_listener.h"
#include "mvvm/commands/remove_item_command.h"

#include <gtest/gtest.h>
#include <mvvm/core/exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/notifying_model_composer.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/taginfo.h>

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
    return std::make_unique<NotifyingModelComposer<ModelComposer>>(&m_event_handler, m_model);
  }

  RemoveItemCommandTests() { m_listener.SubscribeAll(&m_event_handler); }

  SessionModel m_model;
  ModelEventHandler m_event_handler;
  MockModelEventListener m_listener;
};

//! Remove item from the model.

TEST_F(RemoveItemCommandTests, RemoveItemFromRoot)
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

//! Remove item from the parent.

TEST_F(RemoveItemCommandTests, RemoveItemFromParent)
{
  auto composer = CreateStandardComposer();

  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  // inserting two children to the parent
  auto child1 = m_model.InsertItem<SessionItem>(parent);
  child1->SetData(42.0);
  m_model.InsertItem<SessionItem>(parent);

  auto child1_identifier = child1->GetIdentifier();

  // command to remove one child
  auto command = std::make_unique<RemoveItemCommand>(composer.get(), parent, TagIndex{"", 0});

  command->Execute();  // removal
  EXPECT_FALSE(command->IsObsolete());

  // check that one child was removed
  auto taken = command->GetResult();
  EXPECT_EQ(taken.get(), child1);

  EXPECT_EQ(parent->GetTotalItemCount(), 1);

  // undo command
  command->Undo();
  EXPECT_FALSE(command->IsObsolete());

  EXPECT_EQ(parent->GetTotalItemCount(), 2);
  auto restored = parent->GetItem("tag1", 0);
  EXPECT_EQ(restored->GetIdentifier(), child1_identifier);

  // checking the data of restored item
  EXPECT_DOUBLE_EQ(restored->Data<double>(), 42.0);
}

//! Removing parent with child.

TEST_F(RemoveItemCommandTests, RemoveParentWithChild)
{
  auto composer = CreateStandardComposer();

  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto child1 = m_model.InsertItem<SessionItem>(parent);
  child1->SetData(42.0);

  auto parent_identifier = parent->GetIdentifier();
  auto child1_identifier = child1->GetIdentifier();

  // command to remove parent
  auto command =
      std::make_unique<RemoveItemCommand>(composer.get(), m_model.GetRootItem(), TagIndex{"", 0});
  command->Execute();  // removal
  EXPECT_FALSE(command->IsObsolete());

  // check that one child was removed
  auto taken = command->GetResult();
  EXPECT_EQ(taken.get(), parent);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // undo command
  command->Undo();
  EXPECT_FALSE(command->IsObsolete());

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto restored_parent = utils::ChildAt(m_model.GetRootItem(), 0);
  auto restored_child = utils::ChildAt(restored_parent, 0);

  EXPECT_EQ(restored_parent->GetIdentifier(), parent_identifier);
  EXPECT_EQ(restored_child->GetIdentifier(), child1_identifier);

  // checking the data of restored item
  EXPECT_DOUBLE_EQ(restored_child->Data<double>(), 42.0);
}

//! Removing parent with child.
//! This time NotifyingModelComposer is used.

TEST_F(RemoveItemCommandTests, RemoveParentWithChildWIthNotifyingComposer)
{
  const TagIndex tag_index{"", 0};

  auto composer = CreateNotifyingComposer();

  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"), /*set_as_default*/ true);

  auto child1 = m_model.InsertItem<SessionItem>(parent);
  child1->SetData(42.0);

  auto parent_identifier = parent->GetIdentifier();
  auto child1_identifier = child1->GetIdentifier();

  AboutToRemoveItemEvent about_to_remove_event{m_model.GetRootItem(), tag_index};
  ItemRemovedEvent item_removed_event{m_model.GetRootItem(), tag_index};
  AboutToInsertItemEvent about_to_insert_event{m_model.GetRootItem(), tag_index};
  ItemInsertedEvent item_inserted_event{m_model.GetRootItem(), tag_index};

  // expecting signals related to item removal
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(about_to_remove_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(item_removed_event))).Times(1);
  }

  // command to remove parent
  auto command =
      std::make_unique<RemoveItemCommand>(composer.get(), m_model.GetRootItem(), TagIndex{"", 0});
  command->Execute();  // removal, triggering signals
  EXPECT_FALSE(command->IsObsolete());

  // check that one child was removed
  auto taken = command->GetResult();
  EXPECT_EQ(taken.get(), parent);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);

  // expecting signals related to item insertion
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(about_to_insert_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(item_inserted_event))).Times(1);
  }

  // undo command
  command->Undo();
  EXPECT_FALSE(command->IsObsolete());

  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto restored_parent = utils::ChildAt(m_model.GetRootItem(), 0);
  auto restored_child = utils::ChildAt(restored_parent, 0);

  EXPECT_EQ(restored_parent->GetIdentifier(), parent_identifier);
  EXPECT_EQ(restored_child->GetIdentifier(), child1_identifier);

  // checking the data of restored item
  EXPECT_DOUBLE_EQ(restored_child->Data<double>(), 42.0);
}

//! Remove child from one of tags.

TEST_F(RemoveItemCommandTests, RemoveChildFromMultiTag)
{
  auto composer = CreateStandardComposer();

  auto parent = m_model.InsertItem<SessionItem>(m_model.GetRootItem());
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag1"));
  parent->RegisterTag(TagInfo::CreateUniversalTag("tag2"));

  auto child1 = m_model.InsertItem<SessionItem>(parent, "tag1");
  child1->SetData(41.0);

  auto child2 = m_model.InsertItem<SessionItem>(parent, "tag1");
  child2->SetData(42.0);

  auto child3 = m_model.InsertItem<SessionItem>(parent, "tag2");
  child3->SetData(43.0);

  auto parent_identifier = parent->GetIdentifier();
  auto child1_identifier = child1->GetIdentifier();
  auto child2_identifier = child2->GetIdentifier();
  auto child3_identifier = child3->GetIdentifier();

  // command to remove parent
  auto command = std::make_unique<RemoveItemCommand>(composer.get(), parent, TagIndex{"tag1", 1});
  command->Execute();  // removal
  EXPECT_FALSE(command->IsObsolete());

  // check that one child was removed
  EXPECT_EQ(parent->GetTotalItemCount(), 2);

  // undo command
  command->Undo();
  EXPECT_FALSE(command->IsObsolete());
  EXPECT_EQ(parent->GetTotalItemCount(), 3);
  auto restored_parent = utils::ChildAt(m_model.GetRootItem(), 0);
  auto restored_child2 = utils::ChildAt(restored_parent, 1);

  EXPECT_EQ(restored_parent->GetIdentifier(), parent_identifier);
  EXPECT_EQ(restored_child2->GetIdentifier(), child2_identifier);

  // checking the data of restored item
  EXPECT_EQ(restored_child2->Data<double>(), 42.0);
}

TEST_F(RemoveItemCommandTests, AttemptToRemoveItem)
{
  auto composer = CreateStandardComposer();

  auto parent = m_model.InsertItem<CompoundItem>(m_model.GetRootItem());
  parent->AddProperty("thickness", 42.0);

  auto command =
      std::make_unique<RemoveItemCommand>(composer.get(), parent, TagIndex{"thickness", 0});

  EXPECT_THROW(command->Execute(), RuntimeException);
}
