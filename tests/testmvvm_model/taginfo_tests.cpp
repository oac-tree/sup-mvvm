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

#include "mvvm/model/taginfo.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests of TagInfo class.

class TagInfoTests : public ::testing::Test
{
};

TEST_F(TagInfoTests, InitialState)
{
  TagInfo tag;
  EXPECT_EQ(tag.GetName(), std::string());
  EXPECT_EQ(tag.GetMin(), 0);
  EXPECT_EQ(tag.GetMax(), -1);
  EXPECT_FALSE(tag.IsSinglePropertyTag());
  EXPECT_TRUE(tag.IsValidType(""));
  EXPECT_TRUE(tag.IsValidType("abc"));
}

//! Testing default tag intended for storing unlimited amount of items of any type.

TEST_F(TagInfoTests, DefaultTag)
{
  // initial state
  TagInfo tag = TagInfo::CreateUniversalTag("name");
  EXPECT_EQ(tag.GetName(), std::string("name"));
  EXPECT_EQ(tag.GetMin(), 0);
  EXPECT_EQ(tag.GetMax(), -1);
  EXPECT_FALSE(tag.IsSinglePropertyTag());
  EXPECT_TRUE(tag.IsValidType(""));
  EXPECT_TRUE(tag.IsValidType("abc"));
}

//! Testing property tag intended for storing single PropertyItem.

TEST_F(TagInfoTests, PropertyTag)
{
  // initial state
  TagInfo tag = TagInfo::CreatePropertyTag("name", "model_type");

  EXPECT_EQ(tag.GetName(), std::string("name"));
  EXPECT_EQ(tag.GetMin(), 1);
  EXPECT_EQ(tag.GetMax(), 1);
  EXPECT_TRUE(tag.IsSinglePropertyTag());
  EXPECT_TRUE(tag.IsValidType("model_type"));
  EXPECT_FALSE(tag.IsValidType("abc"));
}

//! Testing equality operators.

TEST_F(TagInfoTests, EqualityOperator)
{
  // default constructor
  TagInfo tag1;
  TagInfo tag2;
  EXPECT_TRUE(tag1 == tag2);
  EXPECT_FALSE(tag1 != tag2);

  // same property tag
  TagInfo tag3 = TagInfo::CreatePropertyTag("name", "model_type");
  TagInfo tag4 = TagInfo::CreatePropertyTag("name", "model_type");
  EXPECT_TRUE(tag3 == tag4);
  EXPECT_FALSE(tag3 != tag4);

  // same universal tag
  TagInfo tag5 = TagInfo::CreateUniversalTag("name");
  TagInfo tag6 = TagInfo::CreateUniversalTag("name");
  EXPECT_TRUE(tag5 == tag6);
  EXPECT_FALSE(tag5 != tag6);

  // different tag
  TagInfo tag7("tag7", 0, 1, std::vector<std::string>());
  TagInfo tag8("tag8", 0, 1, std::vector<std::string>());
  EXPECT_FALSE(tag7 == tag8);
  EXPECT_TRUE(tag7 != tag8);
}
