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

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

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
  const TaggedItems tag;

  EXPECT_EQ(tag.GetDefaultTag(), "");
  EXPECT_FALSE(tag.HasTag("abc"));
  EXPECT_EQ(tag.GetTagCount(), 0);
}

//! Registering tags.

TEST_F(TaggedItemsTests, RegisterTag)
{
  const std::string name("tag");
  TaggedItems tag;

  tag.RegisterTag(TagInfo::CreateUniversalTag("abc"));
  EXPECT_TRUE(tag.HasTag("abc"));
  EXPECT_EQ(tag.GetDefaultTag(), "");
  EXPECT_EQ(tag.GetTagCount(), 1);

  // registering default tag
  tag.RegisterTag(TagInfo::CreateUniversalTag("abc2"), /*set_as_default*/ true);
  EXPECT_TRUE(tag.HasTag("abc2"));
  EXPECT_EQ(tag.GetDefaultTag(), "abc2");

  // registering tag with same name is not allowed
  EXPECT_THROW(tag.RegisterTag(TagInfo::CreateUniversalTag("abc")), std::runtime_error);
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTests, CanInsertItem)
{
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag("tag1"));
  tag.RegisterTag(TagInfo::CreatePropertyTag("tag2", "Property"));

  auto item = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.CanInsertItem(item.get(), {"tag1", 0}));
  EXPECT_FALSE(tag.CanInsertItem(item.get(), {"tag2", 0}));
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTests, CanInsertItemForUniversalTag)
{
  TaggedItems tag;
  const std::string tagname = "tag1";
  const int maxItems = 2;
  tag.RegisterTag(TagInfo(tagname, 0, maxItems, std::vector<std::string>() = {}));

  auto [child1, child1_ptr] = CreateItem();
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), {tagname, 0}));
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), {tagname, -1}));
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), {tagname, tag.GetItemCount(tagname)}));
  EXPECT_EQ(tag.InsertItem(std::move(child1), {tagname, -1}), child1_ptr);

  // inserting second child
  auto [child2, child2_ptr] = CreateItem();
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), {tagname, 0}));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), {tagname, -1}));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), {tagname, tag.GetItemCount(tagname)}));
  EXPECT_EQ(tag.InsertItem(std::move(child2), {tagname, -1}), child2_ptr);

  // inserting third child is not possible
  auto child3 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), {tagname, -1}));
}

//! Insert item.

TEST_F(TaggedItemsTests, InsertItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  TaggedItems tag;

  // inserting items without tags defined
  auto item = std::make_unique<SessionItem>();
  EXPECT_THROW(tag.InsertItem(std::move(item), TagIndex::Append()), std::runtime_error);

  // registering tags
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1));
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  EXPECT_EQ(tag.GetTagCount(), 2);

  // inserting items
  auto [child_t1_a, child_t1_a_ptr] = CreateItem();
  auto [child_t1_b, child_t1_b_ptr] = CreateItem();
  auto [child_t2_a, child_t2_a_ptr] = CreateItem();
  auto [child_t2_b, child_t2_b_ptr] = CreateItem();
  auto [child_t2_c, child_t2_c_ptr] = CreateItem();

  EXPECT_EQ(tag.InsertItem(std::move(child_t2_a), TagIndex::Append(tag2)), child_t2_a_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child_t2_c), TagIndex::Append(tag2)), child_t2_c_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child_t1_a), TagIndex::Append(tag1)), child_t1_a_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child_t1_b), TagIndex::Append(tag1)), child_t1_b_ptr);
  // child_t2_b will be between child_t2_a and child_t2_c
  EXPECT_EQ(tag.InsertItem(std::move(child_t2_b), {tag2, 1}), child_t2_b_ptr);

  // checking item order in containers
  std::vector<SessionItem*> expected = {child_t1_a_ptr, child_t1_b_ptr};
  EXPECT_EQ(tag.GetItems(tag1), expected);
  expected = {child_t2_a_ptr, child_t2_b_ptr, child_t2_c_ptr};
  EXPECT_EQ(tag.GetItems(tag2), expected);

  // checking allitems order
  expected = {child_t1_a_ptr, child_t1_b_ptr, child_t2_a_ptr, child_t2_b_ptr, child_t2_c_ptr};
  EXPECT_EQ(tag.GetAllItems(), expected);
}

