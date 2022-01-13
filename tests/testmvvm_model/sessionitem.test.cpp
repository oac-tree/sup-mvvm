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

#include "mvvm/model/sessionitem.h"

#include "test_utils.h"

#include "mvvm/model/itempool.h"
#include "mvvm/model/itemutils.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitemdata.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

using namespace mvvm;

class SessionItemTest : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    TestItem(const std::string& model_type) : SessionItem(model_type) {}
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

TEST_F(SessionItemTest, InitialState)
{
  SessionItem item;
  const int role = DataRole::kData;

  EXPECT_EQ(item.GetModel(), nullptr);
  EXPECT_EQ(item.GetParent(), nullptr);
  EXPECT_EQ(item.GetTotalItemCount(), 0);
  EXPECT_FALSE(Utils::IsValid(item.Data(role)));
  EXPECT_TRUE(item.GetAllItems().empty());
  EXPECT_EQ(item.GetType(), SessionItem::Type);
  EXPECT_EQ(item.GetDisplayName(), SessionItem::Type);

  // Initially item has already an identifier defined.
  std::vector<int> expected_roles = {DataRole::kIdentifier, DataRole::kDisplay};
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);

  // Identifier is not zero
  EXPECT_FALSE(item.GetIdentifier().empty());
}

TEST_F(SessionItemTest, GetType)
{
  SessionItem item;
  EXPECT_EQ(item.GetType(), SessionItem::Type);
}

//! Validating ::setData and appearance of roles.

TEST_F(SessionItemTest, SetData)
{
  SessionItem item;
  const int role = DataRole::kData;

  EXPECT_FALSE(Utils::IsValid(item.Data(role)));

  variant_t expected(42.0);
  EXPECT_TRUE(item.SetData(expected, role));

  std::vector<int> expected_roles = {DataRole::kIdentifier, DataRole::kDisplay, DataRole::kData};
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

//! Validating ::setData in the context of implicit conversion.

TEST_F(SessionItemTest, SetDataAndImplicitConversion)
{
  {
    SessionItem item;
    const int role = DataRole::kData;
    EXPECT_TRUE(item.SetData(43.0, DataRole::kData));
    EXPECT_EQ(Utils::TypeName(item.Data(role)), Constants::kDoubleTypeName);
  }

  {
    SessionItem item;
    const int role = DataRole::kData;
    EXPECT_TRUE(item.SetData(43, DataRole::kData));
    EXPECT_EQ(Utils::TypeName(item.Data(role)), Constants::kIntTypeName);
  }
}

TEST_F(SessionItemTest, HasData)
{
  SessionItem item;

  EXPECT_FALSE(item.HasData());
  EXPECT_TRUE(item.HasData(DataRole::kIdentifier));
  EXPECT_FALSE(item.HasData(DataRole::kData));
  EXPECT_TRUE(item.HasData(DataRole::kDisplay));
  EXPECT_FALSE(item.HasData(DataRole::kAppearance));
  EXPECT_FALSE(item.HasData(DataRole::kTooltip));

  item.SetData(42.0);
  EXPECT_TRUE(item.HasData());
}

TEST_F(SessionItemTest, SetDoubleData)
{
  SessionItem item;
  const double expected = 42.0;
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<double>(), expected);
}

TEST_F(SessionItemTest, SetIntData)
{
  SessionItem item;
  const int expected = 42;
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<int>(), expected);
}

TEST_F(SessionItemTest, SetBoolData)
{
  SessionItem item;
  const bool expected_true = true;
  EXPECT_TRUE(item.SetData(expected_true));
  EXPECT_EQ(item.Data<bool>(), expected_true);
  const bool expected_false = false;
  EXPECT_TRUE(item.SetData(expected_false));
  EXPECT_EQ(item.Data<bool>(), expected_false);
}

TEST_F(SessionItemTest, SetStringData)
{
  SessionItem item;
  const std::string expected{"abc"};
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<std::string>(), expected);
}

//! Validating that const char is correctly converted to std::string, and not to boolean,
//! on the way to variant_t.

TEST_F(SessionItemTest, SetConstCharData)
{
  SessionItem item;
  const char* expected = "abc";
  EXPECT_TRUE(item.SetData(expected));
  EXPECT_EQ(item.Data<std::string>(), std::string(expected));
}

//! Display role.

