// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/serialization/xmlwriteutils.h"

#include "folderbasedtest.h"
#include "test_utils.h"

#include "mvvm/serialization/TreeData.h"

using namespace ModelView;

//! Testing utilities to ite XML files

class XMLWriteUtilsTest : public FolderBasedTest
{
public:
  XMLWriteUtilsTest() : FolderBasedTest("test_XMLWriteUtils") {}
};

//! Write a single element.

TEST_F(XMLWriteUtilsTest, WriteElement)
{
  ModelView::TreeData tree_data("Element");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element/>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElement.xml");
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute.

TEST_F(XMLWriteUtilsTest, WriteElementWithAttribute)
{
  ModelView::TreeData tree_data("Element");
  tree_data.AddAttribute("name", "James");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element name="James"/>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElementWithAttribute.xml");
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute.

TEST_F(XMLWriteUtilsTest, WriteElementWithTwoAttributes)
{
  ModelView::TreeData tree_data("Element");
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
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}

//! Write a single element with content defined.

TEST_F(XMLWriteUtilsTest, WriteElementContent)
{
  ModelView::TreeData tree_data("Element");
  tree_data.SetContent("1.0, 2.0, 3.0");
  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<Element>1.0, 2.0, 3.0</Element>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteElementContent.xml");
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute and content defined.

TEST_F(XMLWriteUtilsTest, WriteElementAttributeAndContent)
{
  ModelView::TreeData tree_data("Element");
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
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute and content defined.

TEST_F(XMLWriteUtilsTest, WriteElementWithChildren)
{
  ModelView::TreeData tree_data("Element");
  tree_data.AddChild(ModelView::TreeData("Child"));
  tree_data.AddChild(ModelView::TreeData("Child"));
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
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}

//! Write a single element with attribute and content defined.

TEST_F(XMLWriteUtilsTest, WriteItemData)
{
  ModelView::TreeData tree_data("ItemData");
  ModelView::TreeData variant1("Variant");
  variant1.AddAttribute("type", "int");
  variant1.AddAttribute("role", "0");
  variant1.SetContent("42");
  ModelView::TreeData variant2("Variant");
  variant2.AddAttribute("type", "vector_double_t");
  variant2.AddAttribute("role", "0");
  variant2.SetContent("42.1, 42.2, 42.3");
  tree_data.AddChild(variant1);
  tree_data.AddChild(variant2);

  const std::string expected{R"(<?xml version="1.0" encoding="UTF-8"?>
<ItemData>
  <Variant role="0" type="int">42</Variant>
  <Variant role="0" type="vector_double_t">42.1, 42.2, 42.3</Variant>
</ItemData>
)"};

  // writing to string and checking content
  auto str = GetXMLString(tree_data);
  EXPECT_EQ(GetXMLString(tree_data), expected);

  // writing to file and checking its content
  const auto file_path = GetFilePath("WriteItemData.xml");
  ModelView::WriteToXMLFile(file_path, tree_data);
  EXPECT_EQ(TestUtils::GetTextFileContent(file_path), expected);
}
