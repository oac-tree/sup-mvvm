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

#include "mvvm/factories/item_catalogue_factory.h"

#include <gtest/gtest.h>

#include <mvvm/standarditems/standard_item_includes.h>

using namespace mvvm;

#include <iostream>
#include <typeinfo>

class ItemCatalogueFactoryTests : public ::testing::Test
{
public:
  //! Returns true if given item can be casted to specified type.
  template <typename T>
  bool IsCorrectType(const mvvm::SessionItem* item)
  {
    return dynamic_cast<const T*>(item) != nullptr;
  }

  //! Returns trus if clone is correctly implemented.
  template <typename T>
  bool IsCloneImplemented()
  {
    T item;
    auto clone = item.Clone();
    return IsCorrectType<T>(clone.get());
  }
};

TEST_F(ItemCatalogueFactoryTests, CreateStandardItemCatalogue)
{
  auto catalogue = CreateStandardItemCatalogue();

  auto item = catalogue->Create(SessionItem::Type);
  EXPECT_TRUE(dynamic_cast<SessionItem*>(item.get()) != nullptr);

  item = catalogue->Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  item = catalogue->Create(VectorItem::Type);
  EXPECT_TRUE(dynamic_cast<VectorItem*>(item.get()) != nullptr);

  item = catalogue->Create(CompoundItem::Type);
  EXPECT_TRUE(dynamic_cast<CompoundItem*>(item.get()) != nullptr);
}

TEST_F(ItemCatalogueFactoryTests, AddStandardItemsToCatalogue)
{
  ItemCatalogue<SessionItem> catalogue;
  AddStandardItemsToCatalogue(catalogue);

  auto item = catalogue.Create(SessionItem::Type);
  EXPECT_TRUE(dynamic_cast<SessionItem*>(item.get()) != nullptr);

  item = catalogue.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  item = catalogue.Create(VectorItem::Type);
  EXPECT_TRUE(dynamic_cast<VectorItem*>(item.get()) != nullptr);

  item = catalogue.Create(CompoundItem::Type);
  EXPECT_TRUE(dynamic_cast<CompoundItem*>(item.get()) != nullptr);
}

//! This is poor man's check that we didn't forget to override SessionItem::Clone

TEST_F(ItemCatalogueFactoryTests, CloneOfItemsRegisteredInCatalogue)
{
  auto catalogue = CreateStandardItemCatalogue();

  for (const auto& item_type : catalogue->GetItemTypes())
  {
    auto item = catalogue->Create(item_type);
    auto clone = item->Clone();
    EXPECT_EQ(item->GetType(), clone->GetType());
  }
}

//! Another check for SessionItem::Clone

TEST_F(ItemCatalogueFactoryTests, CheckCloneImplementation)
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