TEST_F(SessionItemTest, GetDisplayName)
{
  TestItem item("Property");
  variant_t data(42.0);
  EXPECT_TRUE(item.SetData(data));

  // default display name coincide with model type
  EXPECT_EQ(item.GetDisplayName(), "Property");

  // checking setter
  item.SetDisplayName("width");
  EXPECT_EQ(item.GetDisplayName(), "width");
  EXPECT_EQ(item.Data<double>(), 42.0);
}

//! Attempt to set the different Variant to already existing role.

TEST_F(SessionItemTest, VariantMismatch)
{
  SessionItem item;
  const int role = DataRole::kData;
  variant_t expected(42.0);

  // setting data for the first time
  EXPECT_TRUE(item.SetData(expected, role));

  std::vector<int> expected_roles = {DataRole::kIdentifier, DataRole::kDisplay, DataRole::kData};
  EXPECT_EQ(item.GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item.Data(role), expected);

  // attempt to rewrite variant with another type
  EXPECT_THROW(item.SetData(std::string("abc"), role), std::runtime_error);

  // removing value by passing invalid variant
  EXPECT_NO_THROW(item.SetData(variant_t(), role));
  EXPECT_EQ(item.GetItemData()->GetRoles().size(), 2);
}

//! Item registration in a pool.

TEST_F(SessionItemTest, RegisterItem)
{
  auto item = std::make_unique<SessionItem>();
  auto item_id = item->GetIdentifier();
  EXPECT_EQ(item->GetItemData()->GetRoles().size(), 2u);

  std::shared_ptr<ItemPool> pool;

  // creating pool
  pool.reset(new ItemPool);
  pool->RegisterItem(item.get(), item_id);
  // registration shouldn't change item identifier
  EXPECT_EQ(item->GetIdentifier(), item_id);

  // registration key should coincide with item identifier
  auto key = pool->KeyForItem(item.get());
  std::vector<int> expected_roles = {DataRole::kIdentifier, DataRole::kDisplay};
  EXPECT_EQ(item->GetItemData()->GetRoles(), expected_roles);
  EXPECT_EQ(item_id, key);
}

//! Item registration in a pool.

TEST_F(SessionItemTest, DefaultTag)
{
  SessionItem item;
  EXPECT_EQ(item.GetTaggedItems()->GetDefaultTag(), std::string());
  EXPECT_FALSE(Utils::HasTag(item, "defaultTag"));
}

//! Registering tags

TEST_F(SessionItemTest, RegisterTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("tagname"));
  EXPECT_TRUE(Utils::HasTag(item, "tagname"));

  // registering of tag with same name forbidden
  EXPECT_THROW(item.RegisterTag(TagInfo::CreateUniversalTag("tagname")), std::runtime_error);

  // registering empty tag is forbidden
  EXPECT_THROW(item.RegisterTag(TagInfo::CreateUniversalTag("")), std::runtime_error);
}

//! Registering tag and setting it as default

TEST_F(SessionItemTest, RegisterDefaultTag)
{
  SessionItem item;
  item.RegisterTag(TagInfo::CreateUniversalTag("tagname"), /*set_as_default*/ true);
  EXPECT_EQ(item.GetTaggedItems()->GetDefaultTag(), "tagname");
}

//! Simple child insert.

TEST_F(SessionItemTest, InsertItem)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = std::make_unique<SessionItem>();
  auto p_child = child.get();

  // empty parent
  EXPECT_EQ(parent->GetTotalItemCount(), 0);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), nullptr), -1);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child.get()), -1);
  EXPECT_EQ(parent->GetItem("", 0), nullptr);
  EXPECT_EQ(parent->GetItem("", -1), nullptr);
  EXPECT_EQ(parent->GetItem("", 10), nullptr);

  // inserting child
  auto inserted = parent->InsertItem(std::move(child), {"", 0});
  EXPECT_EQ(inserted, p_child);
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem("", 0), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());
}

//! Simple child insert.

TEST_F(SessionItemTest, InsertItemTemplated)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // inserting child
  auto inserted = parent->InsertItem({"", 0});
  EXPECT_EQ(parent->GetTotalItemCount(), 1);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), inserted), 0);
  EXPECT_EQ(parent->GetAllItems()[0], inserted);
  EXPECT_EQ(parent->GetItem("", 0), inserted);
  EXPECT_EQ(inserted->GetParent(), parent.get());

  // inserting property item
  auto property = parent->InsertItem<PropertyItem>({"", 1});
  EXPECT_EQ(parent->GetTotalItemCount(), 2);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), property), 1);
  EXPECT_EQ(parent->GetAllItems()[1], property);
  EXPECT_EQ(parent->GetItem("", 1), property);
  EXPECT_EQ(property->GetParent(), parent.get());
}

