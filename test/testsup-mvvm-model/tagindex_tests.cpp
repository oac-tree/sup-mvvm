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

#include "mvvm/model/tagindex.h"

#include <gtest/gtest.h>

#include <limits>

using namespace mvvm;

/**
 * @brief Tests for TagIndex class.
 */
class TagIndexTests : public ::testing::Test
{
public:
  TagIndex GetTestIndex(const TagIndex& input) { return input; }
};

TEST_F(TagIndexTests, InitialState)
{
  TagIndex tag_index;
  EXPECT_EQ(tag_index.GetTag(), "");
  EXPECT_EQ(tag_index.GetIndex(), 0);
  EXPECT_TRUE(tag_index.IsValid());
  EXPECT_FALSE(tag_index.IsAppend());
}

TEST_F(TagIndexTests, BraceInitializer)
{
  TagIndex tag_index{"abc", 42};
  EXPECT_EQ(tag_index.GetTag(), "abc");
  EXPECT_EQ(tag_index.GetIndex(), 42);

  tag_index = {};
  EXPECT_EQ(tag_index.GetTag(), "");
  EXPECT_EQ(tag_index.GetIndex(), 0);
  EXPECT_TRUE(tag_index.IsValid());

  tag_index = {"cde", 43};
  EXPECT_EQ(tag_index.GetTag(), "cde");
  EXPECT_EQ(tag_index.GetIndex(), 43);

  TagIndex tag_index2 = {"cde"};
  EXPECT_EQ(tag_index2.GetTag(), "cde");
  EXPECT_EQ(tag_index2.GetIndex(), 0);
  EXPECT_TRUE(tag_index2.IsValid());
}

TEST_F(TagIndexTests, EqualityOperators)
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

TEST_F(TagIndexTests, AssignmentOperator)
{
  TagIndex tag1;
  TagIndex tag2{"abc", 42};

  tag1 = tag2;
  EXPECT_EQ(tag1.GetIndex(), 42);
  EXPECT_EQ(tag1.GetTag(), "abc");
}

TEST_F(TagIndexTests, FactoryMethods)
{
  auto tag_index = TagIndex::Append();
  EXPECT_EQ(tag_index.GetTag(), TagIndex::kDefaultTag);
  EXPECT_EQ(tag_index.GetIndex(), -1);
  EXPECT_TRUE(tag_index.IsValid());
  EXPECT_TRUE(tag_index.IsAppend());

  const std::string expected_name("tag");
  tag_index = TagIndex::Append(expected_name);
  EXPECT_EQ(tag_index.GetTag(), expected_name);
  EXPECT_EQ(tag_index.GetIndex(), -1);
  EXPECT_TRUE(tag_index.IsValid());
  EXPECT_TRUE(tag_index.IsAppend());

  tag_index = TagIndex::First(expected_name);
  EXPECT_EQ(tag_index.GetTag(), expected_name);
  EXPECT_EQ(tag_index.GetIndex(), 0);
  EXPECT_TRUE(tag_index.IsValid());
  EXPECT_FALSE(tag_index.IsAppend());

  tag_index = TagIndex::Default(42);
  EXPECT_EQ(tag_index.GetTag(), TagIndex::kDefaultTag);
  EXPECT_EQ(tag_index.GetIndex(), 42);
  EXPECT_TRUE(tag_index.IsValid());
  EXPECT_FALSE(tag_index.IsAppend());

  tag_index = TagIndex::Invalid();
  EXPECT_EQ(tag_index.GetTag(), TagIndex::kDefaultTag);
  EXPECT_FALSE(tag_index.IsValid());
  EXPECT_FALSE(tag_index.IsAppend());
}

TEST_F(TagIndexTests, ImplicitConversion)
{
  auto tag_index = GetTestIndex("abc");
  EXPECT_EQ(tag_index.GetTag(), "abc");
  EXPECT_EQ(tag_index.GetIndex(), 0);
}

TEST_F(TagIndexTests, Next)
{
  {
    TagIndex tag_index("tag", std::numeric_limits<std::size_t>::max());
    EXPECT_FALSE(tag_index.Next().IsValid());
  }

  {
    TagIndex tag_index{"tag", 0};
    EXPECT_EQ(tag_index.Next().GetTag(), "tag");
    EXPECT_EQ(tag_index.Next().GetIndex(), 1);
  }
}

TEST_F(TagIndexTests, Prev)
{
  {
    TagIndex tag_index("tag", 0);
    EXPECT_FALSE(tag_index.Prev().IsValid());
  }

  TagIndex tag_index{"tag", 1};
  EXPECT_EQ(tag_index.Prev().GetTag(), "tag");
  EXPECT_EQ(tag_index.Prev().GetIndex(), 0);
}
