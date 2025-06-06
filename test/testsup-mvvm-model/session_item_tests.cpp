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

#include "mvvm/model/session_item.h"

#include <mvvm/model/item_pool.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>

#include <gtest/gtest.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

/**
 * @brief Tests for SessionItem class.
 */
class SessionItemTests : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    explicit TestItem(const std::string& model_type) : SessionItem(model_type) {}
  };

  class ItemA : public SessionItem
  {
  public:
    ItemA() : SessionItem("ItemA") {}
  };

  class ItemB : public SessionItem
  {
  public:
    ItemB() : SessionItem("ItemB") {}
  };
};

TEST_F(SessionItemTests, InitialState)
{
  SessionItem item;
  const int role = DataRole::kData;

  EXPECT_EQ(item.GetModel(), nullptr);
  EXPECT_EQ(item.GetParent(), nullptr);
  EXPECT_EQ(item.GetTotalItemCount(), 0);
  EXPECT_FALSE(utils::IsValid(item.Data(role)));
  EXPECT_TRUE(item.GetAllItems().empty());
  EXPECT_EQ(item.GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(item.GetDisplayName(), SessionItem::GetStaticType());

  // Initially item has already an identifier defined.
  const std::vector<int> expected_roles = {DataRole::kIdentifier};
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);

  // Identifier is not zero
  EXPECT_FALSE(item.GetIdentifier().empty());
}

TEST_F(SessionItemTests, GetType)
{
  const SessionItem item;
  EXPECT_EQ(item.GetType(), SessionItem::GetStaticType());
}

TEST_F(SessionItemTests, SetData)
{
  SessionItem item;
  const int role = DataRole::kData;

  EXPECT_FALSE(utils::IsValid(item.Data(role)));

  const variant_t expected(42.0);
  EXPECT_TRUE(item.SetData(expected, role));

  std::vector<int> expected_roles = {DataRole::kIdentifier, DataRole::kData};
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item.Data(role), expected);

  // setting another value
  EXPECT_TRUE(item.SetData(43.0, role));
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item.Data(role), variant_t(43.0));

  // setting same value
  EXPECT_FALSE(item.SetData(43.0, role));
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item.Data(role), variant_t(43.0));
}

TEST_F(SessionItemTests, SetDataAndImplicitConversion)
{
  {
    SessionItem item;
    const int role = DataRole::kData;
    EXPECT_TRUE(item.SetData(43.0, role));
    EXPECT_EQ(utils::TypeName(item.Data(role)), constants::kFloat64TypeName);
  }

  {
    SessionItem item;
    const int role = DataRole::kData;
    EXPECT_TRUE(item.SetData(43, role));
    EXPECT_EQ(utils::TypeName(item.Data(role)), constants::kInt32TypeName);
  }

  {
    SessionItem item;
    const int role = DataRole::kData;
    EXPECT_TRUE(item.SetData(43L, role));
    EXPECT_EQ(utils::TypeName(item.Data(role)), constants::kInt64TypeName);
  }

  {
    SessionItem item;
    const int role = DataRole::kData;
    const int64 num(43);
    EXPECT_TRUE(item.SetData(num, role));
    EXPECT_EQ(utils::TypeName(item.Data(role)), constants::kInt64TypeName);
  }
}

TEST_F(SessionItemTests, HasData)
{
  SessionItem item;

  EXPECT_FALSE(item.HasData());
  EXPECT_TRUE(item.HasData(DataRole::kIdentifier));
  EXPECT_FALSE(item.HasData(DataRole::kData));
  EXPECT_FALSE(item.HasData(DataRole::kDisplay));
  EXPECT_FALSE(item.HasData(DataRole::kAppearance));
  EXPECT_FALSE(item.HasData(DataRole::kTooltip));
  EXPECT_FALSE(item.HasData(DataRole::kLowerLimit));
  EXPECT_FALSE(item.HasData(DataRole::kUpperLimit));

  item.SetData(42.0);
  EXPECT_TRUE(item.HasData());
}

TEST_F(SessionItemTests, SetDoubleData)
{
  SessionItem item;
  const double expected = 42.0;
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<double>(), expected);
}

TEST_F(SessionItemTests, SetIntData)
{
  SessionItem item;
  const int expected = 42;
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<int>(), expected);
}