//! Simple children insert.

TEST_F(SessionItemTest, InsertChildren)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // inserting two items
  auto child1 = parent->InsertItem(TagIndex::Append());
  auto child2 = parent->InsertItem(TagIndex::Append());
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child1), 0);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child2), 1);
  EXPECT_EQ(parent->GetItem("", 0), child1);
  EXPECT_EQ(parent->GetItem("", 1), child2);
  std::vector<SessionItem*> expected = {child1, child2};
  EXPECT_EQ(parent->GetAllItems(), expected);

  // inserting third item between two others
  auto child3 = parent->InsertItem({"", 1});
  expected = {child1, child3, child2};
  EXPECT_EQ(parent->GetAllItems(), expected);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child1), 0);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child2), 2);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child3), 1);
  EXPECT_EQ(parent->GetItem("", 0), child1);
  EXPECT_EQ(parent->GetItem("", 1), child3);
  EXPECT_EQ(parent->GetItem("", 2), child2);
  EXPECT_EQ(parent->GetItem("", 3), nullptr);

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
               std::runtime_error);
}

//! Removing (taking) item from parent.

TEST_F(SessionItemTest, TakeItem)
{
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // inserting items
  parent->InsertItem(TagIndex::Append());
  auto child2 = parent->InsertItem(TagIndex::Append());
  auto child3 = parent->InsertItem(TagIndex::Append());

  EXPECT_EQ(parent->GetTotalItemCount(), 3);

  // taking non-existing rows
  EXPECT_EQ(parent->TakeItem({"", -1}), nullptr);
  EXPECT_EQ(parent->TakeItem({"", parent->GetTotalItemCount()}), nullptr);

  // taking first row
  auto taken = parent->TakeItem({"", 0});
  EXPECT_EQ(taken->GetParent(), nullptr);
  std::vector<SessionItem*> expected = {child2, child3};
  EXPECT_EQ(parent->GetAllItems(), expected);
}

//! Insert and take tagged items.

TEST_F(SessionItemTest, SingleTagAndItems)
{
  const std::string tag1 = "tag1";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  EXPECT_TRUE(Utils::HasTag(*parent, tag1));

  // inserting two children
  auto child1 = parent->InsertItem({tag1, -1});
  auto child2 = parent->InsertItem({tag1, -1});

  // testing result of insertion via non-tag interface
  std::vector<SessionItem*> expected = {child1, child2};
  EXPECT_EQ(parent->GetAllItems(), expected);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child1), 0);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child2), 1);

  // testing single item access via tag interface
  EXPECT_EQ(parent->GetItem(tag1), child1);
  EXPECT_EQ(parent->GetItem(tag1, 0), child1);
  EXPECT_EQ(parent->GetItem(tag1, 1), child2);
  EXPECT_EQ(parent->GetItem(tag1, 2), nullptr);  // wrong row

  // access to multiple items via tags interface
  EXPECT_EQ(parent->GetItems(tag1), expected);

  // removing first item
  parent->TakeItem({tag1, 0});
  EXPECT_EQ(parent->GetItems(tag1), std::vector<SessionItem*>() = {child2});
  // removing second item
  parent->TakeItem({tag1, 0});
  EXPECT_EQ(parent->GetItems(tag1), std::vector<SessionItem*>() = {});

  // removing from already empty container
  EXPECT_EQ(parent->TakeItem({tag1, 0}), nullptr);
}

//! Insert and take tagged items when two tags are present.