//! Testing method TagIndexOfItem.

TEST_F(TaggedItemsTests, GetTagIndexOfItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting children
  auto [child_t1_a, child_t1_a_ptr] = CreateItem();
  auto [child_t1_b, child_t1_b_ptr] = CreateItem();
  auto [child_t2_a, child_t2_a_ptr] = CreateItem();
  tag.InsertItem(std::move(child_t1_a), TagIndex::Append());  // 0
  tag.InsertItem(std::move(child_t1_b), TagIndex::Append());  // 1
  tag.InsertItem(std::move(child_t2_a), {tag2, 0});           // 0

  // checking children tag and row
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_a_ptr).tag, tag1);
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_b_ptr).tag, tag1);
  EXPECT_EQ(tag.TagIndexOfItem(child_t2_a_ptr).tag, tag2);
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_a_ptr).index, 0);
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_b_ptr).index, 1);
  EXPECT_EQ(tag.TagIndexOfItem(child_t2_a_ptr).index, 0);

  // alien item has no tag and -1 row
  auto alien = std::make_unique<SessionItem>();
  EXPECT_EQ(tag.TagIndexOfItem(alien.get()), TagIndex("", -1));

  // the same for nullptr
  EXPECT_EQ(tag.TagIndexOfItem(nullptr), TagIndex("", -1));
}

//! Testing method getItem.

TEST_F(TaggedItemsTests, GetItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting children
  auto [child_t1_a, child_t1_a_ptr] = CreateItem();
  auto [child_t1_b, child_t1_b_ptr] = CreateItem();
  auto [child_t2_a, child_t2_a_ptr] = CreateItem();
  tag.InsertItem(std::move(child_t1_a), TagIndex::Append());  // 0
  tag.InsertItem(std::move(child_t1_b), TagIndex::Append());  // 1
  tag.InsertItem(std::move(child_t2_a), {tag2, 0});           // 0

  EXPECT_EQ(tag.GetItem({tag1, 0}), child_t1_a_ptr);
  EXPECT_EQ(tag.GetItem({tag1, 1}), child_t1_b_ptr);
  EXPECT_EQ(tag.GetItem({tag2, 0}), child_t2_a_ptr);
  EXPECT_EQ(tag.GetItem({tag2, 2}), nullptr);
}

//! Testing method getItem.

TEST_F(TaggedItemsTests, TakeItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";
  const std::string model_type("model");

  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // taking non existing items
  EXPECT_EQ(tag.TakeItem({"", 0}), nullptr);

  // inserting items
  auto [child1, child1_ptr] = CreateItem<TestItem>(model_type);
  auto [child2, child2_ptr] = CreateItem<TestItem>(model_type);
  auto [child3, child3_ptr] = CreateItem<TestItem>(model_type);
  auto [child4, child4_ptr] = CreateItem<TestItem>(model_type);
  EXPECT_EQ(tag.InsertItem(std::move(child1), TagIndex::Append()), child1_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child2), TagIndex::Append()), child2_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child3), TagIndex::Append()), child3_ptr);
  EXPECT_EQ(tag.InsertItem(std::move(child4), TagIndex::Append(tag2)), child4_ptr);

  // taking item in between
  EXPECT_TRUE(tag.CanTakeItem({"", 1}));
  auto taken2 = tag.TakeItem({"", 1});
  EXPECT_EQ(child2_ptr, taken2.get());

  // order of remaining children
  EXPECT_EQ(tag.GetItems(tag1), std::vector<SessionItem*>({child1_ptr, child3_ptr}));
  EXPECT_EQ(tag.GetItems(tag2), std::vector<SessionItem*>({child4_ptr}));

  // taking non existing items
  EXPECT_FALSE(tag.CanTakeItem({"", -1}));
  EXPECT_EQ(tag.TakeItem({"", -1}), nullptr);
}
