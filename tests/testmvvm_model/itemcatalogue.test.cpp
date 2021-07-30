// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/itemcatalogue.h"

#include "mvvm/model/propertyitem.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace ModelView;

//! Testing ItemCatalogue construction

class ItemCatalogueTest : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    static inline const std::string Type = "Test";
    TestItem() : SessionItem(Type) {}
  };

  class AnotherTestItem : public SessionItem
  {
  public:
    static inline const std::string Type = "AnotherTest";
    AnotherTestItem() : SessionItem(Type) {}
  };
};

TEST_F(ItemCatalogueTest, initialState)
{
  ItemCatalogue catalogue;
  EXPECT_EQ(catalogue.itemCount(), 0);
  EXPECT_EQ(catalogue.modelTypes(), std::vector<std::string>({}));
  EXPECT_EQ(catalogue.labels(), std::vector<std::string>({}));
}

TEST_F(ItemCatalogueTest, addItem)
{
  ItemCatalogue catalogue;

  catalogue.registerItem<PropertyItem>();

  EXPECT_EQ(catalogue.itemCount(), 1);

  auto item = catalogue.create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // registration of second item is not allowed
  EXPECT_THROW(catalogue.registerItem<PropertyItem>(), std::runtime_error);

  // item was not registered, creation not allowed
  EXPECT_THROW(catalogue.create("non-registered"), std::runtime_error);

  // checking model types and labels
  EXPECT_EQ(catalogue.modelTypes(), std::vector<std::string>({"Property"}));
  EXPECT_EQ(catalogue.labels(), std::vector<std::string>({""}));
}

TEST_F(ItemCatalogueTest, copyConstructor)
{
  ItemCatalogue catalogue;
  catalogue.registerItem<PropertyItem>();

  ItemCatalogue copy(catalogue);

  // creation of item using first catalogue
  auto item = catalogue.create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // creation of item using catalogue copy
  item = copy.create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // checking model types and labels in new catalogue
  EXPECT_EQ(copy.modelTypes(), std::vector<std::string>({"Property"}));
  EXPECT_EQ(copy.labels(), std::vector<std::string>({""}));

  // adding item to first catalogue but not the second
  catalogue.registerItem<TestItem>();
  item = catalogue.create(TestItem::Type);
  EXPECT_TRUE(dynamic_cast<TestItem*>(item.get()) != nullptr);

  // copy of catalogue knows nothing about new VectorType
  EXPECT_THROW(copy.create(TestItem::Type), std::runtime_error);
}

TEST_F(ItemCatalogueTest, assignmentOperator)
{
  ItemCatalogue catalogue;
  catalogue.registerItem<PropertyItem>();

  ItemCatalogue copy;
  copy = catalogue;

  // creation of item using first catalogue
  auto item = catalogue.create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);

  // creation of item using catalogue copy
  item = copy.create(PropertyItem::Type);
  EXPECT_TRUE(dynamic_cast<PropertyItem*>(item.get()) != nullptr);
}

TEST_F(ItemCatalogueTest, contains)
{
  ItemCatalogue catalogue;
  catalogue.registerItem<PropertyItem>();

  EXPECT_TRUE(catalogue.contains(PropertyItem::Type));
  EXPECT_FALSE(catalogue.contains(TestItem::Type));
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

TEST_F(ItemCatalogueTest, addLabeledItem)
{
  ItemCatalogue catalogue;
  catalogue.registerItem<PropertyItem>("property");
  catalogue.registerItem<TestItem>("test item");

  // checking model types and labels
  EXPECT_EQ(catalogue.modelTypes(), std::vector<std::string>({"Property", "Test"}));
  EXPECT_EQ(catalogue.labels(), std::vector<std::string>({"property", "test item"}));
}

TEST_F(ItemCatalogueTest, merge)
{
  ItemCatalogue catalogue1;
  catalogue1.registerItem<PropertyItem>("property");
  catalogue1.registerItem<TestItem>("test");

  ItemCatalogue catalogue2;
  catalogue2.registerItem<AnotherTestItem>("another");

  // adding two catalogue together
  catalogue1.merge(catalogue2);

  std::vector<std::string> expected_models = {PropertyItem::Type, TestItem::Type,
                                              AnotherTestItem::Type};
  std::vector<std::string> expected_labels = {"property", "test", "another"};

  EXPECT_EQ(catalogue1.modelTypes(), expected_models);
  EXPECT_EQ(catalogue1.labels(), expected_labels);

  auto item = catalogue1.create(AnotherTestItem::Type);
  EXPECT_TRUE(dynamic_cast<AnotherTestItem*>(item.get()) != nullptr);

  // duplications is not allowed
  EXPECT_THROW(catalogue1.merge(catalogue2), std::runtime_error);
}