TEST_F(SessionItemTests, SetBoolData)
{
  SessionItem item;
  const bool expected_true = true;
  EXPECT_TRUE(item.SetData(expected_true));
  EXPECT_EQ(item.Data<bool>(), expected_true);
  const bool expected_false = false;
  EXPECT_TRUE(item.SetData(expected_false));
  EXPECT_EQ(item.Data<bool>(), expected_false);
}

TEST_F(SessionItemTests, SetStringData)
{
  SessionItem item;
  const std::string expected{"abc"};
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<std::string>(), expected);
}

TEST_F(SessionItemTests, SetConstCharData)
{
  // bug in gcc8 when const char* is converted to bool
  SessionItem item;
  const char* expected = "abc";
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<std::string>(), std::string(expected));
}

TEST_F(SessionItemTests, GetDisplayName)
{
  TestItem item("Property");
  const variant_t data(42.0);
  EXPECT_TRUE(item.SetData(data));

  // default display name coincide with model type
  EXPECT_EQ(item.GetDisplayName(), "Property");
  EXPECT_FALSE(item.HasData(DataRole::kDisplay));

  // checking setter
  item.SetDisplayName("width");
  EXPECT_EQ(item.GetDisplayName(), "width");
  EXPECT_TRUE(item.HasData(DataRole::kDisplay));

  EXPECT_EQ(item.Data<double>(), 42.0);
}

TEST_F(SessionItemTests, AttemptToSetDifferentVariant)
{
  SessionItem item;
  const int role = DataRole::kData;
  const variant_t expected(42.0);

  // setting data for the first time
  EXPECT_TRUE(item.SetData(expected, role));

  const std::vector<int> expected_roles = {DataRole::kIdentifier, DataRole::kData};
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item.Data(role), expected);

  // attempt to rewrite variant with another type
  EXPECT_THROW(item.SetData(std::string("abc"), role), RuntimeException);

  // removing value by passing invalid variant
  EXPECT_NO_THROW(item.SetData(variant_t(), role));
  EXPECT_EQ(item.GetItemData()->GetRoles().size(), 1);
}

TEST_F(SessionItemTests, RegisterItemInPool)
{
  auto item = std::make_unique<SessionItem>();
  auto item_id = item->GetIdentifier();
  EXPECT_EQ(item->GetItemData()->GetRoles().size(), 1);

  std::shared_ptr<ItemPool> pool;

  // creating pool
  pool = std::make_shared<ItemPool>();
  pool->RegisterItem(item.get(), item_id);
  // registration shouldn't change item identifier
  EXPECT_EQ(item->GetIdentifier(), item_id);

  // registration key should coincide with item identifier
  auto key = pool->KeyForItem(item.get());
  const std::vector<int> expected_roles = {DataRole::kIdentifier};
  EXPECT_EQ(item->GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item_id, key);
}

TEST_F(SessionItemTests, DefaultTag)
{
  SessionItem item;
  EXPECT_EQ(item.GetTaggedItems()->GetDefaultTag(), std::string());
  EXPECT_FALSE(utils::HasTag(item, "defaultTag"));
}

TEST_F(SessionItemTests, RegisterTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("tagname"));
  EXPECT_TRUE(utils::HasTag(item, "tagname"));

  // registering of tag with same name forbidden
  EXPECT_THROW(item.RegisterTag(TagInfo::CreateUniversalTag("tagname")), RuntimeException);

  // registering empty tag is forbidden
  EXPECT_THROW(item.RegisterTag(TagInfo::CreateUniversalTag("")), RuntimeException);
}

//! Registering tag and setting it as default

TEST_F(SessionItemTests, RegisterDefaultTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("tagname"), /*set_as_default*/ true);
  EXPECT_EQ(item.GetTaggedItems()->GetDefaultTag(), "tagname");
}

//! Simple child insert.

TEST_F(SessionItemTests, InsertItem)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = std::make_unique<SessionItem>();
  auto p_child = child.get();

  // empty parent
  EXPECT_EQ(parent->GetTotalItemCount(), 0);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), nullptr), -1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child.get()), -1);
  EXPECT_EQ(parent->GetItem({"", 0}), nullptr);
  EXPECT_EQ(parent->GetItem(TagIndex::Append()), nullptr);
  EXPECT_EQ(parent->GetItem({"", 10}), nullptr);

  // inserting child
  auto inserted = parent->InsertItem(std::move(child), {"", 0});
  EXPECT_EQ(inserted, p_child);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem({"", 0}), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());
}

