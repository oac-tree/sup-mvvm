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

#include "mvvm/factories/item_copy_strategy_factory.h"

#include <mvvm/factories/item_catalogue_factory.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing SessionItem copy strategies.

class ItemCopyStrategyFactoryTests : public ::testing::Test
{
public:
  ItemCopyStrategyFactoryTests()
      : m_factory(std::make_unique<ItemFactory>(CreateStandardItemCatalogue()))
  {
  }

  std::unique_ptr<ItemFactory> m_factory;
};

//! Copy PropertyItem.

TEST_F(ItemCopyStrategyFactoryTests, CopyPropertyItem)
{
  auto strategy = CreateItemCopyStrategy(m_factory.get());

  PropertyItem item;
  item.SetData(42.0);

  auto copy = strategy->CreateCopy(&item);

  EXPECT_EQ(copy->GetType(), item.GetType());
  EXPECT_EQ(copy->GetDisplayName(), item.GetDisplayName());
  EXPECT_NE(copy->GetIdentifier(), item.GetIdentifier());  // different identifiers
  EXPECT_EQ(copy->Data(), item.Data());
}

//! Clone PropertyItem.

TEST_F(ItemCopyStrategyFactoryTests, ClonePropertyItem)
{
  auto strategy = CreateItemCloneStrategy(m_factory.get());

  PropertyItem item;
  item.SetData(42.0);

  auto copy = strategy->CreateCopy(&item);

  EXPECT_EQ(copy->GetType(), item.GetType());
  EXPECT_EQ(copy->GetDisplayName(), item.GetDisplayName());
  EXPECT_EQ(copy->GetIdentifier(), item.GetIdentifier());  // same identifiers
  EXPECT_EQ(copy->Data(), item.Data());
}

//! Copy CustomItem.

TEST_F(ItemCopyStrategyFactoryTests, CopyCustomItem)
{
  auto strategy = CreateItemCopyStrategy(m_factory.get());

  // creating parent with one child
  auto parent = std::make_unique<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child = parent->InsertItem(std::make_unique<SessionItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // creating copy
  auto parent_copy = strategy->CreateCopy(parent.get());

  EXPECT_EQ(parent_copy->GetTotalItemCount(), 1);
  EXPECT_EQ(parent_copy->GetDisplayName(), "parent_name");
  EXPECT_EQ(parent_copy->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(parent_copy->GetModel(), nullptr);
  EXPECT_NE(parent_copy->GetIdentifier(), parent->GetIdentifier()); // different identifiers

  // checking child reconstruction
  auto child_copy = parent_copy->GetItem("defaultTag");
  EXPECT_EQ(child_copy->GetParent(), parent_copy.get());
  EXPECT_EQ(child_copy->GetTotalItemCount(), 0);
  EXPECT_EQ(child_copy->GetDisplayName(), "child_name");
  EXPECT_EQ(child_copy->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_NE(child_copy->GetIdentifier(), child->GetIdentifier()); // different identifiers
}

//! Clone CustomItem.

TEST_F(ItemCopyStrategyFactoryTests, CloneCustomItem)
{
  auto strategy = CreateItemCloneStrategy(m_factory.get());

  // creating parent with one child
  auto parent = std::make_unique<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child = parent->InsertItem(std::make_unique<SessionItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // creating copy
  auto parent_copy = strategy->CreateCopy(parent.get());

  EXPECT_EQ(parent_copy->GetTotalItemCount(), 1);
  EXPECT_EQ(parent_copy->GetDisplayName(), "parent_name");
  EXPECT_EQ(parent_copy->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(parent_copy->GetModel(), nullptr);
  EXPECT_EQ(parent_copy->GetIdentifier(), parent->GetIdentifier()); // same identifiers

  // checking child reconstruction
  auto child_copy = parent_copy->GetItem("defaultTag");
  EXPECT_EQ(child_copy->GetParent(), parent_copy.get());
  EXPECT_EQ(child_copy->GetTotalItemCount(), 0);
  EXPECT_EQ(child_copy->GetDisplayName(), "child_name");
  EXPECT_EQ(child_copy->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(child_copy->GetIdentifier(), child->GetIdentifier()); // same identifiers
}
