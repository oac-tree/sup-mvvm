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

#include "mvvm/factories/item_backup_strategy_factory.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>

namespace
{
class TestItem : public mvvm::CompoundItem
{
public:
  TestItem() : CompoundItem(mvvm::GetTypeName<TestItem>())
  {
    SetDisplayName("parent_name");
    RegisterTag(mvvm::TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
    auto child = InsertItem(std::make_unique<SessionItem>(), mvvm::TagIndex::Append());
    child->SetDisplayName("child_name");
  }
};

}  // namespace

template <>
struct mvvm::item_traits<TestItem>
{
  static constexpr std::string_view type_name() noexcept { return "Test"; }
};

namespace mvvm::test
{

/**
 * @brief Tests for various backup strategies.
 */
class StandardItemBackupStrategiesTests : public ::testing::Test
{
public:
  static IItemFactory* GetFactory() { return &GetGlobalItemFactory(); }

  StandardItemBackupStrategiesTests() { RegisterGlobalItem<TestItem>(); }
  SessionModel m_model;
};

TEST_F(StandardItemBackupStrategiesTests, TreeDataItemBackupStrategyForPropertyItem)
{
  auto strategy = CreateItemTreeDataBackupStrategy(GetFactory());

  PropertyItem item;
  item.SetData(42.0);

  // restoring item without presave doesn't work
  EXPECT_THROW(strategy->RestoreItem(), InvalidOperationException);

  strategy->SaveItem(item);
  auto restored = strategy->RestoreItem();

  // modifying data of the original item
  item.SetData(43.0);

  EXPECT_EQ(restored->GetType(), item.GetType());
  EXPECT_EQ(restored->GetDisplayName(), item.GetDisplayName());
  EXPECT_EQ(restored->GetIdentifier(), item.GetIdentifier());  // same identifiers
  EXPECT_DOUBLE_EQ(restored->Data<double>(), 42.0);            // previous data
}

TEST_F(StandardItemBackupStrategiesTests, ItemCloneBackupStrategyForPropertyItem)
{
  auto strategy = CreateItemCloneBackupStrategy();

  PropertyItem item;
  item.SetData(42.0);

  // restoring item without presave doesn't work
  EXPECT_THROW(strategy->RestoreItem(), InvalidOperationException);

  strategy->SaveItem(item);
  auto restored = strategy->RestoreItem();

  // modifying data of the original item
  item.SetData(43.0);

  EXPECT_EQ(restored->GetType(), item.GetType());
  EXPECT_EQ(restored->GetDisplayName(), item.GetDisplayName());
  EXPECT_EQ(restored->GetIdentifier(), item.GetIdentifier());  // same identifiers
  EXPECT_DOUBLE_EQ(restored->Data<double>(), 42.0);            // previous data
}

TEST_F(StandardItemBackupStrategiesTests, TreeDataItemBackupStrategyForCompoundItem)
{
  auto strategy = CreateItemTreeDataBackupStrategy(GetFactory());

  CompoundItem item;
  auto& property = item.AddProperty("thickness", 42.0);

  strategy->SaveItem(item);
  auto restored = strategy->RestoreItem();

  EXPECT_EQ(item.GetType(), restored->GetType());
  EXPECT_EQ(item.GetIdentifier(), restored->GetIdentifier());
  EXPECT_EQ(restored->GetItem("thickness")->Data<double>(), property.Data<double>());
  EXPECT_EQ(restored->GetItem("thickness")->GetIdentifier(), property.GetIdentifier());
}

TEST_F(StandardItemBackupStrategiesTests, ItemCloneBackupStrategyForCompoundItem)
{
  auto strategy = CreateItemCloneBackupStrategy();

  CompoundItem item;
  auto& property = item.AddProperty("thickness", 42.0);

  strategy->SaveItem(item);
  auto restored = strategy->RestoreItem();

  EXPECT_EQ(item.GetType(), restored->GetType());
  EXPECT_EQ(item.GetIdentifier(), restored->GetIdentifier());
  EXPECT_EQ(restored->GetItem("thickness")->Data<double>(), property.Data<double>());
  EXPECT_EQ(restored->GetItem("thickness")->GetIdentifier(), property.GetIdentifier());
}

TEST_F(StandardItemBackupStrategiesTests, TreeDataItemBackupStrategyForCustomItem)
{
  auto strategy = CreateItemTreeDataBackupStrategy(GetFactory());

  TestItem item;

  // creating copy
  strategy->SaveItem(item);
  auto restored = strategy->RestoreItem();

  EXPECT_NE(dynamic_cast<TestItem*>(&item), nullptr);
  EXPECT_EQ(restored->GetTotalItemCount(), 1);
  EXPECT_EQ(restored->GetDisplayName(), "parent_name");
  EXPECT_EQ(restored->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(restored->GetModel(), nullptr);
  EXPECT_EQ(restored->GetIdentifier(), item.GetIdentifier());  // same identifiers

  // checking child reconstruction
  auto restored_child = restored->GetItem("defaultTag");
  EXPECT_EQ(restored_child->GetParent(), restored.get());
  EXPECT_EQ(restored_child->GetTotalItemCount(), 0);
  EXPECT_EQ(restored_child->GetDisplayName(), "child_name");
  EXPECT_EQ(restored_child->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(restored_child->GetIdentifier(),
            item.GetItem("defaultTag")->GetIdentifier());  // same identifiers
}

TEST_F(StandardItemBackupStrategiesTests, ItemCloneStrategyForCustomItem)
{
  auto strategy = CreateItemCloneBackupStrategy();

  TestItem item;

  // creating copy
  strategy->SaveItem(item);
  auto restored = strategy->RestoreItem();

  EXPECT_NE(dynamic_cast<TestItem*>(&item), nullptr);
  EXPECT_EQ(restored->GetTotalItemCount(), 1);
  EXPECT_EQ(restored->GetDisplayName(), "parent_name");
  EXPECT_EQ(restored->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(restored->GetModel(), nullptr);
  EXPECT_EQ(restored->GetIdentifier(), item.GetIdentifier());  // same identifiers

  // checking child reconstruction
  auto restored_child = restored->GetItem("defaultTag");
  EXPECT_EQ(restored_child->GetParent(), restored.get());
  EXPECT_EQ(restored_child->GetTotalItemCount(), 0);
  EXPECT_EQ(restored_child->GetDisplayName(), "child_name");
  EXPECT_EQ(restored_child->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(restored_child->GetIdentifier(),
            item.GetItem("defaultTag")->GetIdentifier());  // same identifiers
}

}  // namespace mvvm::test
