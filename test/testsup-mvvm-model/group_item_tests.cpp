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

#include "mvvm/standarditems/group_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/combo_property.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/make_clone.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

namespace
{

constexpr auto kSphereLabel = "SphereLabel";
constexpr auto kCylinderLabel = "CylinderLabel";

//! Empty group item.
class EmptyGroupItem : public GroupItem
{
public:
  EmptyGroupItem() : GroupItem(GetTypeName<EmptyGroupItem>()) {}
};

//! SphereItem (content for GroupItem).
class SphereItem : public CompoundItem
{
public:
  SphereItem() : CompoundItem(GetTypeName<SphereItem>()) { (void)AddProperty("Radius", 8.0); }

  std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
};

//! CylinderItem (content for GroupItem).
class CylinderItem : public CompoundItem
{
public:
  CylinderItem() : CompoundItem(GetTypeName<CylinderItem>())
  {
    (void)AddProperty("Radius", 8.0);
    (void)AddProperty("Height", 10.0);
  }

  std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
};

//! Test group item holding sphere and cylinder items.
class TestGroupItem : public GroupItem
{
public:
  TestGroupItem() : GroupItem(GetTypeName<TestGroupItem>())
  {
    AddToGroup<SphereItem>(kSphereLabel);
    AddToGroup<CylinderItem>();  // to check usage of the default label
  }

  std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
};

class TestGroupItemWithPreselection : public GroupItem
{
public:
  TestGroupItemWithPreselection() : GroupItem(GetTypeName<TestGroupItemWithPreselection>())
  {
    AddToGroup<SphereItem>(kSphereLabel);
    AddToGroup<CylinderItem>(kCylinderLabel, /*make_selected*/ true);
  }

  std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
};

}  // namespace

}  // namespace mvvm::test

template <>
struct mvvm::item_traits<mvvm::test::EmptyGroupItem>
{
  static constexpr std::string_view type_name() noexcept { return "EmptyGroupItem"; }
};

template <>
struct mvvm::item_traits<mvvm::test::SphereItem>
{
  static constexpr std::string_view type_name() noexcept { return "SphereItem"; }
};

template <>
struct mvvm::item_traits<mvvm::test::CylinderItem>
{
  static constexpr std::string_view type_name() noexcept { return "CylinderItem"; }
};

template <>
struct mvvm::item_traits<mvvm::test::TestGroupItem>
{
  static constexpr std::string_view type_name() noexcept { return "TestGroupItem"; }
};

template <>
struct mvvm::item_traits<mvvm::test::TestGroupItemWithPreselection>
{
  static constexpr std::string_view type_name() noexcept { return "TestGroupItemWithPreselection"; }
};

