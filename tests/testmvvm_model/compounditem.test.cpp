/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/compounditem.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/model/propertyitem.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace mvvm;

namespace
{
const std::string property_name("name");
}

//! Test of CompountItem machinery (property children etc).

class CompoundItemTest : public ::testing::Test
{
};

TEST_F(CompoundItemTest, InitialState)
{
  CompoundItem item;
  EXPECT_EQ(item.GetTotalItemCount(), 0);
}

TEST_F(CompoundItemTest, AddIntProperty)
{
  CompoundItem item;

  const int expected = 42;
  auto propertyItem = item.AddProperty(property_name, expected);
  EXPECT_TRUE(Utils::HasTag(item, "name"));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->GetDisplayName(), property_name);
  EXPECT_EQ(propertyItem->Data<int>(), expected);

  // FIXME uncomment
  //  EXPECT_FALSE(propertyItem->data<QVariant>(ItemDataRole::LIMITS).isValid());
}

TEST_F(CompoundItemTest, SetIntProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, 41);

  const int expected = 42;
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<int>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<int>(), expected);
}

TEST_F(CompoundItemTest, AddDoubleProperty)
{
  CompoundItem item;

  const double expected = 42.1;
  auto propertyItem = item.AddProperty(property_name, expected);
  EXPECT_TRUE(Utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->GetDisplayName(), property_name);
  EXPECT_EQ(propertyItem->Data<double>(), expected);

  // FIXME uncomment
  //  EXPECT_TRUE(propertyItem->data<QVariant>(ItemDataRole::LIMITS).isValid());

  //  // limits should be "negative 'unlimited' by default
  //  auto limits = propertyItem->data<RealLimits>(ItemDataRole::LIMITS);
  //  EXPECT_FALSE(limits.hasLowerLimit());
  //  EXPECT_FALSE(limits.hasUpperLimit());
}

TEST_F(CompoundItemTest, SetDoubleProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, 41.11);

  const double expected = 42.0;
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<double>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<double>(), expected);
}

TEST_F(CompoundItemTest, AddCharProperty)
{
  CompoundItem item;

  auto propertyItem = item.AddProperty(property_name, "abc");
  EXPECT_TRUE(Utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->Data<std::string>(), std::string("abc"));

  // FIXME uncomment
  //  EXPECT_FALSE(propertyItem->data<QVariant>(ItemDataRole::LIMITS).isValid());
}

TEST_F(CompoundItemTest, SetCharProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, "aaa");

  const char* expected{"bbb"};
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<std::string>(property_name), std::string(expected));
  EXPECT_EQ(propertyItem->Data<std::string>(), std::string(expected));
}

TEST_F(CompoundItemTest, AddStringProperty)
{
  CompoundItem item;

  auto propertyItem = item.AddProperty(property_name, std::string("abc"));
  EXPECT_TRUE(Utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->Data<std::string>(), std::string("abc"));

  // FIXME uncomment
  //  EXPECT_FALSE(propertyItem->data<QVariant>(ItemDataRole::LIMITS).isValid());
}

TEST_F(CompoundItemTest, SetStringProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, std::string("aaa"));

  const std::string expected{"bbb"};
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<std::string>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<std::string>(), expected);
}

TEST_F(CompoundItemTest, AddBoolProperty)
{
  CompoundItem item;

  const bool expected = true;
  auto propertyItem = item.AddProperty(property_name, expected);
  EXPECT_TRUE(Utils::HasTag(item, property_name));

  EXPECT_EQ(propertyItem->GetType(), PropertyItem::Type);
  EXPECT_EQ(propertyItem->Data<bool>(), expected);

  // FIXME uncomment
  //  EXPECT_FALSE(propertyItem->data<QVariant>(ItemDataRole::LIMITS).isValid());
}

TEST_F(CompoundItemTest, SetBoolProperty)
{
  CompoundItem item;
  auto propertyItem = item.AddProperty(property_name, false);

  const bool expected = true;
  item.SetProperty(property_name, expected);

  EXPECT_EQ(item.Property<bool>(property_name), expected);
  EXPECT_EQ(propertyItem->Data<bool>(), expected);
}

TEST_F(CompoundItemTest, ItemAccess)
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

TEST_F(CompoundItemTest, ItemVectorAccess)
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

TEST_F(CompoundItemTest, DisplayNameIndexAddition)
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

TEST_F(CompoundItemTest, Children)
{
  CompoundItem item;
  EXPECT_TRUE(item.GetAllItems().empty());
  auto propertyItem = item.AddProperty(property_name, false);
  EXPECT_EQ(item.GetAllItems(), std::vector<SessionItem*>({propertyItem}));
}
