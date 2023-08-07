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

#include "mvvm/serialization/treedata_model_converter.h"

#include <mvvm/model/item_manager.h>
#include <mvvm/model/item_pool.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/serialization/treedata.h>
#include <mvvm/serialization/xml_write_utils.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace mvvm;

//! Testing TreeDataModelConverter.

class TreeDataModelConverterTests : public ::testing::Test
{
};

//! EmptyModel to TreeData.

TEST_F(TreeDataModelConverterTests, EmptyModel)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  auto tree_data = converter.ToTreeData(model);
  EXPECT_TRUE(converter.IsSessionModelConvertible(*tree_data));
}

//! Empty model to TreeData and back.

TEST_F(TreeDataModelConverterTests, EmptyModelToTreeDataAndBack)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  auto tree_data = converter.ToTreeData(model);

  // attempt to reconstruct model of different type.
  SessionModel target1("NewModel");
  EXPECT_THROW(converter.PopulateSessionModel(*tree_data, target1), std::runtime_error);

  // succesfull reconstruction
  SessionModel target2("TestModel");
  EXPECT_NO_THROW(converter.PopulateSessionModel(*tree_data, target2));
  EXPECT_EQ(target2.GetRootItem()->GetTotalItemCount(), 0);
}

//! Creation of TreeData object: single item in a model.

TEST_F(TreeDataModelConverterTests, SingleItemToTreeDataAndBack)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  auto item = model.InsertItem<SessionItem>();

  auto tree_data = converter.ToTreeData(model);

  // reconstructing model
  SessionModel target("TestModel");
  converter.PopulateSessionModel(*tree_data, target);

  EXPECT_EQ(target.GetRootItem()->GetTotalItemCount(), 1);
  auto reco_item = target.GetRootItem()->GetItem(TagIndex::Prepend());
  EXPECT_EQ(reco_item->GetParent(), target.GetRootItem());
  EXPECT_EQ(reco_item->GetType(), item->GetType());
  EXPECT_EQ(reco_item->GetIdentifier(), item->GetIdentifier());
}

//! Filling model from TreeData: parent and child in a model to TreeData and back.

TEST_F(TreeDataModelConverterTests, ParentAndChildToTreeDataAndBack)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  // filling original model with content
  auto parent = model.InsertItem<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  parent->SetData(42);
  auto child = model.InsertItem<PropertyItem>(parent);
  child->SetDisplayName("child_name");

  // writing model to TreeData
  auto tree_data = converter.ToTreeData(model);

  // reconstructing the model
  SessionModel target("TestModel");
  converter.PopulateSessionModel(*tree_data, target);

  // accessing reconstructed parent and child
  auto reco_parent = target.GetRootItem()->GetItem(TagIndex::Prepend());
  auto reco_child = reco_parent->GetItem(TagIndex::Prepend());

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetModel(), &target);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->GetParent(), target.GetRootItem());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetIdentifier(), parent->GetIdentifier());
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->Data<int>(), 42);

  // checking child reconstruction
  EXPECT_EQ(reco_child->GetModel(), &target);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->GetParent(), reco_parent);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

//! Item in a model to TreeData and back: how persistent are identifiers.

TEST_F(TreeDataModelConverterTests, IdentifierPersistence)
{
  TreeDataModelConverter converter(ConverterMode::kClone);

  // creating source model with own pool for item registration
  auto pool1 = std::make_shared<ItemPool>();
  SessionModel source("SourceModel", CreateDefaultItemManager(pool1));
  auto parent1 = source.InsertItem<SessionItem>();

  // writing model to TreeData
  auto tree_data = converter.ToTreeData(source);

  // creating target with own pool for item registration
  auto pool2 = std::make_shared<ItemPool>();
  SessionModel target("SourceModel", CreateDefaultItemManager(pool2));
  converter.PopulateSessionModel(*tree_data, target);

  auto reco_parent = target.GetRootItem()->GetItem(TagIndex::Prepend());

  // comparing identifiers of two items from different models
  auto id1 = parent1->GetIdentifier();
  auto id2 = reco_parent->GetIdentifier();
  EXPECT_EQ(id1, id2);

  // checking item registrations
  EXPECT_EQ(pool1->ItemForKey(id1), parent1);
  EXPECT_EQ(pool2->ItemForKey(id2), reco_parent);

  // finaly comparing XML strings
  auto tree_data_target = converter.ToTreeData(source);
  EXPECT_EQ(GetXMLString(*tree_data), GetXMLString(*tree_data_target));
}

//! Creation of TreeData object (single item in a model), then writing same TreeData object back
//! to model without emptying it. Real bug case: check if unsubscribtion mechanism works.

TEST_F(TreeDataModelConverterTests, SingleItemToTreeDataAndBackToSameModel)
{
  TreeDataModelConverter converter(ConverterMode::kClone);

  auto pool = std::make_shared<ItemPool>();
  SessionModel model("TestModel", CreateDefaultItemManager(pool));
  auto item = model.InsertItem<SessionItem>();

  auto root_item = model.GetRootItem();
  auto root_id = root_item->GetIdentifier();
  auto item_id = item->GetIdentifier();

  // writing model to TreeData
  auto tree_data = converter.ToTreeData(model);

  // filling new model
  converter.PopulateSessionModel(*tree_data, model);

  EXPECT_EQ(pool->GetSize(), 2);
  EXPECT_FALSE(pool->ItemForKey(root_id) == model.GetRootItem());  // old root identifier has gone
  EXPECT_TRUE(model.GetRootItem() != root_item);                   // old root item gone

  auto new_item = model.GetRootItem()->GetAllItems().at(0);
  EXPECT_EQ(pool->ItemForKey(item_id), new_item);
}