namespace mvvm::test
{

/**
 * @brief Tests for GroupItem class.
 */
class GroupItemTests : public ::testing::Test
{
public:
};

//! Initial state of the empty group item.
TEST_F(GroupItemTests, EmptyGroupInitialState)
{
  EmptyGroupItem item;
  EXPECT_EQ(item.GetCurrentIndex(), -1);
  EXPECT_EQ(item.GetCurrentItem(), nullptr);
  EXPECT_EQ(item.GetCurrentType(), std::string());
  EXPECT_TRUE(item.HasData());
  EXPECT_EQ(item.GetTotalItemCount(), 0);
  EXPECT_THROW(item.SetCurrentType("abc"), RuntimeException);
}

//! Initial state of the TestGroupItem.
TEST_F(GroupItemTests, TestGroupItemInitialState)
{
  TestGroupItem item;

  EXPECT_EQ(item.GetCurrentIndex(), 0);
  ASSERT_TRUE(item.GetCurrentItem() != nullptr);
  EXPECT_EQ(item.GetCurrentType(), item.GetCurrentItem()->GetType());
  EXPECT_EQ(item.GetCurrentType(), GetTypeName<SphereItem>());
  ASSERT_EQ(item.GetTotalItemCount(), 2);

  // parent-child relationship
  EXPECT_EQ(item.GetAllItems().at(0)->GetParent(), &item);
  EXPECT_EQ(item.GetAllItems().at(1)->GetParent(), &item);

  // expected value in combo
  auto combo = item.Data<ComboProperty>();
  EXPECT_EQ(combo.GetCurrentIndex(), 0);
  EXPECT_EQ(combo.GetValues(),
            std::vector<std::string>({kSphereLabel, GetTypeName<CylinderItem>()}));
}

//! Switching current item via type.
TEST_F(GroupItemTests, SetCurrentType)
{
  TestGroupItem item;
  item.SetCurrentType(GetTypeName<CylinderItem>());

  EXPECT_EQ(item.GetCurrentIndex(), 1);
  ASSERT_TRUE(item.GetCurrentItem() != nullptr);
  EXPECT_EQ(item.GetCurrentType(), item.GetCurrentItem()->GetType());
  EXPECT_EQ(item.GetCurrentItem()->GetType(), GetTypeName<CylinderItem>());
  EXPECT_EQ(item.GetTotalItemCount(), 2);

  // expected value in combo
  auto combo = item.Data<ComboProperty>();
  EXPECT_EQ(combo.GetCurrentIndex(), 1);
  EXPECT_EQ(combo.GetValues(), std::vector<std::string>({kSphereLabel, GetTypeName<CylinderItem>()}));

  // setting non-existing type
  EXPECT_THROW(item.SetCurrentType("non-existing"), RuntimeException);
}

//! Accessing the current item through non-const accessor.
TEST_F(GroupItemTests, CurrentItemNonConst)
{
  TestGroupItem item;
  auto* sphere = dynamic_cast<CompoundItem*>(item.GetCurrentItem());
  ASSERT_TRUE(sphere != nullptr);
  EXPECT_TRUE(sphere->SetProperty("Radius", 42.0));
  EXPECT_EQ(sphere->Property<double>("Radius"), 42.0);
}

//! GroupItem behavior when inserted into the model.
TEST_F(GroupItemTests, InModelContext)
{
  SessionModel model;
  auto item = model.InsertItem<TestGroupItem>();
  ASSERT_TRUE(item != nullptr);

  EXPECT_EQ(item->GetCurrentIndex(), 0);
  ASSERT_TRUE(item->GetCurrentItem() != nullptr);
  EXPECT_EQ(item->GetCurrentType(), item->GetCurrentItem()->GetType());
  ASSERT_EQ(item->GetTotalItemCount(), 2);

  // parent-child relationship
  EXPECT_EQ(item->GetAllItems().at(0)->GetParent(), item);
  EXPECT_EQ(item->GetAllItems().at(1)->GetParent(), item);

  // model relationship
  EXPECT_EQ(item->GetAllItems().at(0)->GetModel(), &model);
  EXPECT_EQ(item->GetAllItems().at(1)->GetModel(), &model);
}

//! Switching current item by setting the data through the model.
TEST_F(GroupItemTests, SetDataInModelContext)
{
  SessionModel model;
  auto item = model.InsertItem<TestGroupItem>();
  ASSERT_TRUE(item != nullptr);

  auto combo = std::get<ComboProperty>(item->Data(DataRole::kData));
  EXPECT_EQ(combo.GetCurrentIndex(), 0);

  // setting through combo
  combo.SetCurrentIndex(1);
  EXPECT_TRUE(model.SetData(item, combo, DataRole::kData));

  EXPECT_EQ(item->GetCurrentIndex(), 1);
  EXPECT_EQ(item->GetCurrentType(), GetTypeName<CylinderItem>());
}

//! Cloning the group item.
TEST_F(GroupItemTests, Clone)
{
  TestGroupItem item;
  item.SetCurrentType(GetTypeName<CylinderItem>());

  auto clone = item.Clone();

  auto group_clone = dynamic_cast<GroupItem*>(clone.get());
  ASSERT_TRUE(group_clone != nullptr);
  EXPECT_EQ(group_clone->GetType(), "TestGroupItem");
  EXPECT_EQ(group_clone->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(group_clone->GetCurrentIndex(), 1);
  EXPECT_EQ(group_clone->GetCurrentType(), GetTypeName<CylinderItem>());
  ASSERT_EQ(group_clone->GetTotalItemCount(), 2);
}

TEST_F(GroupItemTests, GroupItemWithPreselection)
{
  const TestGroupItemWithPreselection item;
  EXPECT_EQ(item.GetCurrentIndex(), 1);
  ASSERT_TRUE(item.GetCurrentItem() != nullptr);
  EXPECT_EQ(item.GetCurrentType(), item.GetCurrentItem()->GetType());
  EXPECT_EQ(item.GetCurrentType(), GetTypeName<CylinderItem>());
}

//! Accessing the current item through the templated accessor casting to a concrete type.
TEST_F(GroupItemTests, GetCurrentItemTemplated)
{
  TestGroupItem item;  // SphereItem is current by default

  // non-const version returns a castable, mutable pointer
  auto* sphere = item.GetCurrentItem<SphereItem>();
  ASSERT_TRUE(sphere != nullptr);
  EXPECT_TRUE(sphere->SetProperty("Radius", 42.0));
  EXPECT_EQ(sphere->Property<double>("Radius"), 42.0);

  // const version returns a const pointer
  const TestGroupItem& const_item = item;
  const auto* const_sphere = const_item.GetCurrentItem<SphereItem>();
  ASSERT_TRUE(const_sphere != nullptr);
  EXPECT_EQ(const_sphere->Property<double>("Radius"), 42.0);

  // wrong type throws
  EXPECT_THROW((void)item.GetCurrentItem<CylinderItem>(), RuntimeException);

  // empty group returns nullptr regardless of requested type
  EmptyGroupItem empty;
  EXPECT_EQ(empty.GetCurrentItem<SphereItem>(), nullptr);
}

//! Setting the current item by type and getting it back cast to that type.
TEST_F(GroupItemTests, SetCurrentTypeTemplated)
{
  TestGroupItem item;  // SphereItem is current by default

  auto* cylinder = item.SetCurrentType<CylinderItem>();
  ASSERT_TRUE(cylinder != nullptr);
  EXPECT_EQ(item.GetCurrentIndex(), 1);
  EXPECT_EQ(item.GetCurrentType(), GetTypeName<CylinderItem>());
  EXPECT_EQ(cylinder, item.GetCurrentItem<CylinderItem>());

  // the returned pointer is usable as the concrete type
  EXPECT_TRUE(cylinder->SetProperty("Height", 5.0));
  EXPECT_EQ(cylinder->Property<double>("Height"), 5.0);

  // switching back
  auto* sphere = item.SetCurrentType<SphereItem>();
  ASSERT_TRUE(sphere != nullptr);
  EXPECT_EQ(item.GetCurrentIndex(), 0);
}

}  // namespace mvvm::test