TEST_F(SessionItemTests, InsertItemTemplated)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // inserting child
  auto inserted = parent->InsertItem({"", 0});
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem({"", 0}), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());

  // inserting property item
  auto property = parent->InsertItem<PropertyItem>({"", 1});
  EXPECT_EQ(parent->GetTotalItemCount(), 2);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), property), 1);
  EXPECT_EQ(parent->GetAllItems()[1], property);
  EXPECT_EQ(parent->GetItem({"", 1}), property);
  EXPECT_EQ(property->GetParent(), parent.get());
}

TEST_F(SessionItemTests, InsertChildren)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // inserting two items
  auto child1 = parent->InsertItem(TagIndex::Append());
  auto child2 = parent->InsertItem(TagIndex::Append());
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child1), 0);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child2), 1);
  EXPECT_EQ(parent->GetItem({"", 0}), child1);
  EXPECT_EQ(parent->GetItem({"", 1}), child2);
  std::vector<SessionItem*> expected = {child1, child2};
  EXPECT_EQ(parent->GetAllItems(), expected);

  // inserting third item between two others
  auto child3 = parent->InsertItem({"", 1});
  expected = {child1, child3, child2};
  EXPECT_EQ(parent->GetAllItems(), expected);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child1), 0);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child2), 2);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child3), 1);
  EXPECT_EQ(parent->GetItem({"", 0}), child1);
  EXPECT_EQ(parent->GetItem({"", 1}), child3);
  EXPECT_EQ(parent->GetItem({"", 2}), child2);
  EXPECT_EQ(parent->GetItem({"", 3}), nullptr);

  // inserting forth item using index equal to number of items
  auto child4 = parent->InsertItem({"", parent->GetTotalItemCount()});

  // checking parents
  EXPECT_EQ(child1->GetParent(), parent.get());
  EXPECT_EQ(child2->GetParent(), parent.get());
  EXPECT_EQ(child3->GetParent(), parent.get());
  EXPECT_EQ(child4->GetParent(), parent.get());

  // attempt to insert item using out of scope index
  auto child5 = std::make_unique<SessionItem>();
  EXPECT_THROW(parent->InsertItem(std::move(child5), {"", parent->GetTotalItemCount() + 1}),
               InvalidOperationException);
}

TEST_F(SessionItemTests, AttemptToInsertParentIntoItself)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // trying to insert parent into a child
  EXPECT_THROW(parent->InsertItem(std::move(parent), {"", 0}), InvalidOperationException);
}

TEST_F(SessionItemTests, AttemptToInsertParentIntoChild)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = std::make_unique<SessionItem>();
  child->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child_ptr = child.get();

  // inserting child
  auto inserted = parent->InsertItem(std::move(child), {"", 0});
  EXPECT_EQ(inserted, child_ptr);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem({"", 0}), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());
  EXPECT_EQ(parent->GetParent(), nullptr);

  // trying to insert parent into a child
  EXPECT_THROW(inserted->InsertItem(std::move(parent), {"", 0}), InvalidOperationException);

  // nothing to check here. Unsuccessfull insert anyway ended with parent move and then destruction.
}

TEST_F(SessionItemTests, TakeItem)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // inserting items
  parent->InsertItem(TagIndex::Append());
  auto child2 = parent->InsertItem(TagIndex::Append());
  auto child3 = parent->InsertItem(TagIndex::Append());

  EXPECT_EQ(parent->GetTotalItemCount(), 3);

  // taking non-existing rows
  EXPECT_THROW(parent->TakeItem(TagIndex::Append()), InvalidOperationException);
  EXPECT_THROW(parent->TakeItem({"", parent->GetTotalItemCount()}), InvalidOperationException);

  // taking first row
  EXPECT_NO_THROW(parent->TakeItem({"", 0}));
  const std::vector<SessionItem*> expected = {child2, child3};
  EXPECT_EQ(parent->GetAllItems(), expected);
}

