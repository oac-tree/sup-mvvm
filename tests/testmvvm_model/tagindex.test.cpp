// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/tagindex.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing AxisItems.

class TagIndexTest : public ::testing::Test
{
public:
  TagIndex test_method(const TagIndex& input) { return input; }
};

//! Initial state.

TEST_F(TagIndexTest, initialState)
{
  TagIndex tag_index;
  EXPECT_EQ(tag_index.tag, "");
  EXPECT_EQ(tag_index.index, -1);
}

//! Brace initializer.

TEST_F(TagIndexTest, braceInitializer)
{
  TagIndex tag_index{"abc", 42};
  EXPECT_EQ(tag_index.tag, "abc");
  EXPECT_EQ(tag_index.index, 42);

  tag_index = {};
  EXPECT_EQ(tag_index.tag, "");
  EXPECT_EQ(tag_index.index, -1);

  tag_index = {"cde", 43};
  EXPECT_EQ(tag_index.tag, "cde");
  EXPECT_EQ(tag_index.index, 43);

  TagIndex tag_index2 = {"cde"};
  EXPECT_EQ(tag_index2.tag, "cde");
  EXPECT_EQ(tag_index2.index, -1);
}

//! Equality operators.

TEST_F(TagIndexTest, equalityOperators)
{
  TagIndex tag1;
  TagIndex tag2;
  EXPECT_TRUE(tag1 == tag2);
  EXPECT_FALSE(tag1 != tag2);

  TagIndex tag3 = {"abc", 42};
  TagIndex tag4 = {"abc", 42};
  EXPECT_TRUE(tag3 == tag4);
  EXPECT_FALSE(tag3 != tag4);

  TagIndex tag5 = {"abc", 42};
  TagIndex tag6 = {"abc", 43};
  EXPECT_FALSE(tag5 == tag6);
  EXPECT_TRUE(tag5 != tag6);

  TagIndex tag7 = {"a", 42};
  TagIndex tag8 = {"b", 42};
  EXPECT_FALSE(tag7 == tag8);
  EXPECT_TRUE(tag7 != tag8);
}

//! Assignment operators.

TEST_F(TagIndexTest, assignmentOperator)
{
  TagIndex tag1;
  TagIndex tag2{"abc", 42};

  tag1 = tag2;
  EXPECT_EQ(tag1.index, 42);
  EXPECT_EQ(tag1.tag, "abc");
}

//! Factory methods.

TEST_F(TagIndexTest, factoryMethods)
{
  auto tag_index = TagIndex::append();
  EXPECT_EQ(tag_index.tag, "");
  EXPECT_EQ(tag_index.index, -1);

  const std::string expected_name("tag");
  tag_index = TagIndex::append(expected_name);
  EXPECT_EQ(tag_index.tag, expected_name);
  EXPECT_EQ(tag_index.index, -1);

  tag_index = TagIndex::prepend(expected_name);
  EXPECT_EQ(tag_index.tag, expected_name);
  EXPECT_EQ(tag_index.index, 0);
}

//! Implicit type convertion

TEST_F(TagIndexTest, implicitConvertion)
{
  auto tag_index = test_method("abc");
  EXPECT_EQ(tag_index.tag, "abc");
  EXPECT_EQ(tag_index.index, -1);
}

//! Find next tagrow.

TEST_F(TagIndexTest, next)
{
  TagIndex tag_index{"tag", 0};
  EXPECT_EQ(tag_index.next().tag, "tag");
  EXPECT_EQ(tag_index.next().index, 1);
}

//! Find previous tagrow.

TEST_F(TagIndexTest, prev)
{
  TagIndex tag_index{"tag", 1};
  EXPECT_EQ(tag_index.prev().tag, "tag");
  EXPECT_EQ(tag_index.prev().index, 0);
}
