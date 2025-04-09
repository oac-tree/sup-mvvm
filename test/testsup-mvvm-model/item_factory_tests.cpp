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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/item_factory.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/item_catalogue.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing ItemFactory class and registrations in the global factory.
//! More tests in item_catalogue_tests.cpp

class ItemFactoryTests : public ::testing::Test
{
public:
  /**
   * @brief Returns true if given item is registered in the catalogue.
   */
  template <typename T>
  bool CanCreateCorrectType(const ItemFactory& factory)
  {
    auto item = factory.CreateItem(T::GetStaticType());
    return test::CanCast<T>(item.get());
  }

  class TestItem : public SessionItem
  {
  public:
    TestItem() : SessionItem(GetStaticType()) {}
    static std::string GetStaticType() { return "ItemFactoryTestItem"; }
  };
};

TEST_F(ItemFactoryTests, CheckCloneImplementation)
{
  EXPECT_TRUE(test::IsCloneImplemented<ChartViewportItem>());
  EXPECT_TRUE(test::IsCloneImplemented<CompoundItem>());
  EXPECT_TRUE(test::IsCloneImplemented<ContainerItem>());
  EXPECT_TRUE(test::IsCloneImplemented<Data1DItem>());
  EXPECT_TRUE(test::IsCloneImplemented<FixedBinAxisItem>());
  EXPECT_TRUE(test::IsCloneImplemented<GraphItem>());
  EXPECT_TRUE(test::IsCloneImplemented<GraphViewportItem>());
  EXPECT_TRUE(test::IsCloneImplemented<LineSeriesDataItem>());
  EXPECT_TRUE(test::IsCloneImplemented<LineSeriesItem>());
  EXPECT_TRUE(test::IsCloneImplemented<LinkedItem>());
  EXPECT_TRUE(test::IsCloneImplemented<PenItem>());
  EXPECT_TRUE(test::IsCloneImplemented<PointItem>());
  EXPECT_TRUE(test::IsCloneImplemented<PointwiseAxisItem>());
  EXPECT_TRUE(test::IsCloneImplemented<PropertyItem>());
  EXPECT_TRUE(test::IsCloneImplemented<SessionItem>());
  EXPECT_TRUE(test::IsCloneImplemented<TextItem>());
  EXPECT_TRUE(test::IsCloneImplemented<VectorItem>());
  EXPECT_TRUE(test::IsCloneImplemented<ViewportAxisItem>());
}

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

  EXPECT_TRUE(CanCreateCorrectType<ChartViewportItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<CompoundItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<ContainerItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<Data1DItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<FixedBinAxisItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<GraphItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<GraphViewportItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<LineSeriesDataItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<LineSeriesItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<LinkedItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PenItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PointItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PointwiseAxisItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<PropertyItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<SessionItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<TextItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<VectorItem>(factory));
  EXPECT_TRUE(CanCreateCorrectType<ViewportAxisItem>(factory));
}

TEST_F(ItemFactoryTests, Construction)
{
  auto catalogue = std::make_unique<ItemCatalogue<SessionItem>>();
  auto catalogue_ptr = catalogue.get();

  ItemFactory factory(std::move(catalogue));

  // it is not possible to register empty name as type
  EXPECT_THROW(factory.RegisterItem({}, {}, {}), RuntimeException);

  // it is not possible to for absent factory function
  EXPECT_THROW(factory.RegisterItem("TestType", {}, {}), RuntimeException);
}

TEST_F(ItemFactoryTests, RegisteringItem)
{
  auto& factory = GetGlobalItemFactory();
  EXPECT_THROW(factory.CreateItem(TestItem::GetStaticType()), KeyNotFoundException);
  const size_t registration_count = factory.GetItemTypes().size();

  factory.RegisterItem<TestItem>();
  EXPECT_TRUE(CanCreateCorrectType<TestItem>(factory));
  EXPECT_TRUE(factory.IsRegistered(TestItem::GetStaticType()));
  EXPECT_TRUE(factory.IsRegistered<TestItem>());

  EXPECT_NO_THROW(factory.CreateItem(TestItem::GetStaticType()));
  EXPECT_EQ(factory.GetItemTypes().size(), registration_count + 1);
}