TEST_F(SessionItemTest, TwoTagsAndItems)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));
  EXPECT_TRUE(Utils::HasTag(*parent, tag1));
  EXPECT_TRUE(Utils::HasTag(*parent, tag2));

  // inserting two children
  auto child_t2_a = parent->InsertItem({tag2, -1});
  auto child_t2_c = parent->InsertItem({tag2, -1});
  auto child_t1_a = parent->InsertItem({tag1, -1});
  auto child_t1_b = parent->InsertItem({tag1, -1});
  auto child_t2_b = parent->InsertItem({tag2, 1});  // between child_t2_a and child_t2_c

  // testing item access via non-tag interface
  std::vector<SessionItem*> expected = {child_t1_a, child_t1_b, child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(parent->GetAllItems(), expected);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child_t1_a), 0);
  EXPECT_EQ(Utils::IndexOfChild(parent.get(), child_t2_c), 4);

  // testing single item access via tag interface
  EXPECT_EQ(parent->GetItem(tag1), child_t1_a);
  EXPECT_EQ(parent->GetItem(tag1, 0), child_t1_a);
  EXPECT_EQ(parent->GetItem(tag1, 1), child_t1_b);
  EXPECT_EQ(parent->GetItem(tag2, 0), child_t2_a);
  EXPECT_EQ(parent->GetItem(tag2, 1), child_t2_b);
  EXPECT_EQ(parent->GetItem(tag2, 2), child_t2_c);
  EXPECT_EQ(parent->GetItem(tag2, 3), nullptr);  // no items with such row

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

//! Inserting and removing items when tag has limits.

TEST_F(SessionItemTest, TagWithLimits)
{
  const std::string tag1 = "tag1";
  const int maxItems = 3;
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo(tag1, 0, maxItems, std::vector<std::string>() = {}));

  // placing maximu allowed number of items
  std::vector<SessionItem*> expected;
  for (int i = 0; i < maxItems; ++i)
  {
    auto [uptr, raw] = TestUtils::CreateTestData<SessionItem>();
    expected.push_back(raw);
    EXPECT_TRUE(parent->InsertItem(std::move(uptr), {tag1, -1}));
  }
  EXPECT_EQ(parent->GetItems(tag1), expected);

  // no room for extra item
  EXPECT_THROW(parent->InsertItem(std::make_unique<SessionItem>(), {tag1, -1}), std::runtime_error);

  // removing first element
  parent->TakeItem({tag1, 0});
  expected.erase(expected.begin());
  EXPECT_EQ(parent->GetItems(tag1), expected);

  // adding extra item
  auto [uptr, raw] = TestUtils::CreateTestData<SessionItem>();
  parent->InsertItem(std::move(uptr), {tag1, -1});
  expected.push_back(raw);
  EXPECT_EQ(parent->GetItems(tag1), expected);
}

////! Inserting and removing items when tag has limits.
// TEST_F(SessionItemTest, TagItemTypes)
//{
//  const std::string tag1 = "tag1";
//  const std::string tag2 = "tag2";
//  const std::string itemType1 = "ItemType1";
//  const std::string itemType2 = "ItemType2";
//  const std::string itemType3 = "ItemType3";
//  const std::string itemType4 = "ItemType4";

//  auto parent = std::make_unique<SessionItem>();
//  parent->RegisterTag(TagInfo(tag1, 0, -1, std::vector<std::string>() = {itemType1, itemType2}));
//  parent->RegisterTag(TagInfo(tag2, 0, -1, std::vector<std::string>() = {itemType3}));

//  auto item1 = new TestItem(itemType1);
//  auto item2 = new TestItem(itemType2);
//  auto item3 = new TestItem(itemType3);

//  // attempt to add item not intended for tag
//  EXPECT_FALSE(parent->insertItem(item1, {tag2, -1}));
//  EXPECT_FALSE(parent->insertItem(item3, {tag1, -1}));

//  // normal insert to appropriate tag
//  parent->insertItem(item3, {tag2, -1});
//  parent->insertItem(item1, {tag1, -1});
//  parent->insertItem(item2, {tag1, -1});

//  std::vector<SessionItem*> expected = {item1, item2};
//  EXPECT_EQ(parent->getItems(tag1), expected);
//  expected = {item3};
//  EXPECT_EQ(parent->getItems(tag2), expected);
//}

//! Checks row of item in its tag