TEST_F(SessionItemTests, InsertAndTakeItemsFromSingleTag)
{
  const std::string tag1 = "tag1";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  EXPECT_TRUE(utils::HasTag(*parent, tag1));

  // inserting two children
  auto child1 = parent->InsertItem(TagIndex::Append(tag1));
  auto child2 = parent->InsertItem(TagIndex::Append(tag1));

  // testing result of insertion via non-tag interface
  const std::vector<SessionItem*> expected = {child1, child2};
  EXPECT_EQ(parent->GetAllItems(), expected);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child1), 0);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child2), 1);

  // testing single item access via tag interface
  EXPECT_EQ(parent->GetItem(tag1), child1);
  EXPECT_EQ(parent->GetItem({tag1, 0}), child1);
  EXPECT_EQ(parent->GetItem({tag1, 1}), child2);
  EXPECT_EQ(parent->GetItem({tag1, 2}), nullptr);  // wrong row

  // access to multiple items via tags interface
  EXPECT_EQ(parent->GetItems(tag1), expected);

  // removing first item
  parent->TakeItem({tag1, 0});
  EXPECT_EQ(parent->GetItems(tag1), std::vector<SessionItem*>() = {child2});
  // removing second item
  parent->TakeItem({tag1, 0});
  EXPECT_EQ(parent->GetItems(tag1), std::vector<SessionItem*>() = {});

  // removing from already empty container
  EXPECT_THROW(parent->TakeItem({tag1, 0}), InvalidOperationException);
}

TEST_F(SessionItemTests, InsertAndTakeItemsWhenTwoTags)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));
  EXPECT_TRUE(utils::HasTag(*parent, tag1));
  EXPECT_TRUE(utils::HasTag(*parent, tag2));

  // inserting two children
  auto child_t2_a = parent->InsertItem(TagIndex::Append(tag2));
  auto child_t2_c = parent->InsertItem(TagIndex::Append(tag2));
  auto child_t1_a = parent->InsertItem(TagIndex::Append(tag1));
  auto child_t1_b = parent->InsertItem(TagIndex::Append(tag1));
  auto child_t2_b = parent->InsertItem({tag2, 1});  // between child_t2_a and child_t2_c

  // testing item access via non-tag interface
  std::vector<SessionItem*> expected = {child_t1_a, child_t1_b, child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(parent->GetAllItems(), expected);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child_t1_a), 0);
  EXPECT_EQ(utils::IndexOfChild(parent.get(), child_t2_c), 4);

  // testing single item access via tag interface
  EXPECT_EQ(parent->GetItem(tag1), child_t1_a);
  EXPECT_EQ(parent->GetItem({tag1, 0}), child_t1_a);
  EXPECT_EQ(parent->GetItem({tag1, 1}), child_t1_b);
  EXPECT_EQ(parent->GetItem({tag2, 0}), child_t2_a);
  EXPECT_EQ(parent->GetItem({tag2, 1}), child_t2_b);
  EXPECT_EQ(parent->GetItem({tag2, 2}), child_t2_c);
  EXPECT_EQ(parent->GetItem({tag2, 3}), nullptr);  // no items with such row

  // access to multiple items via tags interface
  expected = {child_t1_a, child_t1_b};
  EXPECT_EQ(parent->GetItems(tag1), expected);
  expected = {child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(parent->GetItems(tag2), expected);

  // removing item from the middle of tag2
  parent->TakeItem({tag2, 1});
  expected = {child_t1_a, child_t1_b};
  EXPECT_EQ(parent->GetItems(tag1), expected);
  expected = {child_t2_a, child_t2_c};
  EXPECT_EQ(parent->GetItems(tag2), expected);
}

TEST_F(SessionItemTests, InsertAndTakeWhenTagHasLimits)
{
  const std::string tag1 = "tag1";
  const int maxItems = 3;
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo(tag1, 0, maxItems, std::vector<std::string>() = {}));

  // placing maximu allowed number of items
  std::vector<SessionItem*> expected;
  for (int i = 0; i < maxItems; ++i)
  {
    auto [uptr, raw] = mvvm::test::CreateTestData<SessionItem>();
    expected.push_back(raw);
    EXPECT_TRUE(parent->InsertItem(std::move(uptr), TagIndex::Append(tag1)));
  }
  EXPECT_EQ(parent->GetItems(tag1), expected);

  // no room for extra item
  EXPECT_THROW(parent->InsertItem(std::make_unique<SessionItem>(), TagIndex::Append(tag1)),
               InvalidOperationException);

  // removing first element
  parent->TakeItem({tag1, 0});
  expected.erase(expected.begin());
  EXPECT_EQ(parent->GetItems(tag1), expected);

  // adding extra item
  auto [uptr, raw] = mvvm::test::CreateTestData<SessionItem>();
  parent->InsertItem(std::move(uptr), TagIndex::Append(tag1));
  expected.push_back(raw);
  EXPECT_EQ(parent->GetItems(tag1), expected);
}

