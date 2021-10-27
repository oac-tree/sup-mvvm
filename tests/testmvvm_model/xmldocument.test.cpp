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

#include "mvvm/serialization/xmldocument.h"

#include "folderbasedtest.h"
#include "test_utils.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/serialization/xmldocument.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace ModelView;

//! Testing XmlDocument.

class XmlDocumentTest : public FolderBasedTest
{
public:
  XmlDocumentTest() : FolderBasedTest("test_XmlDocument") {}

  class TestModel1 : public SessionModel
  {
  public:
    TestModel1() : SessionModel("TestModel1") {}
  };

  class TestModel2 : public SessionModel
  {
  public:
    TestModel2() : SessionModel("TestModel2") {}
  };
};

//! Saving the model with content into document and restoring it after.

TEST_F(XmlDocumentTest, SaveLoadEmptyModel)
{
  const auto file_path = GetFilePath("SaveLoadEmptyModel.xml");

  SessionModel model("TestModel");
  XmlDocument document({&model});

  auto original_root = model.GetRootItem();
  auto original_identifier = original_root->GetIdentifier();

  // saving model in file
  document.Save(file_path);

  // loading model from file
  document.Load(file_path);

  EXPECT_NE(model.GetRootItem(), original_root);
  EXPECT_NE(model.GetRootItem()->GetIdentifier(), original_identifier);
}

//! Saving the model with content into document and restoring it after.

TEST_F(XmlDocumentTest, SaveLoadModelWithPropertyItem)
{
  const auto file_path = GetFilePath("SaveLoadModelWithPropertyItem.xml");

  SessionModel model("TestModel");
  model.InsertItem<PropertyItem>();
  XmlDocument document({&model});

  auto original_root = model.GetRootItem();
  auto original_identifier = original_root->GetIdentifier();

  // saving model in file
  document.Save(file_path);

  // loading model from file
  document.Load(file_path);

  EXPECT_NE(model.GetRootItem(), original_root);
  EXPECT_NE(model.GetRootItem()->GetIdentifier(), original_identifier);
}

//! Saving the model with content into document and restoring it after.

TEST_F(XmlDocumentTest, SaveLoadModelWithParentAndChild)
{
  const auto file_path = GetFilePath("SaveLoadModelWithParentAndChild.xml");

  SessionModel model("TestModel");
  XmlDocument document({&model});

  // filling model with parent and child
  auto parent = model.InsertItem<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  const auto parent_identifier = parent->GetIdentifier();

  parent->SetData(42);
  auto child = model.InsertItem<PropertyItem>(parent);
  child->SetDisplayName("child_name");
  const auto child_identifier = child->GetIdentifier();

  // saving model in file
  document.Save(file_path);

  // modifying model further
  model.RemoveItem(model.GetRootItem(), {"", 0});

  // loading model from file
  document.Load(file_path);

  // checking that it is as it was right after the save

  // accessing reconstructed parent and child
  auto reco_parent = model.GetRootItem()->GetItem("", 0);
  auto reco_child = reco_parent->GetItem("", 0);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->GetModel(), &model);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->GetParent(), model.GetRootItem());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->GetTotalItemCount(), 1);
  EXPECT_EQ(reco_parent->GetIdentifier(), parent_identifier);
  EXPECT_EQ(reco_parent->GetTaggedItems()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->Data<int>(), 42);

  // checking child reconstruction
  EXPECT_EQ(reco_child->GetModel(), &model);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->GetParent(), reco_parent);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->GetTotalItemCount(), 0);
  EXPECT_EQ(reco_child->GetIdentifier(), child_identifier);
  EXPECT_EQ(reco_child->GetTaggedItems()->GetDefaultTag(), "");
}

//! Saving two models with content into document and restoring it after.

TEST_F(XmlDocumentTest, SaveLoadTwoModels)
{
  const auto file_path = GetFilePath("SaveLoadTwoModels.xml");

  TestModel1 model1;
  TestModel2 model2;
  XmlDocument document({&model1, &model2});

  // filling models
  auto parent1 = model1.InsertItem<SessionItem>();
  const auto parent_identifier1 = parent1->GetIdentifier();

  auto parent2 = model2.InsertItem<SessionItem>();
  const auto parent_identifier2 = parent2->GetIdentifier();

  // saving models in file
  document.Save(file_path);

  // modifying model further
  model1.RemoveItem(model1.GetRootItem(), {"", 0});
  model2.RemoveItem(model2.GetRootItem(), {"", 0});

  // loading model from file
  document.Load(file_path);

  // checking that it is as it was right after the save

  // accessing reconstructed parent and child
  auto reco_parent1 = model1.GetRootItem()->GetItem("", 0);
  auto reco_parent2 = model2.GetRootItem()->GetItem("", 0);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent1->GetModel(), &model1);
  EXPECT_EQ(reco_parent1->GetParent(), model1.GetRootItem());
  EXPECT_EQ(reco_parent1->GetIdentifier(), parent_identifier1);

  EXPECT_EQ(reco_parent2->GetModel(), &model2);
  EXPECT_EQ(reco_parent2->GetParent(), model2.GetRootItem());
  EXPECT_EQ(reco_parent2->GetIdentifier(), parent_identifier2);
}

//! Attempt to restore models in wrong order.

TEST_F(XmlDocumentTest, LoadModelsInWrongOrder)
{
  const auto file_path = GetFilePath("LoadModelsInWrongOrder.xml");
  TestModel1 model1;
  TestModel2 model2;

  // filling models
  auto parent1 = model1.InsertItem<SessionItem>();
  const auto parent_identifier1 = parent1->GetIdentifier();

  auto parent2 = model2.InsertItem<SessionItem>();
  const auto parent_identifier2 = parent2->GetIdentifier();

  // saving models in file
  {
    XmlDocument document({&model1, &model2});
    document.Save(file_path);
  }

  // modifying model further
  model1.RemoveItem(model1.GetRootItem(), {"", 0});
  model2.RemoveItem(model2.GetRootItem(), {"", 0});

  XmlDocument document({&model2, &model1});  // intentional wrong order

  // loading model from file
  EXPECT_THROW(document.Load(file_path), std::runtime_error);
}
