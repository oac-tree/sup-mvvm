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

#include "mvvm/model/itemcatalogue.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/core/exceptions.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace mvvm;

//! Testing ItemCatalogue construction

class ItemCatalogueTest : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    static inline const std::string Type = "TestItem";
    TestItem() : SessionItem(Type) {}
  };

  class AnotherTestItem : public SessionItem
  {
  public:
    static inline const std::string Type = "AnotherTestItem";
    AnotherTestItem() : SessionItem(Type) {}
  };
};

TEST_F(ItemCatalogueTest, InitialState)
{
  ItemCatalogue catalogue;
  EXPECT_EQ(catalogue.GetItemCount(), 0);
  EXPECT_EQ(catalogue.GetItemTypes(), std::vector<std::string>({}));
  EXPECT_EQ(catalogue.GetLabels(), std::vector<std::string>({}));
}

TEST_F(ItemCatalogueTest, AddItem)
{
  ItemCatalogue catalogue;

  catalogue.RegisterItem<PropertyItem>();

  EXPECT_EQ(catalogue.GetItemCount(), 1);

  auto item = catalogue.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // registration of second item is not allowed
  EXPECT_THROW(catalogue.RegisterItem<PropertyItem>(), ExistingKeyException);

  // item was not registered, creation not allowed
  EXPECT_THROW(catalogue.Create("non-registered"), NotFoundKeyException);

  // checking model types and labels
  EXPECT_EQ(catalogue.GetItemTypes(), std::vector<std::string>({PropertyItem::Type}));
  EXPECT_EQ(catalogue.GetLabels(), std::vector<std::string>({""}));
}

TEST_F(ItemCatalogueTest, CopyConstructor)
{
  ItemCatalogue catalogue;
  catalogue.RegisterItem<PropertyItem>();

  ItemCatalogue copy(catalogue);

  // creation of item using first catalogue
  auto item = catalogue.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // creation of item using catalogue copy
  item = copy.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // checking model types and labels in new catalogue
  EXPECT_EQ(copy.GetItemTypes(), std::vector<std::string>({PropertyItem::Type}));
  EXPECT_EQ(copy.GetLabels(), std::vector<std::string>({""}));

  // adding item to first catalogue but not the second
  catalogue.RegisterItem<TestItem>();
  item = catalogue.Create(TestItem::Type);
  EXPECT_TRUE(dynamic_cast<TestItem*>(item.get()) != nullptr);

  // copy of catalogue knows nothing about new VectorType
  EXPECT_THROW(copy.Create(TestItem::Type), NotFoundKeyException);
}

TEST_F(ItemCatalogueTest, AssignmentOperator)
{
  ItemCatalogue catalogue;
  catalogue.RegisterItem<PropertyItem>();

  ItemCatalogue copy;
  copy = catalogue;

  // creation of item using first catalogue
  auto item = catalogue.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // creation of item using catalogue copy
  item = copy.Create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);
}

TEST_F(ItemCatalogueTest, Contains)
{
  ItemCatalogue catalogue;
  catalogue.RegisterItem<PropertyItem>();

  EXPECT_TRUE(catalogue.Contains(PropertyItem::Type));
  EXPECT_FALSE(catalogue.Contains(TestItem::Type));
}

// FIXME uncomment
// TEST_F(ItemCatalogueTest, defaultItemCatalogue)
//{
//  auto catalogue = CreateStandardItemCatalogue();

//  auto item = catalogue->create(Constants::BaseType);
//  EXPECT_TRUE(dynamic_cast<SessionItem*>(item.get()) != nullptr);

//  item = catalogue->create(Constants::PropertyType);
//  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

//  item = catalogue->create(Constants::VectorItemType);
//  EXPECT_TRUE(dynamic_cast<VectorItem*>(item.get()) != nullptr);

//  item = catalogue->create(Constants::CompoundItemType);
//  EXPECT_TRUE(dynamic_cast<CompoundItem*>(item.get()) != nullptr);
//}

TEST_F(ItemCatalogueTest, AddLabeledItem)
{
  ItemCatalogue catalogue;
  catalogue.RegisterItem<PropertyItem>("property");
  catalogue.RegisterItem<TestItem>("test item");

  // checking model types and labels
  EXPECT_EQ(catalogue.GetItemTypes(),
            std::vector<std::string>({PropertyItem::Type, TestItem::Type}));
  EXPECT_EQ(catalogue.GetLabels(), std::vector<std::string>({"property", "test item"}));
}

TEST_F(ItemCatalogueTest, Merge)
{
  ItemCatalogue catalogue1;
  catalogue1.RegisterItem<PropertyItem>("property");
  catalogue1.RegisterItem<TestItem>("test");

  ItemCatalogue catalogue2;
  catalogue2.RegisterItem<AnotherTestItem>("another");

  // adding two catalogue together
  catalogue1.merge(catalogue2);

  std::vector<std::string> expected_models = {PropertyItem::Type, TestItem::Type,
                                              AnotherTestItem::Type};
  std::vector<std::string> expected_labels = {"property", "test", "another"};

  EXPECT_EQ(catalogue1.GetItemTypes(), expected_models);
  EXPECT_EQ(catalogue1.GetLabels(), expected_labels);

  auto item = catalogue1.Create(AnotherTestItem::Type);
  EXPECT_TRUE(dynamic_cast<AnotherTestItem*>(item.get()) != nullptr);

  // duplications is not allowed
  EXPECT_THROW(catalogue1.merge(catalogue2), std::runtime_error);
}
