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

#include "mvvm/model/tagged_items.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/test/test_utils.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace mvvm;

//! Tests for SessionItemTags class.

class TaggedItemsTests : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    TestItem(const std::string& model_type) : SessionItem(model_type) {}
  };

  template <typename T = SessionItem, typename... Args>
  static auto CreateItem(Args&&... args)
  {
    return testutils::CreateTestData<T>(std::forward<Args>(args)...);
  }
};

//! Initial state of emty SessionItemTags.

TEST_F(TaggedItemsTests, InitialState)
{
  const std::string name("tag");
  const TaggedItems items;

  EXPECT_EQ(items.GetDefaultTag(), "");
  EXPECT_FALSE(items.HasTag("abc"));
  EXPECT_EQ(items.GetTagCount(), 0);
}

//! Registering tags.

TEST_F(TaggedItemsTests, RegisterTag)
{
  const std::string name("tag");
  TaggedItems items;

  items.RegisterTag(TagInfo::CreateUniversalTag("abc"));
  EXPECT_TRUE(items.HasTag("abc"));
  EXPECT_EQ(items.GetDefaultTag(), "");
  EXPECT_EQ(items.GetTagCount(), 1);

  // registering default tag
  items.RegisterTag(TagInfo::CreateUniversalTag("abc2"), /*set_as_default*/ true);
  EXPECT_TRUE(items.HasTag("abc2"));
  EXPECT_EQ(items.GetDefaultTag(), "abc2");

  // registering tag with same name is not allowed
  EXPECT_THROW(items.RegisterTag(TagInfo::CreateUniversalTag("abc")), std::runtime_error);
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTests, CanInsertItem)
{
  TaggedItems items;
  items.RegisterTag(TagInfo::CreateUniversalTag("tag1"));
  items.RegisterTag(TagInfo::CreatePropertyTag("tag2", "Property"));

  auto item = std::make_unique<SessionItem>();
  EXPECT_TRUE(items.CanInsertItem(item.get(), {"tag1", 0}));
  EXPECT_FALSE(items.CanInsertItem(item.get(), {"tag2", 0}));
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTests, CanInsertItemForUniversalTag)
{
  TaggedItems items;
  const std::string tagname = "tag1";
  const int maxItems = 2;
  items.RegisterTag(TagInfo(tagname, 0, maxItems, std::vector<std::string>() = {}));

  auto [child1, child1_ptr] = CreateItem();
  EXPECT_TRUE(items.CanInsertItem(child1.get(), {tagname, 0}));
  EXPECT_TRUE(items.CanInsertItem(child1.get(), {tagname, -1}));
  EXPECT_TRUE(items.CanInsertItem(child1.get(), {tagname, items.GetItemCount(tagname)}));
  EXPECT_EQ(items.InsertItem(std::move(child1), {tagname, -1}), child1_ptr);

  // inserting second child
  auto [child2, child2_ptr] = CreateItem();
  EXPECT_TRUE(items.CanInsertItem(child2.get(), {tagname, 0}));
  EXPECT_TRUE(items.CanInsertItem(child2.get(), {tagname, -1}));
  EXPECT_TRUE(items.CanInsertItem(child2.get(), {tagname, items.GetItemCount(tagname)}));
  EXPECT_EQ(items.InsertItem(std::move(child2), {tagname, -1}), child2_ptr);

  // inserting third child is not possible
  auto child3 = std::make_unique<SessionItem>();
  EXPECT_FALSE(items.CanInsertItem(child3.get(), {tagname, -1}));
}

//! Insert item.

