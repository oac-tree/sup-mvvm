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

#include "mvvm/serialization/tree_data_item_converter.h"

#include <mvvm/model/compound_item.h>
#include <mvvm/model/external_property.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/session_item_data.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/standarditems/vector_item.h>

#include <sup/xml/tree_data_parser.h>
#include <sup/xml/tree_data_serialize.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace mvvm;

//! Testing TreeDataItemConverter.

class TreeDataItemConverterTest : public mvvm::test::FolderTest
{
public:
  TreeDataItemConverterTest() : FolderTest("TreeDataItemConverterTest") {}

  static std::unique_ptr<TreeDataItemConverter> CreateCloneConverter()
  {
    return std::make_unique<TreeDataItemConverter>(GetFactory(), ConverterMode::kClone);
  }

  static std::unique_ptr<TreeDataItemConverter> CreateCopyConverter(
      const std::function<bool(const SessionItem&)>& filter_func = {})
  {
    return std::make_unique<TreeDataItemConverter>(GetFactory(), ConverterMode::kCopy, filter_func);
  }

  void WriteToXMLFile(const std::string& file_name, const SessionItem& item) const
  {
    auto converter = CreateCloneConverter();
    auto tree_data = converter->ToTreeData(item);

    mvvm::xml::TreeDataToFile(file_name, *tree_data);
  }

  template <typename T>
  std::unique_ptr<T> ReadFromXMLFile(const std::string& file_name)
  {
    auto tree_data = mvvm::xml::TreeDataFromFile(file_name);
    auto converter = CreateCloneConverter();
    auto result = converter->ToSessionItem(*tree_data);
    return std::unique_ptr<T>(static_cast<T*>(result.release()));
  }

  static IItemFactory* GetFactory() { return &GetGlobalItemFactory(); }
};

//! Default PropertyItem to TreeData and back.

