/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/xml_write_utils.h"

#include <mvvm/serialization/treedata.h>
#include <mvvm/test/test_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace mvvm;

//! Testing utilities to ite XML files

class XMLWriteUtilsTest : public mvvm::test::FolderTest
{
public:
  XMLWriteUtilsTest() : FolderTest("XMLWriteUtilsTest") {}
};

//! Write a single element.

TEST_F(XMLWriteUtilsTest, WriteElement)
{
  mvvm::TreeData tree_data("Element");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element/>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElement.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute.

TEST_F(XMLWriteUtilsTest, WriteElementWithAttribute)
{
  mvvm::TreeData tree_data("Element");
  tree_data.AddAttribute("name", "James");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element name="James"/>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElementWithAttribute.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute.

TEST_F(XMLWriteUtilsTest, WriteElementWithTwoAttributes)
{
  mvvm::TreeData tree_data("Element");
  tree_data.AddAttribute("age", "33");
  tree_data.AddAttribute("name", "James");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element age="33" name="James"/>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElementWithTwoAttributes.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}

//! Write a single element with content defined.

TEST_F(XMLWriteUtilsTest, WriteElementContent)
{
  mvvm::TreeData tree_data("Element");
  tree_data.SetContent("1.0, 2.0, 3.0");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element>1.0, 2.0, 3.0</Element>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElementContent.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute and content defined.

TEST_F(XMLWriteUtilsTest, WriteElementAttributeAndContent)
{
  mvvm::TreeData tree_data("Element");
  tree_data.AddAttribute("name", "James");
  tree_data.SetContent("1.0, 2.0, 3.0");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element name="James">1.0, 2.0, 3.0</Element>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("element_with_content_and_attribute.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute and content defined.

TEST_F(XMLWriteUtilsTest, WriteElementWithChildren)
{
  mvvm::TreeData tree_data("Element");
  tree_data.AddChild(mvvm::TreeData("Child"));
  tree_data.AddChild(mvvm::TreeData("Child"));
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element>
  <Child/>
  <Child/>
</Element>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElementWithChildren.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute and content defined.

TEST_F(XMLWriteUtilsTest, WriteItemData)
{
  mvvm::TreeData tree_data("ItemData");
  mvvm::TreeData variant1("Variant");
  variant1.AddAttribute("type", "int");
  variant1.AddAttribute("role", "0");
  variant1.SetContent("42");
  mvvm::TreeData variant2("Variant");
  variant2.AddAttribute("type", "vector_double");
  variant2.AddAttribute("role", "0");
  variant2.SetContent("42.1, 42.2, 42.3");
  tree_data.AddChild(variant1);
  tree_data.AddChild(variant2);

  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<ItemData>
  <Variant role="0" type="int">42</Variant>
  <Variant role="0" type="vector_double">42.1, 42.2, 42.3</Variant>
</ItemData>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteItemData.xml");
  mvvm::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(mvvm::test::GetTextFileContent(file_path), expected);
}
