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
};

//! Initial state of emty SessionItemTags.

TEST_F(TaggedItemsTest, initialState)
{
  const std::string name("tag");
  TaggedItems tag;
  EXPECT_EQ(tag.defaultTag(), "");

  EXPECT_FALSE(tag.isTag("abc"));

  EXPECT_EQ(tag.tagsCount(), 0);
}

//! Registering tags.

TEST_F(TaggedItemsTest, registerTag)
{
  const std::string name("tag");
  TaggedItems tag;

  tag.registerTag(TagInfo::universalTag("abc"));
  EXPECT_TRUE(tag.isTag("abc"));
  EXPECT_EQ(tag.defaultTag(), "");
  EXPECT_EQ(tag.tagsCount(), 1);

  // registering default tag
  tag.registerTag(TagInfo::universalTag("abc2"), /*set_as_default*/ true);
  EXPECT_TRUE(tag.isTag("abc2"));
  EXPECT_EQ(tag.defaultTag(), "abc2");

  // registering tag with same name is not allowed
  EXPECT_THROW(tag.registerTag(TagInfo::universalTag("abc")), std::runtime_error);
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTest, canInsertItem)
{
  TaggedItems tag;
  tag.registerTag(TagInfo::universalTag("tag1"));
  tag.registerTag(TagInfo::propertyTag("tag2", "Property"));

  auto item = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.canInsertItem(item.get(), {"tag1", 0}));
  EXPECT_FALSE(tag.canInsertItem(item.get(), {"tag2", 0}));
}

//! Testing ::canInsertItem.

TEST_F(TaggedItemsTest, canInsertItemForUniversalTag)
{
  TaggedItems tag;
  const std::string tagname = "tag1";
  const int maxItems = 2;
  tag.registerTag(TagInfo(tagname, 0, maxItems, std::vector<std::string>() = {}));

  auto child1 = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.canInsertItem(child1.get(), {tagname, 0}));
  EXPECT_TRUE(tag.canInsertItem(child1.get(), {tagname, -1}));
  EXPECT_TRUE(tag.canInsertItem(child1.get(), {tagname, tag.itemCount(tagname)}));
  EXPECT_TRUE(tag.insertItem(child1.release(), {tagname, -1}));

  // inserting second child
  auto child2 = std::make_unique<SessionItem>();
  EXPECT_TRUE(tag.canInsertItem(child2.get(), {tagname, 0}));
  EXPECT_TRUE(tag.canInsertItem(child2.get(), {tagname, -1}));
  EXPECT_TRUE(tag.canInsertItem(child2.get(), {tagname, tag.itemCount(tagname)}));
  EXPECT_TRUE(tag.insertItem(child2.release(), {tagname, -1}));

  // inserting third child is not possible
  auto child3 = std::make_unique<SessionItem>();
  EXPECT_FALSE(tag.canInsertItem(child3.get(), {tagname, -1}));
}

//! Insert item.