TEST_F(TreeDataItemConverterTest, PropertyItemToTreeDataAndBack)
{
  auto converter = CreateCloneConverter();

  PropertyItem item;

  // to TreeData
  auto tree_data = converter->ToTreeData(item);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // Reconstructing back
  auto reco = converter->ToSessionItem(*tree_data);
  EXPECT_EQ(reco->GetType(), item.GetType());
  EXPECT_EQ(reco->GetDisplayName(), item.GetDisplayName());
  EXPECT_EQ(reco->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(reco->GetItemData()->GetRoles(), item.GetItemData()->GetRoles());
}

//! PropertyItem with data to TreeData and back.

TEST_F(TreeDataItemConverterTest, PropertyItemWithDataToTreeDataAndBack)
{
  PropertyItem item;
  item.SetData(42, DataRole::kData);
  item.SetData("width", DataRole::kDisplay);
  item.SetData("Width in nm", DataRole::kTooltip);
  const int custom_role = 99;
  item.SetData(std::vector<double>({1.0, 2.0, 3.0}), custom_role);

  // to TreeData
  auto converter = CreateCloneConverter();
  auto tree_data = converter->ToTreeData(item);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // reconstructiong back
  auto reco = converter->ToSessionItem(*tree_data);
  EXPECT_EQ(reco->GetType(), item.GetType());
  EXPECT_EQ(reco->GetDisplayName(), item.GetDisplayName());
  EXPECT_EQ(reco->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(reco->GetItemData()->GetRoles(), item.GetItemData()->GetRoles());
  EXPECT_EQ(reco->Data(DataRole::kData), variant_t(42));
  EXPECT_EQ(reco->Data(DataRole::kDisplay), variant_t(std::string("width")));
  EXPECT_EQ(reco->Data(DataRole::kTooltip), variant_t(std::string("Width in nm")));
  EXPECT_EQ(reco->Data(custom_role), variant_t(std::vector<double>({1.0, 2.0, 3.0})));
}

//! PropertyItem with data to TreeData and back.

TEST_F(TreeDataItemConverterTest, PropertyItemWithDataToFileAndBack)
{
  PropertyItem item;
  item.SetData(42, DataRole::kData);
  item.SetData("width", DataRole::kDisplay);
  item.SetData("Width in nm", DataRole::kTooltip);
  const int custom_role1 = 99;
  item.SetData(std::vector<double>({1.0, 2.0, 3.0}), custom_role1);
  const int custom_role2 = 100;
  item.SetData(ComboProperty({"a1", "abc edf", "a3"}, "abc edf"), custom_role2);
  const int custom_role3 = 101;
  item.SetData(ExternalProperty("text", "color", "identifier"), custom_role3);
  SetLimited(11, 12, item);

  const auto file_path = GetFilePath("PropertyItemWithDataToFileAndBack.xml");
  WriteToXMLFile(file_path, item);

  // reconstructiong back
  auto reco = ReadFromXMLFile<PropertyItem>(file_path);
  EXPECT_EQ(reco->GetType(), item.GetType());
  EXPECT_EQ(reco->GetDisplayName(), item.GetDisplayName());
  EXPECT_EQ(reco->GetIdentifier(), item.GetIdentifier());
  EXPECT_EQ(reco->GetItemData()->GetRoles(), item.GetItemData()->GetRoles());
  EXPECT_EQ(reco->Data(DataRole::kData), variant_t(42));
  EXPECT_EQ(reco->Data(DataRole::kDisplay), variant_t(std::string("width")));
  EXPECT_EQ(reco->Data(DataRole::kTooltip), variant_t(std::string("Width in nm")));
  EXPECT_EQ(reco->Data(custom_role1), variant_t(std::vector<double>({1.0, 2.0, 3.0})));
  EXPECT_EQ(reco->Data(custom_role2), variant_t(ComboProperty({"a1", "abc edf", "a3"}, "abc edf")));
  EXPECT_EQ(reco->Data(custom_role3), variant_t(ExternalProperty("text", "color", "identifier")));
  EXPECT_EQ(reco->Data(DataRole::kLowerLimit), variant_t(11));
  EXPECT_EQ(reco->Data(DataRole::kUpperLimit), variant_t(12));
}

//! Parent and child to TreeData object and back.

TEST_F(TreeDataItemConverterTest, ParentAndChildToTreeDataAndBack)
{
  SessionItem parent;
  parent.SetDisplayName("parent_name");
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // to TreeData
  auto converter = CreateCloneConverter();
  auto tree_data = converter->ToTreeData(parent);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // reconstructiong back
  auto reco_parent = converter->ToSessionItem(*tree_data);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

//! Char8 to TreeData object and back.

TEST_F(TreeDataItemConverterTest, Char8ToFileAndBack)
{
  SessionItem item;
  item.SetData(mvvm::char8{0});
  item.SetData(mvvm::char8{'A'}, 42);

  const auto file_path = GetFilePath("Char8ToFileAndBack.xml");
  WriteToXMLFile(file_path, item);

  // reconstructiong back
  auto reco_item = ReadFromXMLFile<SessionItem>(file_path);

  // checking parent reconstruction
  EXPECT_EQ(reco_item->Data<char8>(), '\0');
  EXPECT_EQ(reco_item->Data<char8>(42), 'A');
}

//! Parent and child to TreeData object and back.

TEST_F(TreeDataItemConverterTest, ParentAndChildToFileAndBack)
{
  SessionItem parent;
  parent.SetDisplayName("parent_name");
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  const auto file_path = GetFilePath("ParentAndChildToFileAndBack.xml");
  WriteToXMLFile(file_path, parent);

  // reconstructiong back
  auto reco_parent = ReadFromXMLFile<SessionItem>(file_path);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

//! Parent and child to TreeData object and back.

TEST_F(TreeDataItemConverterTest, CompoundItemToTreeDataAndBack)
{
  CompoundItem parent;
  parent.SetDisplayName("parent_name");
  auto& position_item = parent.AddProperty<VectorItem>("Position");
  position_item.SetXYZ(1.0, 2.0, 3.0);

  // to TreeData
  auto converter = CreateCloneConverter();
  auto tree_data = converter->ToTreeData(parent);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // reconstructiong back
  auto reco_parent = converter->ToSessionItem(*tree_data);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), CompoundItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem<VectorItem>("Position");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 3);
  EXPECT_EQ(reco_child->GetType(), VectorItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "Position");
  EXPECT_EQ(reco_child->GetIdentifier(), position_item.GetIdentifier());
  EXPECT_EQ(reco_child->X(), position_item.X());
  EXPECT_EQ(reco_child->Y(), position_item.Y());
  EXPECT_EQ(reco_child->Z(), position_item.Z());
}

//! Parent and child to TreeData object and back.

TEST_F(TreeDataItemConverterTest, CompoundItemFileAndBack)
{
  CompoundItem parent;
  parent.SetDisplayName("parent_name");
  auto& position_item = parent.AddProperty<VectorItem>("Position");
  position_item.SetXYZ(1.0, 2.0, 3.0);

  const auto file_path = GetFilePath("CompoundItemFileAndBack.xml");
  WriteToXMLFile(file_path, parent);

  // reconstructiong back
  auto reco_parent = ReadFromXMLFile<SessionItem>(file_path);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), CompoundItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem<VectorItem>("Position");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 3);
  EXPECT_EQ(reco_child->GetType(), VectorItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "Position");
  EXPECT_EQ(reco_child->GetIdentifier(), position_item.GetIdentifier());
  EXPECT_EQ(reco_child->X(), position_item.X());
  EXPECT_EQ(reco_child->Y(), position_item.Y());
  EXPECT_EQ(reco_child->Z(), position_item.Z());
}

//! Parent and child to TreeData object and back (converter in copy mode).
TEST_F(TreeDataItemConverterTest, ParentAndChildCopy)
{
  SessionItem parent;
  parent.SetDisplayName("parent_name");
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child->SetDisplayName("child_name");

  // to TreeData
  auto converter = CreateCopyConverter();
  auto tree_data = converter->ToTreeData(parent);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // reconstructiong back
  auto reco_parent = converter->ToSessionItem(*tree_data);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_NE(reco_parent->GetIdentifier(), parent.GetIdentifier());  // regenerated identifiers
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_NE(reco_child->GetIdentifier(), child->GetIdentifier());  // regenerated identifiers
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

TEST_F(TreeDataItemConverterTest, ParentAndChildCopyWithFilter)
{
  SessionItem parent;
  parent.SetDisplayName("parent_name");
  parent.RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child0 = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child0->SetDisplayName("child_name0");
  auto child1 = parent.InsertItem(std::make_unique<PropertyItem>(), TagIndex::Append());
  child1->SetDisplayName("child_name1");

  auto filter_func = [](const auto& item) -> bool
  { return item.GetDisplayName() != "child_name0"; };

  // to TreeData
  auto converter = CreateCopyConverter(filter_func);
  auto tree_data = converter->ToTreeData(parent);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // reconstructiong back
  auto reco_parent = converter->ToSessionItem(*tree_data);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::GetStaticType());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_NE(reco_parent->GetIdentifier(), parent.GetIdentifier());  // regenerated identifiers
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::GetStaticType());
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name1");
  EXPECT_NE(reco_child->GetIdentifier(), child1->GetIdentifier());  // regenerated identifiers
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}
