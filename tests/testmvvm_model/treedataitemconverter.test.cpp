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

#include "mvvm/serialization/treedataitemconverter.h"

#include "folderbasedtest.h"
#include "test_utils.h"

#include "mvvm/factories/itemcataloguefactory.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/model/externalproperty.h"
#include "mvvm/model/itemfactory.h"
#include "mvvm/model/sessionitemdata.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/xmlparseutils.h"
#include "mvvm/serialization/xmlwriteutils.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/standarditems/vectoritem.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace mvvm;

//! Testing TreeDataItemConverter.

class TreeDataItemConverterTest : public FolderBasedTest
{
public:
  TreeDataItemConverterTest()
      : FolderBasedTest("test_TreeDataItemConverter")
      , m_factory(mvvm::CreateStandardItemCatalogue()){};

  std::unique_ptr<TreeDataItemConverter> CreateCloneConverter() const
  {
    return std::make_unique<TreeDataItemConverter>(&m_factory, ConverterMode::kClone);
  }

  std::unique_ptr<TreeDataItemConverter> CreateCopyConverter() const
  {
    return std::make_unique<TreeDataItemConverter>(&m_factory, ConverterMode::kCopy);
  }

  void WriteToXMLFile(const std::string& file_name, const SessionItem& item) const
  {
    auto converter = CreateCloneConverter();
    auto tree_data = converter->ToTreeData(item);

    ::mvvm::WriteToXMLFile(file_name, *tree_data);
  }

  template <typename T>
  std::unique_ptr<T> ReadFromXMLFile(const std::string& file_name)
  {
    auto tree_data = ParseXMLDataFile(file_name);
    auto converter = CreateCloneConverter();
    auto result = converter->ToSessionItem(*tree_data);
    return std::unique_ptr<T>(static_cast<T*>(result.release()));
  }

private:
  mvvm::ItemFactory m_factory;
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
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
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
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

//! Parent and child to TreeData object and back.

TEST_F(TreeDataItemConverterTest, CompoundItemToTreeDataAndBack)
{
  CompoundItem parent;
  parent.SetDisplayName("parent_name");
  auto position_item = parent.AddProperty<VectorItem>("Position");
  position_item->SetXYZ(1.0, 2.0, 3.0);

  // to TreeData
  auto converter = CreateCloneConverter();
  auto tree_data = converter->ToTreeData(parent);
  EXPECT_TRUE(converter->IsSessionItemConvertible(*tree_data));

  // reconstructiong back
  auto reco_parent = converter->ToSessionItem(*tree_data);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), CompoundItem::Type);
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem<VectorItem>("Position");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 3);
  EXPECT_EQ(reco_child->GetType(), VectorItem::Type);
  EXPECT_EQ(reco_child->GetDisplayName(), "Position");
  EXPECT_EQ(reco_child->GetIdentifier(), position_item->GetIdentifier());
  EXPECT_EQ(reco_child->X(), position_item->X());
  EXPECT_EQ(reco_child->Y(), position_item->Y());
  EXPECT_EQ(reco_child->Z(), position_item->Z());
}

//! Parent and child to TreeData object and back.

TEST_F(TreeDataItemConverterTest, CompoundItemFileAndBack)
{
  CompoundItem parent;
  parent.SetDisplayName("parent_name");
  auto position_item = parent.AddProperty<VectorItem>("Position");
  position_item->SetXYZ(1.0, 2.0, 3.0);

  const auto file_path = GetFilePath("CompoundItemFileAndBack.xml");
  WriteToXMLFile(file_path, parent);

  // reconstructiong back
  auto reco_parent = ReadFromXMLFile<SessionItem>(file_path);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetType(), CompoundItem::Type);
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetIdentifier(), parent.GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem<VectorItem>("Position");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 3);
  EXPECT_EQ(reco_child->GetType(), VectorItem::Type);
  EXPECT_EQ(reco_child->GetDisplayName(), "Position");
  EXPECT_EQ(reco_child->GetIdentifier(), position_item->GetIdentifier());
  EXPECT_EQ(reco_child->X(), position_item->X());
  EXPECT_EQ(reco_child->Y(), position_item->Y());
  EXPECT_EQ(reco_child->Z(), position_item->Z());
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
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_NE(reco_parent->GetIdentifier(), parent.GetIdentifier());  // regenerated identifiers
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->GetModel(), nullptr);

  // checking child reconstruction
  auto reco_child = reco_parent->GetItem("defaultTag");
  EXPECT_EQ(reco_child->GetParent(), reco_parent.get());
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_NE(reco_child->GetIdentifier(), child->GetIdentifier());  // regenerated identifiers
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

// Restore unit test after implementing filtered serialization of SessionItemData
// TEST_F(JsonItemConverterTest, testItemToFileAndBack)
//{
//  auto converter = createConverter();

//  TestItem item;
//  auto object = converter->to_json(&item);

//  // saving object to file
//  auto fileName = TestUtils::TestFileName(testDir(), "testItemToFileAndBack.json");
//  TestUtils::SaveJson(object, fileName);

//  auto document = TestUtils::LoadJson(fileName);
//  auto reco = converter->from_json(document.object());

//  EXPECT_EQ(reco->parent(), nullptr);
//  EXPECT_EQ(reco->modelType(), item.modelType());
//  EXPECT_EQ(reco->displayName(), item.displayName());
//  EXPECT_EQ(reco->identifier(), item.identifier());

//  EXPECT_EQ(reco->toolTip(), "compound");
//  // tooltip was preserved after the serialization
//  EXPECT_EQ(reco->getItem("Thickness")->toolTip(), "thickness");
//}
