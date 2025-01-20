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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/taginfo.h"

#include <mvvm/core/exceptions.h>

#include <gtest/gtest.h>

#include <limits>

using namespace mvvm;

/**
 * @brief Tests for TagInfo class.
 */
class TagInfoTests : public ::testing::Test
{
};

TEST_F(TagInfoTests, MainCtor)
{
  EXPECT_THROW(TagInfo("", 0, 1, {}), RuntimeException);
  EXPECT_THROW(TagInfo("abc", 1, 0, {}), RuntimeException);

  {
    TagInfo tag("abc", {}, {}, {});
    EXPECT_EQ(tag.GetName(), "abc");
    EXPECT_EQ(tag.GetMin(), 0);
    EXPECT_EQ(tag.GetMax(), std::numeric_limits<size_t>::max());
    EXPECT_TRUE(tag.GetItemTypes().empty());
  }

  {
    TagInfo tag("abc", 42, 43, {"def", "ghk"});
    EXPECT_EQ(tag.GetName(), "abc");
    EXPECT_EQ(tag.GetMin(), 42);
    EXPECT_EQ(tag.GetMax(), 43);
    EXPECT_EQ(tag.GetItemTypes(), std::vector<std::string>({"def", "ghk"}));
  }
}

TEST_F(TagInfoTests, CreateUniversalTag)
{
  // initial state
  const auto tag = TagInfo::CreateUniversalTag("name");
  EXPECT_EQ(tag.GetName(), std::string("name"));
  EXPECT_TRUE(tag.IsValidType(""));
  EXPECT_TRUE(tag.IsValidType("abc"));
  EXPECT_FALSE(tag.HasMin());
  EXPECT_FALSE(tag.HasMax());
}

TEST_F(TagInfoTests, PropertyTag)
{
  // initial state
  const auto tag = TagInfo::CreatePropertyTag("name", "model_type");

  EXPECT_EQ(tag.GetName(), std::string("name"));
  EXPECT_EQ(tag.GetMin(), 1);
  EXPECT_EQ(tag.GetMax(), 1);
  EXPECT_TRUE(tag.IsValidType("model_type"));
  EXPECT_FALSE(tag.IsValidType("abc"));
}

TEST_F(TagInfoTests, EqualityOperator)
{
  // default constructor
  const TagInfo tag1{"abc", {}, {}, {}};
  const TagInfo tag2{"abc", {}, {}, {}};
  EXPECT_TRUE(tag1 == tag2);
  EXPECT_FALSE(tag1 != tag2);

  // same property tag
  const auto tag3 = TagInfo::CreatePropertyTag("name", "model_type");
  const auto tag4 = TagInfo::CreatePropertyTag("name", "model_type");
  EXPECT_TRUE(tag3 == tag4);
  EXPECT_FALSE(tag3 != tag4);

  // same universal tag
  const auto tag5 = TagInfo::CreateUniversalTag("name");
  const auto tag6 = TagInfo::CreateUniversalTag("name");
  EXPECT_TRUE(tag5 == tag6);
  EXPECT_FALSE(tag5 != tag6);

  // different tag
  const TagInfo tag7("tag7", 0, 1, std::vector<std::string>());
  const TagInfo tag8("tag8", 0, 1, std::vector<std::string>());
  EXPECT_FALSE(tag7 == tag8);
  EXPECT_TRUE(tag7 != tag8);
}

TEST_F(TagInfoTests, CopyConstructor)
{
  {
    const TagInfo tag_info{"abc", {}, {}, {}};

    const TagInfo copy(tag_info);
    EXPECT_EQ(copy.GetName(), std::string("abc"));
    EXPECT_FALSE(copy.HasMin());
    EXPECT_FALSE(copy.HasMax());
    EXPECT_TRUE(copy.IsValidType(""));
    EXPECT_TRUE(copy.IsValidType("abc"));
    EXPECT_TRUE(tag_info == copy);
  }

  {  // from parameters
    const TagInfo tag_info("abc", 0, 1, {"type"});

    const TagInfo copy(tag_info);
    EXPECT_EQ(copy.GetName(), std::string("abc"));
    EXPECT_EQ(copy.GetMin(), 0);
    EXPECT_EQ(copy.GetMax(), 1);
    EXPECT_FALSE(copy.IsValidType(""));
    EXPECT_TRUE(copy.IsValidType("type"));
    EXPECT_TRUE(tag_info == copy);
  }
}

TEST_F(TagInfoTests, AssignmentOperator)
{
  {
    const TagInfo tag_info{"abc", {}, {}, {}};

    TagInfo copy{"def", {}, {}, {}};
    copy = tag_info;

    EXPECT_EQ(copy.GetName(), std::string("abc"));
    EXPECT_FALSE(copy.HasMin());
    EXPECT_FALSE(copy.HasMax());
    EXPECT_TRUE(copy.IsValidType(""));
    EXPECT_TRUE(copy.IsValidType("abc"));
    EXPECT_TRUE(tag_info == copy);
  }

  {  // from parameters
    const TagInfo tag_info("abc", 0, 1, {"type"});

    TagInfo copy{"def", {}, {}, {}};
    copy = tag_info;

    EXPECT_EQ(copy.GetName(), std::string("abc"));
    EXPECT_EQ(copy.GetMin(), 0);
    EXPECT_EQ(copy.GetMax(), 1);
    EXPECT_FALSE(copy.IsValidType(""));
    EXPECT_TRUE(copy.IsValidType("type"));
    EXPECT_TRUE(tag_info == copy);
  }
}
