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

#include "mvvm/standarditems/linked_item.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_pool.h>
#include <mvvm/model/model_composer.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/test/mock_item_listener.h>

#include <gtest/gtest.h>

using namespace mvvm;
using ::testing::_;
using mock_listener_t = ::testing::StrictMock<mvvm::test::MockItemListener>;

//! LinkedItem tests.

class LinkedItemTests : public ::testing::Test
{
};

//! Initial state of item when it is created outside of model context.

TEST_F(LinkedItemTests, InitialState)
{
  LinkedItem item;
  EXPECT_EQ(item.Get(), nullptr);
  EXPECT_EQ(item.Data<std::string>(), std::string());
}

//! Link in single model context.

TEST_F(LinkedItemTests, SameModelContext)
{
  SessionModel model;
  auto item = model.InsertItem<PropertyItem>();
  auto link = model.InsertItem<LinkedItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  // setting the link
  link->SetLink(item);

  // now linked
  EXPECT_EQ(link->Get(), item);
  EXPECT_EQ(link->Data<std::string>(), item->GetIdentifier());
}

//! Link in different model context.

TEST_F(LinkedItemTests, DifferentModelContext)
{
  auto pool = std::make_shared<ItemPool>();

  SessionModel model1("TestModel1", pool);
  SessionModel model2("TestModel2", pool);

  auto item = model1.InsertItem<PropertyItem>();
  auto link = model2.InsertItem<LinkedItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  // setting the link
  link->SetLink(item);

  // now linked
  EXPECT_EQ(link->Get(), item);
}

TEST_F(LinkedItemTests, onSetLink)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  auto link = model.InsertItem<LinkedItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  mock_listener_t widget(link);

  auto expected_role = DataRole::kData;
  auto expected_item = link;

  DataChangedEvent expected_event{expected_item, expected_role};
  EXPECT_CALL(widget, OnDataChanged(expected_event)).Times(1);

  // making action
  link->SetLink(item);
}

//! Link in different model context.

TEST_F(LinkedItemTests, SetNullAsLink)
{
  auto pool = std::make_shared<ItemPool>();

  SessionModel model("TestModel", pool);
  auto link = model.InsertItem<LinkedItem>();
  auto item = model.InsertItem<PropertyItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  // setting link
  link->SetLink(item);
  EXPECT_EQ(link->Get(), item);

  // still null link
  link->SetLink(nullptr);
  EXPECT_EQ(link->Get(), nullptr);
  EXPECT_TRUE(utils::IsValid(link->Data()));
  EXPECT_EQ(link->Data<std::string>(), std::string());
}