TEST_F(SessionItemTests, GetTagIndex)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting two children
  auto child_t2_a = parent->InsertItem(TagIndex::Append(tag2));  // 0
  auto child_t2_c = parent->InsertItem(TagIndex::Append(tag2));  // 2
  auto child_t1_a = parent->InsertItem(TagIndex::Append(tag1));  // 0
  auto child_t1_b = parent->InsertItem(TagIndex::Append(tag1));  // 1
  auto child_t2_b = parent->InsertItem({tag2, 1});  // 1 between child_t2_a and child_t2_c

  EXPECT_EQ(child_t1_a->GetTagIndex().GetIndex(), 0);
  EXPECT_EQ(child_t1_b->GetTagIndex().GetIndex(), 1);
  EXPECT_EQ(child_t2_a->GetTagIndex().GetIndex(), 0);
  EXPECT_EQ(child_t2_b->GetTagIndex().GetIndex(), 1);
  EXPECT_EQ(child_t2_c->GetTagIndex().GetIndex(), 2);

  EXPECT_EQ(child_t1_a->GetTagIndex().GetTag(), "tag1");
  EXPECT_EQ(child_t1_b->GetTagIndex().GetTag(), "tag1");
  EXPECT_EQ(child_t2_a->GetTagIndex().GetTag(), "tag2");
  EXPECT_EQ(child_t2_b->GetTagIndex().GetTag(), "tag2");
  EXPECT_EQ(child_t2_c->GetTagIndex().GetTag(), "tag2");
}

TEST_F(SessionItemTests, TagIndexOfItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting two children
  auto child_t2_a = parent->InsertItem(TagIndex::Append(tag2));  // 0
  auto child_t2_c = parent->InsertItem(TagIndex::Append(tag2));  // 2
  auto child_t1_a = parent->InsertItem(TagIndex::Append(tag1));  // 0
  auto child_t1_b = parent->InsertItem(TagIndex::Append(tag1));  // 1
  auto child_t2_b = parent->InsertItem({tag2, 1});  // 1 between child_t2_a and child_t2_c

  EXPECT_EQ(parent->TagIndexOfItem(child_t1_a).GetIndex(), 0);
  EXPECT_EQ(parent->TagIndexOfItem(child_t1_b).GetIndex(), 1);
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_a).GetIndex(), 0);
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_b).GetIndex(), 1);
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_c).GetIndex(), 2);

  EXPECT_EQ(parent->TagIndexOfItem(child_t1_a).GetTag(), "tag1");
  EXPECT_EQ(parent->TagIndexOfItem(child_t1_b).GetTag(), "tag1");
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_a).GetTag(), "tag2");
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_b).GetTag(), "tag2");
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_c).GetTag(), "tag2");
}

TEST_F(SessionItemTests, CheckAppearanceOnSetEnabled)
{
  SessionItem item;

  // there shouldn't be any data
  auto variant = item.Data(DataRole::kAppearance);
  EXPECT_FALSE(utils::IsValid(variant));

  // default status
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // disabling item
  item.SetEnabled(false);
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // data should be there now
  variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kDisabled));

  // enabling again
  item.SetEnabled(true);
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // data is there, but appearance is the default one
  variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kDefault));
}

TEST_F(SessionItemTests, CheckAppearanceOnSetEditable)
{
  SessionItem item;

  // there shouldn't be any data
  auto variant = item.Data(DataRole::kAppearance);
  EXPECT_FALSE(utils::IsValid(variant));

  // default status
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // setting readonly
  item.SetEditable(false);
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_FALSE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // data should be there now
  variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kReadOnly));

  // enabling again
  item.SetEditable(true);
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // data is there, but appearance is the default one
  variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kDefault));
}

TEST_F(SessionItemTests, CheckAppearanceOnSetVisible)
{
  SessionItem item;

  // there shouldn't be any data
  auto variant = item.Data(DataRole::kAppearance);
  EXPECT_FALSE(utils::IsValid(variant));

  // default status
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // setting readonly
  item.SetVisible(false);
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_FALSE(item.IsVisible());

  // data should be there now
  variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kHidden));

  // enabling again
  item.SetVisible(true);
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // data is there, but appearance is the default one
  variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kDefault));
}