TEST_F(SessionItemTest, GetTagIndex)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting two children
  auto child_t2_a = parent->InsertItem({tag2, -1});  // 0
  auto child_t2_c = parent->InsertItem({tag2, -1});  // 2
  auto child_t1_a = parent->InsertItem({tag1, -1});  // 0
  auto child_t1_b = parent->InsertItem({tag1, -1});  // 1
  auto child_t2_b = parent->InsertItem({tag2, 1});   // 1 between child_t2_a and child_t2_c

  EXPECT_EQ(child_t1_a->GetTagIndex().index, 0);
  EXPECT_EQ(child_t1_b->GetTagIndex().index, 1);
  EXPECT_EQ(child_t2_a->GetTagIndex().index, 0);
  EXPECT_EQ(child_t2_b->GetTagIndex().index, 1);
  EXPECT_EQ(child_t2_c->GetTagIndex().index, 2);

  EXPECT_EQ(child_t1_a->GetTagIndex().tag, "tag1");
  EXPECT_EQ(child_t1_b->GetTagIndex().tag, "tag1");
  EXPECT_EQ(child_t2_a->GetTagIndex().tag, "tag2");
  EXPECT_EQ(child_t2_b->GetTagIndex().tag, "tag2");
  EXPECT_EQ(child_t2_c->GetTagIndex().tag, "tag2");
}

//! Checks row of item in its tag

TEST_F(SessionItemTest, TagIndexOfItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting two children
  auto child_t2_a = parent->InsertItem({tag2, -1});  // 0
  auto child_t2_c = parent->InsertItem({tag2, -1});  // 2
  auto child_t1_a = parent->InsertItem({tag1, -1});  // 0
  auto child_t1_b = parent->InsertItem({tag1, -1});  // 1
  auto child_t2_b = parent->InsertItem({tag2, 1});   // 1 between child_t2_a and child_t2_c

  EXPECT_EQ(parent->TagIndexOfItem(child_t1_a).index, 0);
  EXPECT_EQ(parent->TagIndexOfItem(child_t1_b).index, 1);
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_a).index, 0);
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_b).index, 1);
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_c).index, 2);

  EXPECT_EQ(parent->TagIndexOfItem(child_t1_a).tag, "tag1");
  EXPECT_EQ(parent->TagIndexOfItem(child_t1_b).tag, "tag1");
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_a).tag, "tag2");
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_b).tag, "tag2");
  EXPECT_EQ(parent->TagIndexOfItem(child_t2_c).tag, "tag2");
}

//! Checks item appearance (enabled/disabled and editable/readonly).

TEST_F(SessionItemTest, Appearance)
{
  SessionItem item;

  // there shouldn't be any data
  auto variant = item.Data(DataRole::kAppearance);
  EXPECT_FALSE(Utils::IsValid(variant));

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
  EXPECT_TRUE(Utils::IsValid(variant));

  // making it readonly
  item.SetEditable(false);
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_FALSE(item.IsEditable());
  EXPECT_TRUE(item.IsVisible());

  // making it hidden
  item.SetVisible(false);
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_FALSE(item.IsEditable());
  EXPECT_FALSE(item.IsVisible());
}

//! Checks item tooltip.

TEST_F(SessionItemTest, Tooltip)
{
  SessionItem item;

  EXPECT_EQ(item.GetToolTip(), "");
  EXPECT_FALSE(item.HasData(DataRole::kTooltip));

  EXPECT_EQ(item.SetToolTip("abc"), &item);
  EXPECT_TRUE(item.HasData(DataRole::kTooltip));
  EXPECT_EQ(item.GetToolTip(), "abc");
}

TEST_F(SessionItemTest, GetItemCount)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag1));
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting two children
  parent->InsertItem({tag1, -1});
  parent->InsertItem({tag2, -1});
  parent->InsertItem({tag2, -1});

  EXPECT_EQ(parent->GetItemCount(tag1), 1);
  EXPECT_EQ(parent->GetItemCount(tag2), 2);
}

TEST_F(SessionItemTest, CastedItemAccess)
{
  const std::string tag = "tag";
  auto parent = std::make_unique<SessionItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag(tag), true);

  auto item0 = parent->InsertItem<ItemA>({tag, -1});
  auto item1 = parent->InsertItem<ItemB>({tag, -1});
  auto item2 = parent->InsertItem<ItemA>({tag, -1});

  EXPECT_EQ(parent->GetItem<ItemA>(tag), item0);
  // current behavior of item<> method, consider to change
  EXPECT_THROW(parent->GetItem<ItemB>(tag), std::runtime_error);

  EXPECT_EQ(parent->GetItems<ItemA>(tag), std::vector<ItemA*>({item0, item2}));
  EXPECT_EQ(parent->GetItems<ItemB>(tag), std::vector<ItemB*>({item1}));
}
