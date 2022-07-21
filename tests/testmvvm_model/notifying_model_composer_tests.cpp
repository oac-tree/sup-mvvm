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

#include "mock_model.h"
#include "mock_model_event_notifier.h"
#include "mvvm/model/notifying_model_composer.h"

#include <gtest/gtest.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/signals/model_event_notifier.h>

using namespace mvvm;
using ::testing::_;

//! Testing NotifyingModelComposer class.

class NotifyingModelComposerTests : public ::testing::Test
{
public:
  std::unique_ptr<NotifyingModelComposer<ModelComposer>> CreateComposer()
  {
    auto notifier = std::make_unique<MockModelEventNotifierDecorator>(&m_notifier);
    return std::make_unique<NotifyingModelComposer<ModelComposer>>(std::move(notifier), m_model);
  }

  MockModel m_model;
  MockModelEventNotifier m_notifier;
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

  auto child = std::make_unique<SessionItem>();
  auto p_child = child.get();

  EXPECT_CALL(m_notifier, AboutToInsertItemNotify(parent.get(), expected_tagindex)).Times(1);
  EXPECT_CALL(m_notifier, ItemInsertedNotify(parent.get(), expected_tagindex)).Times(1);

  // inserting child
  auto inserted = composer->InsertItem(std::move(child), parent.get(), {"", 0});

  EXPECT_EQ(inserted, p_child);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem("", 0), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());
}

TEST_F(NotifyingModelComposerTests, SetData)
{
  SessionItem expected_item;
  int expected_role{DataRole::kData};
  auto composer = CreateComposer();

  EXPECT_CALL(m_notifier, DataChangedNotify(&expected_item, expected_role)).Times(1);

  EXPECT_TRUE(composer->SetData(&expected_item, 42, expected_role));
}

TEST_F(NotifyingModelComposerTests, SetSameData)
{
  SessionItem expected_item;
  int expected_role{DataRole::kData};
  expected_item.SetData(42, expected_role);
  auto composer = CreateComposer();

  EXPECT_CALL(m_notifier, DataChangedNotify(_, _)).Times(0);

  EXPECT_FALSE(composer->SetData(&expected_item, 42, expected_role));
}
