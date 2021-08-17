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
#include "mvvm/serialization/treedataitemconverter.h"
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

TEST_F(XmlDocumentTest, SaveLoadSingleModel)
{
  const auto file_path = GetFilePath("SaveLoadSingleModel.xml");

  SessionModel model("TestModel");
  XmlDocument document({&model});

  // filling model with parent and child
  auto parent = model.insertItem<SessionItem>();
  parent->SetDisplayName("parent_name");
  parent->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  const auto parent_identifier = parent->GetIdentifier();

  parent->setData(42);
  auto child = model.insertItem<PropertyItem>(parent);
  child->SetDisplayName("child_name");
  const auto child_identifier = child->GetIdentifier();

  // saving model in file
  document.Save(file_path);

  // modifying model further
  model.removeItem(model.rootItem(), {"", 0});

  // loading model from file
  document.Load(file_path);

  // checking that it is as it was right after the save

  // accessing reconstructed parent and child
  auto reco_parent = model.rootItem()->getItem("", 0);
  auto reco_child = reco_parent->getItem("", 0);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent->model(), &model);
  EXPECT_EQ(reco_parent->GetType(), SessionItem::Type);
  EXPECT_EQ(reco_parent->parent(), model.rootItem());
  EXPECT_EQ(reco_parent->GetDisplayName(), "parent_name");
  EXPECT_EQ(reco_parent->childrenCount(), 1);
  EXPECT_EQ(reco_parent->GetIdentifier(), parent_identifier);
  EXPECT_EQ(reco_parent->itemTags()->GetDefaultTag(), "defaultTag");
  EXPECT_EQ(reco_parent->data<int>(), 42);

  // checking child reconstruction
  EXPECT_EQ(reco_child->model(), &model);
  EXPECT_EQ(reco_child->GetType(), PropertyItem::Type);
  EXPECT_EQ(reco_child->parent(), reco_parent);
  EXPECT_EQ(reco_child->GetDisplayName(), "child_name");
  EXPECT_EQ(reco_child->childrenCount(), 0);
  EXPECT_EQ(reco_child->GetIdentifier(), child_identifier);
  EXPECT_EQ(reco_child->itemTags()->GetDefaultTag(), "");
}

//! Saving two models with content into document and restoring it after.

TEST_F(XmlDocumentTest, SaveLoadTwoModels)
{
  const auto file_path = GetFilePath("SaveLoadTwoModels.xml");

  TestModel1 model1;
  TestModel2 model2;
  XmlDocument document({&model1, &model2});

  // filling models
  auto parent1 = model1.insertItem<SessionItem>();
  const auto parent_identifier1 = parent1->GetIdentifier();

  auto parent2 = model2.insertItem<SessionItem>();
  const auto parent_identifier2 = parent2->GetIdentifier();

  // saving models in file
  document.Save(file_path);

  // modifying model further
  model1.removeItem(model1.rootItem(), {"", 0});
  model2.removeItem(model2.rootItem(), {"", 0});

  // loading model from file
  document.Load(file_path);

  // checking that it is as it was right after the save

  // accessing reconstructed parent and child
  auto reco_parent1 = model1.rootItem()->getItem("", 0);
  auto reco_parent2 = model2.rootItem()->getItem("", 0);

  // checking parent reconstruction
  EXPECT_EQ(reco_parent1->model(), &model1);
  EXPECT_EQ(reco_parent1->parent(), model1.rootItem());
  EXPECT_EQ(reco_parent1->GetIdentifier(), parent_identifier1);

  EXPECT_EQ(reco_parent2->model(), &model2);
  EXPECT_EQ(reco_parent2->parent(), model2.rootItem());
  EXPECT_EQ(reco_parent2->GetIdentifier(), parent_identifier2);
}

//! Attempt to restore models in wrong order.

TEST_F(XmlDocumentTest, LoadModelsInWrongOrder)
{
  const auto file_path = GetFilePath("LoadModelsInWrongOrder.xml");
  TestModel1 model1;
  TestModel2 model2;

  // filling models
  auto parent1 = model1.insertItem<SessionItem>();
  const auto parent_identifier1 = parent1->GetIdentifier();

  auto parent2 = model2.insertItem<SessionItem>();
  const auto parent_identifier2 = parent2->GetIdentifier();

  // saving models in file
  {
    XmlDocument document({&model1, &model2});
    document.Save(file_path);
  }

  // modifying model further
  model1.removeItem(model1.rootItem(), {"", 0});
  model2.removeItem(model2.rootItem(), {"", 0});

  XmlDocument document({&model2, &model1});  // intentional wrong order

  // loading model from file
  EXPECT_THROW(document.Load(file_path), std::runtime_error);
}