TEST_F(SessionItemTests, CheckAppearanceForDisabledAndReaonly)
{
  SessionItem item;

  // setting readonly
  item.SetEnabled(false);
  item.SetEditable(false);

  EXPECT_FALSE(item.IsEnabled());
  EXPECT_FALSE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // data should be there now
  auto variant = item.Data(DataRole::kAppearance);
  EXPECT_TRUE(utils::IsValid(variant));
  EXPECT_EQ(variant, variant_t(Appearance::kDisabled | Appearance::kReadOnly));
}

TEST_F(SessionItemTests, SetAppearanceFlag)
{
  SessionItem item;

  // default status
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  EXPECT_FALSE(item.HasFlag(kDisabled));
  EXPECT_FALSE(item.HasFlag(kReadOnly));
  EXPECT_FALSE(item.HasFlag(kHidden));
  EXPECT_FALSE(item.HasFlag(kProperty));
  EXPECT_FALSE(item.HasFlag(kHighlighted));

  // there shouldn't be any data
  auto variant = item.Data(DataRole::kAppearance);
  EXPECT_FALSE(utils::IsValid(variant));

  // set kDisabled
  item.SetData(Appearance::kDefault, DataRole::kAppearance);
  EXPECT_EQ(&item.SetFlag(kDisabled, true), &item);
  EXPECT_TRUE(item.HasFlag(kDisabled));
  EXPECT_FALSE(item.HasFlag(kReadOnly));
  EXPECT_FALSE(item.HasFlag(kHidden));
  EXPECT_FALSE(item.HasFlag(kProperty));
  EXPECT_FALSE(item.HasFlag(kHighlighted));

  // set kReadOnly
  item.SetData(Appearance::kDefault, DataRole::kAppearance);
  EXPECT_EQ(&item.SetFlag(kReadOnly, true), &item);
  EXPECT_FALSE(item.HasFlag(kDisabled));
  EXPECT_TRUE(item.HasFlag(kReadOnly));
  EXPECT_FALSE(item.HasFlag(kHidden));
  EXPECT_FALSE(item.HasFlag(kProperty));
  EXPECT_FALSE(item.HasFlag(kHighlighted));

  // set kHidden
  item.SetData(Appearance::kDefault, DataRole::kAppearance);
  EXPECT_EQ(&item.SetFlag(kHidden, true), &item);
  EXPECT_FALSE(item.HasFlag(kDisabled));
  EXPECT_FALSE(item.HasFlag(kReadOnly));
  EXPECT_TRUE(item.HasFlag(kHidden));
  EXPECT_FALSE(item.HasFlag(kProperty));
  EXPECT_FALSE(item.HasFlag(kHighlighted));

  // set kProperty
  item.SetData(Appearance::kDefault, DataRole::kAppearance);
  EXPECT_EQ(&item.SetFlag(kProperty, true), &item);
  EXPECT_FALSE(item.HasFlag(kDisabled));
  EXPECT_FALSE(item.HasFlag(kReadOnly));
  EXPECT_FALSE(item.HasFlag(kHidden));
  EXPECT_TRUE(item.HasFlag(kProperty));
  EXPECT_FALSE(item.HasFlag(kHighlighted));

  // set kHighlighted
  item.SetData(Appearance::kDefault, DataRole::kAppearance);
  EXPECT_EQ(&item.SetFlag(kHighlighted, true), &item);
  EXPECT_FALSE(item.HasFlag(kDisabled));
  EXPECT_FALSE(item.HasFlag(kReadOnly));
  EXPECT_FALSE(item.HasFlag(kHidden));
  EXPECT_FALSE(item.HasFlag(kProperty));
  EXPECT_TRUE(item.HasFlag(kHighlighted));
}

TEST_F(SessionItemTests, Tooltip)
{
  SessionItem item;

  EXPECT_EQ(item.GetToolTip(), "");
  EXPECT_FALSE(item.HasData(DataRole::kTooltip));

  EXPECT_EQ(&item.SetToolTip("abc"), &item);
  EXPECT_TRUE(item.HasData(DataRole::kTooltip));
  EXPECT_EQ(item.GetToolTip(), "abc");
}

TEST_F(SessionItemTests, EditorType)
{
  SessionItem item;

  EXPECT_EQ(item.GetEditorType(), "");
  EXPECT_FALSE(item.HasData(DataRole::kEditor));

  EXPECT_EQ(&item.SetEditorType("abc"), &item);
  EXPECT_TRUE(item.HasData(DataRole::kEditor));
  EXPECT_EQ(item.GetEditorType(), "abc");
}