TEST_F(TaggedItemsTests, InsertItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  TaggedItems items;

  // inserting items without tags defined
  auto item = std::make_unique<SessionItem>();
  EXPECT_THROW(items.InsertItem(std::move(item), TagIndex::Append()), std::runtime_error);

  // registering tags
  items.RegisterTag(TagInfo::CreateUniversalTag(tag1));
  items.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  EXPECT_EQ(items.GetTagCount(), 2);

  // inserting items
  auto [child_t1_a, child_t1_a_ptr] = CreateItem();
  auto [child_t1_b, child_t1_b_ptr] = CreateItem();
  auto [child_t2_a, child_t2_a_ptr] = CreateItem();
  auto [child_t2_b, child_t2_b_ptr] = CreateItem();
  auto [child_t2_c, child_t2_c_ptr] = CreateItem();

  EXPECT_EQ(items.InsertItem(std::move(child_t2_a), TagIndex::Append(tag2)), child_t2_a_ptr);
  EXPECT_EQ(items.InsertItem(std::move(child_t2_c), TagIndex::Append(tag2)), child_t2_c_ptr);
  EXPECT_EQ(items.InsertItem(std::move(child_t1_a), TagIndex::Append(tag1)), child_t1_a_ptr);
  EXPECT_EQ(items.InsertItem(std::move(child_t1_b), TagIndex::Append(tag1)), child_t1_b_ptr);
  // child_t2_b will be between child_t2_a and child_t2_c
  EXPECT_EQ(items.InsertItem(std::move(child_t2_b), {tag2, 1}), child_t2_b_ptr);

  // checking item order in containers
  std::vector<SessionItem*> expected = {child_t1_a_ptr, child_t1_b_ptr};
  EXPECT_EQ(items.GetItems(tag1), expected);
  expected = {child_t2_a_ptr, child_t2_b_ptr, child_t2_c_ptr};
  EXPECT_EQ(items.GetItems(tag2), expected);

  // checking allitems order
  expected = {child_t1_a_ptr, child_t1_b_ptr, child_t2_a_ptr, child_t2_b_ptr, child_t2_c_ptr};
  EXPECT_EQ(items.GetAllItems(), expected);
}

//! Testing method TagIndexOfItem.

TEST_F(TaggedItemsTests, GetTagIndexOfItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems items;
  items.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  items.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting children
  auto [child_t1_a, child_t1_a_ptr] = CreateItem();
  auto [child_t1_b, child_t1_b_ptr] = CreateItem();
  auto [child_t2_a, child_t2_a_ptr] = CreateItem();
  items.InsertItem(std::move(child_t1_a), TagIndex::Append());  // 0
  items.InsertItem(std::move(child_t1_b), TagIndex::Append());  // 1
  items.InsertItem(std::move(child_t2_a), {tag2, 0});           // 0

  // checking children tag and row
  EXPECT_EQ(items.TagIndexOfItem(child_t1_a_ptr).tag, tag1);
  EXPECT_EQ(items.TagIndexOfItem(child_t1_b_ptr).tag, tag1);
  EXPECT_EQ(items.TagIndexOfItem(child_t2_a_ptr).tag, tag2);
  EXPECT_EQ(items.TagIndexOfItem(child_t1_a_ptr).index, 0);
  EXPECT_EQ(items.TagIndexOfItem(child_t1_b_ptr).index, 1);
  EXPECT_EQ(items.TagIndexOfItem(child_t2_a_ptr).index, 0);

  // alien item has no tag and -1 row
  auto alien = std::make_unique<SessionItem>();
  EXPECT_EQ(items.TagIndexOfItem(alien.get()), TagIndex("", -1));

  // the same for nullptr
  EXPECT_EQ(items.TagIndexOfItem(nullptr), TagIndex("", -1));
}

//! Testing method getItem.

TEST_F(TaggedItemsTests, GetItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems items;
  items.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  items.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting children
  auto [child_t1_a, child_t1_a_ptr] = CreateItem();
  auto [child_t1_b, child_t1_b_ptr] = CreateItem();
  auto [child_t2_a, child_t2_a_ptr] = CreateItem();
  items.InsertItem(std::move(child_t1_a), TagIndex::Append());  // 0
  items.InsertItem(std::move(child_t1_b), TagIndex::Append());  // 1
  items.InsertItem(std::move(child_t2_a), {tag2, 0});           // 0

  EXPECT_EQ(items.GetItem({tag1, 0}), child_t1_a_ptr);
  EXPECT_EQ(items.GetItem({tag1, 1}), child_t1_b_ptr);
  EXPECT_EQ(items.GetItem({tag2, 0}), child_t2_a_ptr);
  EXPECT_EQ(items.GetItem({tag2, 2}), nullptr);
}

