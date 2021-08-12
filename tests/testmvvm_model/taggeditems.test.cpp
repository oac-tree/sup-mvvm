// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/taggeditems.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/model/taginfo.h"

#include <gtest/gtest.h>

#include <stdexcept>

using namespace ModelView;

//! Tests for SessionItemTags class.

class TaggedItemsTest : public ::testing::Test
{
public:
  class TestItem : public SessionItem
  {
  public:
    TestItem(const std::string& model_type) : SessionItem(model_type) {}
  };
};

//! Initial state of emty SessionItemTags.

TEST_F(TaggedItemsTest, InitialState)
{
  const std::string name("tag");
  TaggedItems tag;
  EXPECT_EQ(tag.GetDefaultTag(), "");

  EXPECT_FALSE(tag.HasTag("abc"));

  EXPECT_EQ(tag.GetTagsCount(), 0);
}

//! Registering tags.

TEST_F(TaggedItemsTest, RegisterTag)
{
  const std::string name("tag");
  TaggedItems tag;

  tag.RegisterTag(TagInfo::CreateUniversalTag("abc"));
  EXPECT_TRUE(tag.HasTag("abc"));
  EXPECT_EQ(tag.GetDefaultTag(), "");
  EXPECT_EQ(tag.GetTagsCount(), 1);

  // registering default tag
  tag.RegisterTag(TagInfo::CreateUniversalTag("abc2"), /*set_as_default*/ true);
  EXPECT_TRUE(tag.HasTag("abc2"));
  EXPECT_EQ(tag.GetDefaultTag(), "abc2");

  // registering tag with same name is not allowed
  EXPECT_THROW(tag.RegisterTag(TagInfo::CreateUniversalTag("abc")), std::runtime_error);
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTest, CanInsertItem)
{
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag("tag1"));
  tag.RegisterTag(TagInfo::CreatePropertyTag("tag2", "Property"));

  auto item = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.CanInsertItem(item.get(), {"tag1", 0}));
  EXPECT_FALSE(tag.CanInsertItem(item.get(), {"tag2", 0}));
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTest, CanInsertItemForUniversalTag)
{
  TaggedItems tag;
  const std::string tagname = "tag1";
  const int maxItems = 2;
  tag.RegisterTag(TagInfo(tagname, 0, maxItems, std::vector<std::string>() = {}));

  auto child1 = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), {tagname, 0}));
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), {tagname, -1}));
  EXPECT_TRUE(tag.CanInsertItem(child1.get(), {tagname, tag.GetItemCount(tagname)}));
  EXPECT_TRUE(tag.InsertItem(child1.release(), {tagname, -1}));

  // inserting second child
  auto child2 = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), {tagname, 0}));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), {tagname, -1}));
  EXPECT_TRUE(tag.CanInsertItem(child2.get(), {tagname, tag.GetItemCount(tagname)}));
  EXPECT_TRUE(tag.InsertItem(child2.release(), {tagname, -1}));

  // inserting third child is not possible
  auto child3 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.CanInsertItem(child3.get(), {tagname, -1}));
}

//! Insert item.