TEST_F(SessionItemTests, GetItemCount)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting two children
  parent->InsertItem(TagIndex::Append(tag1));
  parent->InsertItem(TagIndex::Append(tag2));
  parent->InsertItem(TagIndex::Append(tag2));

  EXPECT_EQ(parent->GetItemCount(tag1), 1);
  EXPECT_EQ(parent->GetItemCount(tag2), 2);
}

TEST_F(SessionItemTests, CastedItemAccess)
{
  const std::string tag = "tag";
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag), true);

  auto item0 = parent->InsertItem<ItemA>(TagIndex::Append());
  auto item1 = parent->InsertItem<ItemB>(TagIndex::Append());
  auto item2 = parent->InsertItem<ItemA>(TagIndex::Append());

  EXPECT_EQ(parent->GetItem<ItemA>(tag), item0);
  // current behavior of item<> method, consider to change
  EXPECT_THROW(parent->GetItem<ItemB>(tag), RuntimeException);

  EXPECT_EQ(parent->GetItems<ItemA>(tag), std::vector<ItemA*>({item0, item2}));
  EXPECT_EQ(parent->GetItems<ItemB>(tag), std::vector<ItemB*>({item1}));
}

TEST_F(SessionItemTests, Clone)
{
  SessionItem item;
  item.SetDisplayName("abc");

  {  // deep copy
    auto clone = utils::CopyItem(item);
    EXPECT_NE(item.GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(item.GetDisplayName(), clone->GetDisplayName());
    EXPECT_EQ(item.GetParent(), nullptr);
    EXPECT_EQ(item.GetModel(), nullptr);
  }

  {  // clone
    auto clone = item.Clone();
    EXPECT_EQ(item.GetIdentifier(), clone->GetIdentifier());
    EXPECT_EQ(item.GetDisplayName(), clone->GetDisplayName());
    EXPECT_EQ(item.GetParent(), nullptr);
    EXPECT_EQ(item.GetModel(), nullptr);
  }
}

TEST_F(SessionItemTests, CloneParentAndChild)
{
  const std::string tag = "tag";
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag), true);
  parent->SetDisplayName("abc");
  auto child = parent->InsertItem<ItemA>(TagIndex::Append());
  child->SetDisplayName("def");

  {  // deep copy
    auto parent_clone = utils::CopyItem(*parent);
    EXPECT_NE(parent->GetIdentifier(), parent_clone->GetIdentifier());
    ASSERT_EQ(parent_clone->GetTotalItemCount(), 1);
    EXPECT_EQ(parent_clone->GetDisplayName(), parent->GetDisplayName());
    EXPECT_EQ(parent_clone->GetModel(), nullptr);
    auto child_clone = parent_clone->GetItem({tag, 0});
    EXPECT_NE(child_clone->GetIdentifier(), child->GetIdentifier());
    EXPECT_EQ(child_clone->GetDisplayName(), child->GetDisplayName());
    EXPECT_EQ(child_clone->GetModel(), nullptr);

    // cloned child should have proper parent
    EXPECT_EQ(child_clone->GetParent(), parent_clone.get());
    EXPECT_EQ(parent_clone->TagIndexOfItem(child_clone), TagIndex("tag", 0));
  }

  {  // clone
    auto parent_clone = parent->Clone();
    EXPECT_EQ(parent->GetIdentifier(), parent_clone->GetIdentifier());
    ASSERT_EQ(parent_clone->GetTotalItemCount(), 1);
    EXPECT_EQ(parent_clone->GetDisplayName(), parent->GetDisplayName());
    EXPECT_EQ(parent_clone->GetModel(), nullptr);
    auto child_clone = parent_clone->GetItem({tag, 0});
    EXPECT_EQ(child_clone->GetIdentifier(), child->GetIdentifier());
    EXPECT_EQ(child_clone->GetDisplayName(), child->GetDisplayName());
    EXPECT_EQ(child_clone->GetModel(), nullptr);

    // cloned child should have proper parent
    EXPECT_EQ(child_clone->GetParent(), parent_clone.get());
    EXPECT_EQ(parent_clone->TagIndexOfItem(child_clone), TagIndex("tag", 0));
  }
}
