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

#include "mvvm/model/item_factory.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/standarditems/standard_item_includes.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing ItemFactory class and registrations in the global factory.
//! More tests in item_catalogue_tests.cpp

class ItemFactoryTests : public ::testing::Test
{
public:
  /**
   * @brief Returns true if given item can be casted to specified type.
   */
  template <typename T>
  bool IsCorrectType(const mvvm::SessionItem* item)
  {
    return dynamic_cast<const T*>(item) != nullptr;
  }

  /**
   * @brief Returns true if clone is correctly implemented.
   */
  template <typename T>
  bool IsCloneImplemented()
  {
    T item;
    auto clone = item.Clone();
    return IsCorrectType<T>(clone.get());
  }

  /**
   * @brief Returns true if given item is registered in the catalogue.
   */
  template <typename T>
  bool CanCreateCorrectType(const ItemFactory& factory)
  {
    auto item = factory.CreateItem(T::Type);
    return IsCorrectType<T>(item.get());
  }

  class TestItem : public SessionItem
  {
  public:
    static inline const std::string Type = "ItemFactoryTestItem";
    TestItem() : SessionItem(Type) {}
  };
};

//! Check for SessionItem::Clone.

TEST_F(ItemFactoryTests, CheckCloneImplementation)
{
  EXPECT_TRUE(IsCloneImplemented<CompoundItem>());
  EXPECT_TRUE(IsCloneImplemented<ContainerItem>());
  EXPECT_TRUE(IsCloneImplemented<Data1DItem>());
  EXPECT_TRUE(IsCloneImplemented<FixedBinAxisItem>());
  EXPECT_TRUE(IsCloneImplemented<GraphItem>());
  EXPECT_TRUE(IsCloneImplemented<GraphViewportItem>());
  EXPECT_TRUE(IsCloneImplemented<LinkedItem>());
  EXPECT_TRUE(IsCloneImplemented<PenItem>());
  EXPECT_TRUE(IsCloneImplemented<PointwiseAxisItem>());
  EXPECT_TRUE(IsCloneImplemented<PropertyItem>());
  EXPECT_TRUE(IsCloneImplemented<TextItem>());
  EXPECT_TRUE(IsCloneImplemented<VectorItem>());
  EXPECT_TRUE(IsCloneImplemented<ViewportAxisItem>());
}

//! Another check for SessionItem::Clone.

TEST_F(ItemFactoryTests, CloneOfItemsRegisteredInCatalogue)
{
  const auto& factory = GetGlobalItemFactory();

  for (const auto& item_type : factory.GetItemTypes())
  {
    auto item = factory.CreateItem(item_type);
    auto clone = item->Clone();
    EXPECT_EQ(item->GetType(), clone->GetType());
  }
}

TEST_F(ItemFactoryTests, GetGlobalItemFactory)
{
  const auto& factory = GetGlobalItemFactory();

  EXPECT_TRUE(CanCreateCorrectType<CompoundItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PropertyItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<ContainerItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<SessionItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<LinkedItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<VectorItem>(factory));

  EXPECT_TRUE(CanCreateCorrectType<Data1DItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<FixedBinAxisItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<GraphItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<GraphViewportItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PenItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PointwiseAxisItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<TextItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<ViewportAxisItem>(factory));
}

TEST_F(ItemFactoryTests, RegisteringItem)
{
  auto& factory = GetGlobalItemFactory();
  EXPECT_THROW(factory.CreateItem(TestItem::Type), KeyNotFoundException);
  const size_t registration_count = factory.GetItemTypes().size();

  factory.RegisterItem<TestItem>();
  EXPECT_TRUE(CanCreateCorrectType<TestItem>(factory));
  EXPECT_TRUE(factory.IsRegistered(TestItem::Type));
  EXPECT_TRUE(factory.IsRegistered<TestItem>());

  EXPECT_NO_THROW(factory.CreateItem(TestItem::Type));
  EXPECT_EQ(factory.GetItemTypes().size(), registration_count + 1);
}
