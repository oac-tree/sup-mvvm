/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/serialization/treedatamodelconverter.h"

#include "test_utils.h"

#include "mvvm/model/itempool.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/TreeData.h"
#include "mvvm/serialization/treedatamodelconverter.h"
#include "mvvm/serialization/xmlwriteutils.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Testing TreeDataModelConverter.

class TreeDataModelConverterTest : public ::testing::Test
{
};

//! EmptyModel to TreeData.

TEST_F(TreeDataModelConverterTest, EmptyModel)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  auto tree_data = converter.ToTreeData(model);
  EXPECT_TRUE(converter.IsSessionModelConvertible(*tree_data));
}

//! Empty model to TreeData and back.

TEST_F(TreeDataModelConverterTest, EmptyModelToJsonAndBack)
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
  EXPECT_EQ(target2.rootItem()->childrenCount(), 0);
}

//! Creation of TreeData object: single item in a model.

TEST_F(TreeDataModelConverterTest, SingleItemToTreeDataAndBack)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  auto item = model.insertItem<SessionItem>();

  auto tree_data = converter.ToTreeData(model);

  // reconstructing model
  SessionModel target("TestModel");
  converter.PopulateSessionModel(*tree_data, target);

  EXPECT_EQ(target.rootItem()->childrenCount(), 1);
  auto reco_item = target.rootItem()->getItem("", 0);
  EXPECT_EQ(reco_item->parent(), target.rootItem());
  EXPECT_EQ(reco_item->GetType(), item->GetType());
  EXPECT_EQ(reco_item->GetIdentifier(), item->GetIdentifier());
}

//! Filling model from json: parent and child in a model to json and back.

TEST_F(TreeDataModelConverterTest, ParentAndChildToTreeDataAndBack)
{
  TreeDataModelConverter converter(ConverterMode::kClone);
  SessionModel model("TestModel");

  // filling original model with content
  auto parent = model.insertItem<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  parent->setData(42);
  auto child = model.insertItem<PropertyItem>(parent);
  child->SetDisplayName("child_name");

  // writing model to TreeData
  auto tree_data = converter.ToTreeData(model);

  // reconstructing the model
  SessionModel target("TestModel");
  converter.PopulateSessionModel(*tree_data, target);

  // accessing reconstructed parent and child
  auto reco_parent = target.rootItem()->getItem("", 0);
  auto reco_child = reco_parent->getItem("", 0);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->model(), &target);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->parent(), target.rootItem());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->childrenCount(), 1);
  EXPECT_EQ(reco_parent->GetIdentifier(), parent->GetIdentifier());
  EXPECT_EQ(reco_parent->itemTags()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->data<int>(), 42);

  // checking child reconstruction
  EXPECT_EQ(reco_child->model(), &target);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->parent(), reco_parent);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->childrenCount(), 0);
  EXPECT_EQ(reco_child->GetIdentifier(), child->GetIdentifier());
  EXPECT_EQ(reco_child->itemTags()->GetDefaultTag(), "");
}

//! Item in a model to TreeData and back: how persistent are identifiers.

TEST_F(TreeDataModelConverterTest, IdentifierPersistence)
{
  TreeDataModelConverter converter(ConverterMode::kClone);

  // creating source model with own pool for item registration
  auto pool1 = std::make_shared<ItemPool>();
  SessionModel source("SourceModel", pool1);
  auto parent1 = source.insertItem<SessionItem>();

  // writing model to TreeData
  auto tree_data = converter.ToTreeData(source);

  // creating target with own pool for item registration
  auto pool2 = std::make_shared<ItemPool>();
  SessionModel target("SourceModel", pool2);
  converter.PopulateSessionModel(*tree_data, target);

  auto reco_parent = target.rootItem()->getItem("", 0);

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

//! Creation of json object (single item in a model), then writing same json object back
//! to model without emptying it. Real bug case: check if unsubscribtion mechanism works.

TEST_F(TreeDataModelConverterTest, SingleItemToJsonAndBackToSameModel)
{
  TreeDataModelConverter converter(ConverterMode::kClone);

  auto pool = std::make_shared<ItemPool>();
  SessionModel model("TestModel", pool);
  auto item = model.insertItem<SessionItem>();

  auto root_item = model.rootItem();
  auto root_id = root_item->GetIdentifier();
  auto item_id = item->GetIdentifier();

  // writing model to TreeData
  auto tree_data = converter.ToTreeData(model);

  // filling new model
  converter.PopulateSessionModel(*tree_data, model);

  EXPECT_EQ(pool->size(), 2);
  EXPECT_FALSE(pool->ItemForKey(root_id) == model.rootItem());  // old root identifier has gone
  EXPECT_TRUE(model.rootItem() != root_item);                   // old root item gone

  auto new_item = model.rootItem()->children().at(0);
  EXPECT_EQ(pool->ItemForKey(item_id), new_item);
}
