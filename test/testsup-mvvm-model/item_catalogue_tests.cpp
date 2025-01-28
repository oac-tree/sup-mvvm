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

#include "mvvm/model/item_catalogue.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/property_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing ItemCatalogue class.

class ItemCatalogueTests : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    TestItem() : SessionItem(GetStaticType()) {}
    static std::string GetStaticType() { return "TestItem"; }
  };

  class AnotherTestItem : public SessionItem
  {
  public:
    AnotherTestItem() : SessionItem(GetStaticType()) {}
    static std::string GetStaticType() { return "AnotherTestItem"; }
  };
};

TEST_F(ItemCatalogueTests, InitialState)
{
  ItemCatalogue<SessionItem> catalogue;
  EXPECT_EQ(catalogue.GetItemCount(), 0);
  EXPECT_EQ(catalogue.GetItemTypes(), std::vector<std::string>({}));
  EXPECT_EQ(catalogue.GetLabels(), std::vector<std::string>({}));
}

TEST_F(ItemCatalogueTests, AddItem)
{
  ItemCatalogue<SessionItem> catalogue;

  catalogue.RegisterItem<PropertyItem>();

  EXPECT_EQ(catalogue.GetItemCount(), 1);

  auto item = catalogue.Create(PropertyItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // registration of second item is not allowed
  EXPECT_THROW(catalogue.RegisterItem<PropertyItem>(), ExistingKeyException);

  // item was not registered, creation not allowed
  EXPECT_THROW(catalogue.Create("non-registered"), KeyNotFoundException);

  // checking model types and labels
  EXPECT_EQ(catalogue.GetItemTypes(), std::vector<std::string>({PropertyItem::GetStaticType()}));
  EXPECT_EQ(catalogue.GetLabels(), std::vector<std::string>({""}));
}

TEST_F(ItemCatalogueTests, CopyConstructor)
{
  ItemCatalogue<SessionItem> catalogue;
  catalogue.RegisterItem<PropertyItem>();

  ItemCatalogue<SessionItem> copy(catalogue);

  // creation of item using first catalogue
  auto item = catalogue.Create(PropertyItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // creation of item using catalogue copy
  item = copy.Create(PropertyItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // checking model types and labels in new catalogue
  EXPECT_EQ(copy.GetItemTypes(), std::vector<std::string>({PropertyItem::GetStaticType()}));
  EXPECT_EQ(copy.GetLabels(), std::vector<std::string>({""}));

  // adding item to first catalogue but not the second
  catalogue.RegisterItem<TestItem>();
  item = catalogue.Create(TestItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<TestItem*>(item.get()) != nullptr);

  // copy of catalogue knows nothing about new VectorType
  EXPECT_THROW(copy.Create(TestItem::GetStaticType()), KeyNotFoundException);
}

TEST_F(ItemCatalogueTests, AssignmentOperator)
{
  ItemCatalogue<SessionItem> catalogue;
  catalogue.RegisterItem<PropertyItem>();

  ItemCatalogue<SessionItem> copy;
  copy = catalogue;

  // creation of item using first catalogue
  auto item = catalogue.Create(PropertyItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // creation of item using catalogue copy
  item = copy.Create(PropertyItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);
}

TEST_F(ItemCatalogueTests, IsRegistered)
{
  ItemCatalogue<SessionItem> catalogue;
  catalogue.RegisterItem<PropertyItem>();

  EXPECT_TRUE(catalogue.IsRegistered(PropertyItem::GetStaticType()));
  EXPECT_FALSE(catalogue.IsRegistered(TestItem::GetStaticType()));
}

TEST_F(ItemCatalogueTests, AddLabeledItem)
{
  ItemCatalogue<SessionItem> catalogue;
  catalogue.RegisterItem<PropertyItem>("property");
  catalogue.RegisterItem<TestItem>("test item");

  // checking model types and labels
  EXPECT_EQ(catalogue.GetItemTypes(),
            std::vector<std::string>({PropertyItem::GetStaticType(), TestItem::GetStaticType()}));
  EXPECT_EQ(catalogue.GetLabels(), std::vector<std::string>({"property", "test item"}));
}

TEST_F(ItemCatalogueTests, Merge)
{
  ItemCatalogue<SessionItem> catalogue1;
  catalogue1.RegisterItem<PropertyItem>("property");
  catalogue1.RegisterItem<TestItem>("test");

  ItemCatalogue<SessionItem> catalogue2;
  catalogue2.RegisterItem<AnotherTestItem>("another");

  // adding two catalogue together
  catalogue1.Merge(catalogue2);

  const std::vector<std::string> expected_models = {
      PropertyItem::GetStaticType(), TestItem::GetStaticType(), AnotherTestItem::GetStaticType()};
  const std::vector<std::string> expected_labels = {"property", "test", "another"};

  EXPECT_EQ(catalogue1.GetItemTypes(), expected_models);
  EXPECT_EQ(catalogue1.GetLabels(), expected_labels);

  auto item = catalogue1.Create(AnotherTestItem::GetStaticType());
  EXPECT_TRUE(dynamic_cast<AnotherTestItem*>(item.get()) != nullptr);

  // duplications is not allowed
  EXPECT_THROW(catalogue1.Merge(catalogue2), mvvm::ExistingKeyException);
}
