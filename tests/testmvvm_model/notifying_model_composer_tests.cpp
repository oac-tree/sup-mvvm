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

#include "mvvm/model/notifying_model_composer.h"

#include <gtest/gtest.h>
#include <testutils/mock_event_listener.h>
#include <testutils/mock_model.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/signals/model_event_handler.h>

using namespace mvvm;
using ::testing::_;

//! Testing NotifyingModelComposer class.

class NotifyingModelComposerTests : public ::testing::Test
{
public:
  std::unique_ptr<NotifyingModelComposer<ModelComposer>> CreateComposer()
  {
    return std::make_unique<NotifyingModelComposer<ModelComposer>>(&m_event_handler, m_model);
  }

  NotifyingModelComposerTests() { m_listener.SubscribeAll(&m_event_handler); }

  testutils::MockModel m_model;
  ModelEventHandler m_event_handler;
  MockEventListener m_listener;
};

TEST_F(NotifyingModelComposerTests, InitialState)
{
  auto composer = CreateComposer();

  EXPECT_EQ(composer->GetModel(), &m_model);
}

TEST_F(NotifyingModelComposerTests, InsertItem)
{
  TagIndex expected_tagindex{"", 0};
  auto composer = CreateComposer();

  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  AboutToInsertItemEvent about_to_insert_event{parent.get(), expected_tagindex};
  ItemInsertedEvent item_inserted_event{parent.get(), expected_tagindex};

  auto child = std::make_unique<SessionItem>();
  auto p_child = child.get();

  // expecting signals related to item insertion
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(about_to_insert_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(item_inserted_event))).Times(1);
  }

  // inserting child
  auto inserted = composer->InsertItem(std::move(child), parent.get(), {"", 0});

  EXPECT_EQ(inserted, p_child);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem({"", 0}), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());
}

TEST_F(NotifyingModelComposerTests, TakeItem)
{
  auto composer = CreateComposer();

  TagIndex expected_tagindex{"", 0};

  // preparing parent
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child = parent->InsertItem(TagIndex::Append());

  AboutToRemoveItemEvent about_to_remove_event{parent.get(), expected_tagindex};
  ItemRemovedEvent item_removed_event{parent.get(), expected_tagindex};

  EXPECT_EQ(parent->GetTotalItemCount(), 1);

  // expecting signals related to item removal
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(about_to_remove_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(item_removed_event))).Times(1);
  }

  // taking item via composer
  auto taken = composer->TakeItem(parent.get(), {"", 0});

  EXPECT_EQ(taken->GetParent(), nullptr);
  EXPECT_EQ(taken.get(), child);
}

TEST_F(NotifyingModelComposerTests, SetData)
{
  auto composer = CreateComposer();

  SessionItem expected_item;
  int expected_role{DataRole::kData};

  DataChangedEvent data_changed_event{&expected_item, expected_role};
  EXPECT_CALL(m_listener, OnEvent(event_variant_t(data_changed_event))).Times(1);

  EXPECT_TRUE(composer->SetData(&expected_item, 42, expected_role));
}

TEST_F(NotifyingModelComposerTests, SetSameData)
{
  SessionItem expected_item;
  int expected_role{DataRole::kData};
  expected_item.SetData(42, expected_role);
  auto composer = CreateComposer();

  EXPECT_CALL(m_listener, OnEvent(_)).Times(0);

  EXPECT_FALSE(composer->SetData(&expected_item, 42, expected_role));
}

TEST_F(NotifyingModelComposerTests, Reset)
{
  auto composer = CreateComposer();

  auto parent0 = std::make_unique<SessionItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child0 = parent0->InsertItem<PropertyItem>(TagIndex::Append());

  ModelAboutToBeResetEvent model_about_reset_event{&m_model};
  ModelResetEvent model_reset_event{&m_model};

  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(model_about_reset_event))).Times(1);
    EXPECT_CALL(m_listener, OnEvent(event_variant_t(model_reset_event))).Times(1);
  }

  composer->Reset(parent0, {});

  EXPECT_EQ(parent0->GetTotalItemCount(), 0);
  EXPECT_EQ(parent0->GetModel(), &m_model);
}
