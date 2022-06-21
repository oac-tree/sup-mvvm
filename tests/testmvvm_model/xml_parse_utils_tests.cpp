/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/xml_parse_utils.h"

#include "test_utils.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing XML parse utility functions.

class XMLParseUtilsTests : public ::testing::Test
{
};

//! Parsing XML data string representing empty document.

TEST_F(XMLParseUtilsTests, ParseXMLDataString_EmptyString)
{
  EXPECT_THROW(mvvm::ParseXMLDataString(""), std::runtime_error);

  // empty xml file with default root node
  auto tree = mvvm::ParseXMLDataString(testutils::CreateXMLDocumentString(""));
  ASSERT_TRUE(tree.get() != nullptr);
  EXPECT_EQ(tree->GetType(), "root");
  EXPECT_EQ(tree->GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree->GetContent(), std::string());
  EXPECT_EQ(tree->GetNumberOfChildren(), 0);
}

//! Parsing XML data string representing a document with TagInfo description.

TEST_F(XMLParseUtilsTests, ParseXMLDataString_TagInfo)
{
  const std::string body{R"(
<TagInfo min="-1" name="TagName">model1, model2</TagInfo>
)"};

  auto tree = mvvm::ParseXMLDataString(testutils::CreateXMLDocumentString(body));
  ASSERT_TRUE(tree.get() != nullptr);
  ASSERT_EQ(tree->GetNumberOfChildren(), 1);

  auto child = tree->Children().at(0);
  EXPECT_EQ(child.GetType(), "TagInfo");
  EXPECT_EQ(child.GetContent(), std::string("model1, model2"));

  EXPECT_EQ(child.GetNumberOfAttributes(), 2);
  EXPECT_EQ(child.GetAttribute("min"), std::string("-1"));
  EXPECT_EQ(child.GetAttribute("name"), std::string("TagName"));
}

//! Parsing XML data string representing an element with TagInfo description.

TEST_F(XMLParseUtilsTests, ParseXMLElementString_TagInfo)
{
  const std::string body{R"(
<TagInfo min="-1" name="TagName">model1, model2</TagInfo>
)"};

  auto tree = mvvm::ParseXMLElementString(body);
  ASSERT_TRUE(tree.get() != nullptr);
  ASSERT_EQ(tree->GetNumberOfChildren(), 0);

  EXPECT_EQ(tree->GetType(), "TagInfo");
  EXPECT_EQ(tree->GetContent(), std::string("model1, model2"));

  EXPECT_EQ(tree->GetNumberOfAttributes(), 2);
  EXPECT_EQ(tree->GetAttribute("min"), std::string("-1"));
  EXPECT_EQ(tree->GetAttribute("name"), std::string("TagName"));
}

//! Parsing XML data string representing an element with variant/role description.

TEST_F(XMLParseUtilsTests, ParseXMLElementString_DataRole)
{
  const std::string body{R"(
<Variant role="0" type="int">42</Variant>
)"};

  auto tree = mvvm::ParseXMLElementString(body);
  ASSERT_TRUE(tree.get() != nullptr);
  ASSERT_EQ(tree->GetNumberOfChildren(), 0);

  EXPECT_EQ(tree->GetType(), "Variant");
  EXPECT_EQ(tree->GetContent(), std::string("42"));

  EXPECT_EQ(tree->GetNumberOfAttributes(), 2);
  EXPECT_EQ(tree->GetAttribute("role"), std::string("0"));
  EXPECT_EQ(tree->GetAttribute("type"), std::string("int"));
}