TEST_F(TaggedItemsTest, InsertItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  TaggedItems tag;

  // inserting items without tags defined
  auto item = std::make_unique<SessionItem>();
  EXPECT_THROW(tag.InsertItem(item.get(), TagIndex::append()), std::runtime_error);

  // registering tags
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1));
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  EXPECT_EQ(tag.GetTagsCount(), 2);

  // inserting items
  auto child_t1_a = new SessionItem;
  auto child_t1_b = new SessionItem;
  auto child_t2_a = new SessionItem;
  auto child_t2_b = new SessionItem;
  auto child_t2_c = new SessionItem;
  EXPECT_TRUE(tag.InsertItem(child_t2_a, TagIndex::append(tag2)));
  EXPECT_TRUE(tag.InsertItem(child_t2_c, TagIndex::append(tag2)));
  EXPECT_TRUE(tag.InsertItem(child_t1_a, TagIndex::append(tag1)));
  EXPECT_TRUE(tag.InsertItem(child_t1_b, TagIndex::append(tag1)));
  EXPECT_TRUE(tag.InsertItem(child_t2_b, {tag2, 1}));  // between child_t2_a and child_t2_c

  // checking item order in containers
  std::vector<SessionItem*> expected = {child_t1_a, child_t1_b};
  EXPECT_EQ(tag.GetItems(tag1), expected);
  expected = {child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(tag.GetItems(tag2), expected);

  // checking allitems order
  expected = {child_t1_a, child_t1_b, child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(tag.GetAllItems(), expected);
}

//! Testing method tagRowOfItem.

TEST_F(TaggedItemsTest, TagRowOfItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting children
  auto child_t1_a = new SessionItem;
  auto child_t1_b = new SessionItem;
  auto child_t2_a = new SessionItem;
  tag.InsertItem(child_t1_a, TagIndex::append());  // 0
  tag.InsertItem(child_t1_b, TagIndex::append());  // 1
  tag.InsertItem(child_t2_a, {tag2, 0});           // 0

  // checking children tag and row
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_a).tag, tag1);
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_b).tag, tag1);
  EXPECT_EQ(tag.TagIndexOfItem(child_t2_a).tag, tag2);
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_a).index, 0);
  EXPECT_EQ(tag.TagIndexOfItem(child_t1_b).index, 1);
  EXPECT_EQ(tag.TagIndexOfItem(child_t2_a).index, 0);

  // alien item has no tag and -1 row
  auto alien = std::make_unique<SessionItem>();
  EXPECT_EQ(tag.TagIndexOfItem(alien.get()).tag, "");
  EXPECT_EQ(tag.TagIndexOfItem(alien.get()).index, -1);

  // the same for nullptr
  EXPECT_EQ(tag.TagIndexOfItem(nullptr).tag, "");
  EXPECT_EQ(tag.TagIndexOfItem(nullptr).index, -1);
}

//! Testing method getItem.

TEST_F(TaggedItemsTest, GetItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag1), /*set_as_default*/ true);
  tag.RegisterTag(TagInfo::CreateUniversalTag(tag2));

  // inserting children
  auto child_t1_a = new SessionItem;
  auto child_t1_b = new SessionItem;
  auto child_t2_a = new SessionItem;
  tag.InsertItem(child_t1_a, TagIndex::append());  // 0
  tag.InsertItem(child_t1_b, TagIndex::append());  // 1
  tag.InsertItem(child_t2_a, {tag2, 0});           // 0

  EXPECT_EQ(tag.GetItem({tag1, 0}), child_t1_a);
  EXPECT_EQ(tag.GetItem({tag1, 1}), child_t1_b);
  EXPECT_EQ(tag.GetItem({tag2, 0}), child_t2_a);
  EXPECT_EQ(tag.GetItem({tag2, 2}), nullptr);
}

//! Testing method getItem.

TEST_F(TaggedItemsTest, TakeItem)
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
  auto child1 = new TestItem(model_type);
  auto child2 = new TestItem(model_type);
  auto child3 = new TestItem(model_type);
  auto child4 = new TestItem(model_type);
  EXPECT_TRUE(tag.InsertItem(child1, TagIndex::append()));
  EXPECT_TRUE(tag.InsertItem(child2, TagIndex::append()));
  EXPECT_TRUE(tag.InsertItem(child3, TagIndex::append()));
  EXPECT_TRUE(tag.InsertItem(child4, TagIndex::append(tag2)));

  // taking item in between
  EXPECT_TRUE(tag.CanTakeItem({"", 1}));
  auto taken2 = tag.TakeItem({"", 1});
  EXPECT_EQ(child2, taken2);
  delete taken2;

  // order of remaining children
  std::vector<SessionItem*> expected = {child1, child3};
  EXPECT_EQ(tag.GetItems(tag1), expected);
  expected = {child4};
  EXPECT_EQ(tag.GetItems(tag2), expected);

  // taking non existing items
  EXPECT_FALSE(tag.CanTakeItem({"", -1}));
  EXPECT_EQ(tag.TakeItem({"", -1}), nullptr);
}

//! Testing isSinglePropertyTag.

TEST_F(TaggedItemsTest, IsSinglePropertyTag)
{
  TaggedItems tag;
  tag.RegisterTag(TagInfo::CreateUniversalTag("universal"), /*set_as_default*/ true);
  EXPECT_FALSE(tag.IsSinglePropertyTag("universal"));

  tag.RegisterTag(TagInfo::CreatePropertyTag("property_tag", "Vector"));
  EXPECT_TRUE(tag.IsSinglePropertyTag("property_tag"));

  EXPECT_FALSE(tag.IsSinglePropertyTag("unexisting tag"));
}