//! Testing method getItem.

TEST_F(TaggedItemsTests, TakeItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";
  const std::string model_type("model");

  TaggedItems items;
  items.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  items.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // taking non existing items
  EXPECT_EQ(items.TakeItem({"", 0}), nullptr);

  // inserting items
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  auto [child2, child2_ptr] = CreateItem<TestItem>(model_type);
  auto [child3, child3_ptr] = CreateItem<TestItem>(model_type);
  auto [child4, child4_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_EQ(items.InsertItem(std::move(child1), TagIndex::Append()), child1_ptr);
  EXPECT_EQ(items.InsertItem(std::move(child2), TagIndex::Append()), child2_ptr);
  EXPECT_EQ(items.InsertItem(std::move(child3), TagIndex::Append()), child3_ptr);
  EXPECT_EQ(items.InsertItem(std::move(child4), TagIndex::Append(tag2)), child4_ptr);

  // taking item in between
  EXPECT_TRUE(items.CanTakeItem({"", 1}));
  auto taken2 = items.TakeItem({"", 1});
  EXPECT_EQ(child2_ptr, taken2.get());

  // order of remaining children
  EXPECT_EQ(items.GetItems(tag1), std::vector<SessionItem*>({child1_ptr, child3_ptr}));
  EXPECT_EQ(items.GetItems(tag2), std::vector<SessionItem*>({child4_ptr}));

  // taking non existing items
  EXPECT_FALSE(items.CanTakeItem({"", -1}));
  EXPECT_EQ(items.TakeItem({"", -1}), nullptr);
}

//! Testing method TaggedItems::Clone

TEST_F(TaggedItemsTests, Clone)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating object with two item containers, and insert one item into each
  TaggedItems items;
  items.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  items.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  auto [child_t1, child_t1_ptr] = CreateItem();
  auto [child_t2, child_t2_ptr] = CreateItem();
  items.InsertItem(std::move(child_t1), TagIndex::Append())->SetDisplayName("abc");
  items.InsertItem(std::move(child_t2), {tag2, 0})->SetDisplayName("def");

  {  // deep copy
    auto clone = items.Clone(/*make_unique_id*/ true);
    EXPECT_EQ(clone->GetDefaultTag(), tag1);
    ASSERT_EQ(clone->GetTagCount(), 2);
    EXPECT_NE(&clone->ContainerAt(0), &items.ContainerAt(0));
    EXPECT_NE(clone->GetItem({tag1, 0})->GetIdentifier(), child_t1_ptr->GetIdentifier());
    EXPECT_NE(clone->GetItem({tag2, 0})->GetIdentifier(), child_t2_ptr->GetIdentifier());
    EXPECT_EQ(clone->GetItem({tag1, 0})->GetDisplayName(), std::string("abc"));
    EXPECT_EQ(clone->GetItem({tag2, 0})->GetDisplayName(), std::string("def"));
  }

  {  // clone
    auto clone = items.Clone(/*make_unique_id*/ false);
    EXPECT_EQ(clone->GetDefaultTag(), tag1);
    ASSERT_EQ(clone->GetTagCount(), 2);
    EXPECT_NE(&clone->ContainerAt(0), &items.ContainerAt(0));
    EXPECT_EQ(clone->GetItem({tag1, 0})->GetIdentifier(), child_t1_ptr->GetIdentifier());
    EXPECT_EQ(clone->GetItem({tag2, 0})->GetIdentifier(), child_t2_ptr->GetIdentifier());
    EXPECT_EQ(clone->GetItem({tag1, 0})->GetDisplayName(), std::string("abc"));
    EXPECT_EQ(clone->GetItem({tag2, 0})->GetDisplayName(), std::string("def"));
  }
}