TEST_F(TaggedItemsTest, insertItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  TaggedItems tag;

  // inserting items without tags defined
  auto item = std::make_unique<SessionItem>();
  EXPECT_THROW(tag.insertItem(item.get(), TagIndex::append()), std::runtime_error);

  // registering tags
  tag.registerTag(TagInfo::universalTag(tag1));
  tag.registerTag(TagInfo::universalTag(tag2));

  EXPECT_EQ(tag.tagsCount(), 2);

  // inserting items
  auto child_t1_a = new SessionItem;
  auto child_t1_b = new SessionItem;
  auto child_t2_a = new SessionItem;
  auto child_t2_b = new SessionItem;
  auto child_t2_c = new SessionItem;
  EXPECT_TRUE(tag.insertItem(child_t2_a, TagIndex::append(tag2)));
  EXPECT_TRUE(tag.insertItem(child_t2_c, TagIndex::append(tag2)));
  EXPECT_TRUE(tag.insertItem(child_t1_a, TagIndex::append(tag1)));
  EXPECT_TRUE(tag.insertItem(child_t1_b, TagIndex::append(tag1)));
  EXPECT_TRUE(tag.insertItem(child_t2_b, {tag2, 1}));  // between child_t2_a and child_t2_c

  // checking item order in containers
  std::vector<SessionItem*> expected = {child_t1_a, child_t1_b};
  EXPECT_EQ(tag.getItems(tag1), expected);
  expected = {child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(tag.getItems(tag2), expected);

  // checking allitems order
  expected = {child_t1_a, child_t1_b, child_t2_a, child_t2_b, child_t2_c};
  EXPECT_EQ(tag.allitems(), expected);
}

//! Testing method tagRowOfItem.

TEST_F(TaggedItemsTest, tagRowOfItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems tag;
  tag.registerTag(TagInfo::universalTag(tag1), /*set_as_default*/ true);
  tag.registerTag(TagInfo::universalTag(tag2));

  // inserting children
  auto child_t1_a = new SessionItem;
  auto child_t1_b = new SessionItem;
  auto child_t2_a = new SessionItem;
  tag.insertItem(child_t1_a, TagIndex::append());  // 0
  tag.insertItem(child_t1_b, TagIndex::append());  // 1
  tag.insertItem(child_t2_a, {tag2, 0});         // 0

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

TEST_F(TaggedItemsTest, getItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";

  // creating parent with one tag
  TaggedItems tag;
  tag.registerTag(TagInfo::universalTag(tag1), /*set_as_default*/ true);
  tag.registerTag(TagInfo::universalTag(tag2));

  // inserting children
  auto child_t1_a = new SessionItem;
  auto child_t1_b = new SessionItem;
  auto child_t2_a = new SessionItem;
  tag.insertItem(child_t1_a, TagIndex::append());  // 0
  tag.insertItem(child_t1_b, TagIndex::append());  // 1
  tag.insertItem(child_t2_a, {tag2, 0});         // 0

  EXPECT_EQ(tag.getItem({tag1, 0}), child_t1_a);
  EXPECT_EQ(tag.getItem({tag1, 1}), child_t1_b);
  EXPECT_EQ(tag.getItem({tag2, 0}), child_t2_a);
  EXPECT_EQ(tag.getItem({tag2, 2}), nullptr);
}

//! Testing method getItem.

TEST_F(TaggedItemsTest, takeItem)
{
  const std::string tag1 = "tag1";
  const std::string tag2 = "tag2";
  const std::string model_type("model");

  TaggedItems tag;
  tag.registerTag(TagInfo::universalTag(tag1), /*set_as_default*/ true);
  tag.registerTag(TagInfo::universalTag(tag2));

  // taking non existing items
  EXPECT_EQ(tag.takeItem({"", 0}), nullptr);

  // inserting items
  auto child1 = new SessionItem(model_type);
  auto child2 = new SessionItem(model_type);
  auto child3 = new SessionItem(model_type);
  auto child4 = new SessionItem(model_type);
  EXPECT_TRUE(tag.insertItem(child1, TagIndex::append()));
  EXPECT_TRUE(tag.insertItem(child2, TagIndex::append()));
  EXPECT_TRUE(tag.insertItem(child3, TagIndex::append()));
  EXPECT_TRUE(tag.insertItem(child4, TagIndex::append(tag2)));

  // taking item in between
  EXPECT_TRUE(tag.canTakeItem({"", 1}));
  auto taken2 = tag.takeItem({"", 1});
  EXPECT_EQ(child2, taken2);
  delete taken2;

  // order of remaining children
  std::vector<SessionItem*> expected = {child1, child3};
  EXPECT_EQ(tag.getItems(tag1), expected);
  expected = {child4};
  EXPECT_EQ(tag.getItems(tag2), expected);

  // taking non existing items
  EXPECT_FALSE(tag.canTakeItem({"", -1}));
  EXPECT_EQ(tag.takeItem({"", -1}), nullptr);
}

//! Testing isSinglePropertyTag.

TEST_F(TaggedItemsTest, isSinglePropertyTag)
{
  TaggedItems tag;
  tag.registerTag(TagInfo::universalTag("universal"), /*set_as_default*/ true);
  EXPECT_FALSE(tag.isSinglePropertyTag("universal"));

  tag.registerTag(TagInfo::propertyTag("property_tag", "Vector"));
  EXPECT_TRUE(tag.isSinglePropertyTag("property_tag"));

  EXPECT_FALSE(tag.isSinglePropertyTag("unexisting tag"));
}
