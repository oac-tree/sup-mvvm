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

#include "mvvm/model/compound_item.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace mvvm;

namespace
{
const std::string property_name("name");
}

//! Test of CompountItem machinery (property children etc).

class CompoundItemTests : public ::testing::Test
{
};

TEST_F(CompoundItemTests, InitialState)
{
  CompoundItem item;
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

TEST_F(CompoundItemTests, AddIntProperty)
{
  CompoundItem item;

  const int expected = 42;
  auto propertyItem = item.AddProperty(property_name, expected);
  EXPECT_TRUE(utils::HasTag(item, "name"));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->GetDisplayName(), property_name);
  EXPECT_EQ(propertyItem->Data<int>(), expected);
}

TEST_F(CompoundItemTests, SetIntProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, 41);

  const int expected = 42;
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<int>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<int>(), expected);
}

TEST_F(CompoundItemTests, AddDoubleProperty)
{
  CompoundItem item;

  const double expected = 42.1;
  auto propertyItem = item.AddProperty(property_name, expected);
  EXPECT_TRUE(utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->GetDisplayName(), property_name);
  EXPECT_EQ(propertyItem->Data<double>(), expected);
}

TEST_F(CompoundItemTests, SetDoubleProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, 41.11);

  const double expected = 42.0;
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<double>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<double>(), expected);
}

TEST_F(CompoundItemTests, AddCharProperty)
{
  CompoundItem item;

  auto propertyItem = item.AddProperty(property_name, "abc");
  EXPECT_TRUE(utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->Data<std::string>(), std::string("abc"));
}

TEST_F(CompoundItemTests, SetCharProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, "aaa");

  const char* expected{"bbb"};
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<std::string>(property_name), std::string(expected));
  EXPECT_EQ(propertyItem->Data<std::string>(), std::string(expected));
}

TEST_F(CompoundItemTests, AddStringProperty)
{
  CompoundItem item;

  auto propertyItem = item.AddProperty(property_name, std::string("abc"));
  EXPECT_TRUE(utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->Data<std::string>(), std::string("abc"));
}

TEST_F(CompoundItemTests, SetStringProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, std::string("aaa"));

  const std::string expected{"bbb"};
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<std::string>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<std::string>(), expected);
}

TEST_F(CompoundItemTests, AddBoolProperty)
{
  CompoundItem item;

  const bool expected = true;
  auto propertyItem = item.AddProperty(property_name, expected);
  EXPECT_TRUE(utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->Data<bool>(), expected);
}

TEST_F(CompoundItemTests, SetBoolProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, false);

  const bool expected = true;
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<bool>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<bool>(), expected);
}

TEST_F(CompoundItemTests, ItemAccess)
{
  const std::string tag = "tag";

  // creating parent with one tag
  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag(tag));

  // inserting two children
  auto property = parent.InsertItem<PropertyItem>({tag, 0});
  EXPECT_TRUE(parent.GetItem<PropertyItem>(tag) == property);
  EXPECT_THROW(parent.GetItem<CompoundItem>(tag), std::runtime_error);
}

TEST_F(CompoundItemTests, ItemVectorAccess)
{
  const std::string tag = "tag";

  // creating parent with one tag
  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag(tag));

  // inserting two children
  auto property1 = parent.InsertItem<PropertyItem>(TagIndex::Append(tag));
  auto property2 = parent.InsertItem<PropertyItem>(TagIndex::Append(tag));

  auto items = parent.GetItems<PropertyItem>(tag);
  std::vector<PropertyItem*> expected = {property1, property2};
  EXPECT_EQ(items, expected);
  EXPECT_EQ(parent.GetItems<CompoundItem>(tag).size(), 0);
}

//! Tests automatic index addition to default display name.

TEST_F(CompoundItemTests, DisplayNameIndexAddition)
{
  const std::string tag = "tag";

  // creating parent with one tag
  SessionItem parent;
  parent.RegisterTag(TagInfo::CreateUniversalTag(tag));

  // inserting two children
  auto child0 = parent.InsertItem<CompoundItem>(TagIndex::Append(tag));
  auto child1 = parent.InsertItem<CompoundItem>(TagIndex::Append(tag));

  // Default display names of items of the same type should have indices
  EXPECT_EQ(child0->GetDisplayName(), CompoundItem::Type + "0");
  EXPECT_EQ(child1->GetDisplayName(), CompoundItem::Type + "1");

  // however, if children have custom display name, they should remain intact
  child0->SetDisplayName("Jekyll");
  child1->SetDisplayName("Hyde");
  EXPECT_EQ(child0->GetDisplayName(), "Jekyll");
  EXPECT_EQ(child1->GetDisplayName(), "Hyde");
}

//! Test all children method.
//! Property items are also children.

TEST_F(CompoundItemTests, Children)
{
  CompoundItem item;
  EXPECT_TRUE(item.GetAllItems().empty());
  auto propertyItem = item.AddProperty(property_name, false);
  EXPECT_EQ(item.GetAllItems(), std::vector<SessionItem*>({propertyItem}));
}

//! Test AddBranch method.

TEST_F(CompoundItemTests, AddBranch)
{
  CompoundItem item;

  auto branch0 = item.AddBranch<CompoundItem>("branch0");
  auto property0 = item.AddProperty("thickness", 0.0);
  auto property1 = item.AddProperty<VectorItem>("position");
  auto branch1 = item.AddBranch<CompoundItem>("branch1");

  // it is not possible to add another branch with same name
  EXPECT_THROW(item.AddBranch<CompoundItem>("branch0"), std::runtime_error);

  EXPECT_EQ(utils::TopLevelItems(item), std::vector<SessionItem*>({branch0, branch1}));
  EXPECT_EQ(utils::SinglePropertyItems(item), std::vector<SessionItem*>({property0, property1}));
}
