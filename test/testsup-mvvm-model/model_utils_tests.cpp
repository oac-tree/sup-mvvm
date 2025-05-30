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

#include "mvvm/model/model_utils.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/path.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>

using namespace mvvm;

class ModelUtilsTests : public ::testing::Test
{
public:
  class TestCompoundItem : public CompoundItem
  {
  public:
    TestCompoundItem() : CompoundItem("TestItem")
    {
      RegisterTag(TagInfo::CreateUniversalTag("default_tag"), /*set_as_default*/ true);
    }
  };

  class TestModel : public SessionModel
  {
  public:
    TestModel() : SessionModel("TestModel") {}
  };
};

TEST_F(ModelUtilsTests, GetTopItem)
{
  EXPECT_THROW(utils::GetTopItem<CompoundItem>(nullptr), RuntimeException);

  SessionModel model;
  EXPECT_EQ(utils::GetTopItem<>(&model), nullptr);
  EXPECT_EQ(utils::GetTopItem(&model), nullptr);

  auto property = model.InsertItem<PropertyItem>();
  auto compound = model.InsertItem<CompoundItem>();
  EXPECT_EQ(utils::GetTopItem<>(&model), property);
  EXPECT_EQ(utils::GetTopItem(&model), property);
  EXPECT_EQ(utils::GetTopItem<CompoundItem>(&model), compound);
}

TEST_F(ModelUtilsTests, GetTopItems)
{
  EXPECT_THROW(utils::GetTopItems<CompoundItem>(nullptr), RuntimeException);

  std::vector<SessionItem*> expected;

  SessionModel model;
  EXPECT_EQ(utils::GetTopItems<>(&model), expected);
  EXPECT_EQ(utils::GetTopItems(&model), expected);

  auto property1 = model.InsertItem<PropertyItem>();
  auto compound1 = model.InsertItem<CompoundItem>();
  auto property2 = model.InsertItem<PropertyItem>();
  auto compound2 = model.InsertItem<CompoundItem>();

  expected = {property1, compound1, property2, compound2};
  EXPECT_EQ(utils::GetTopItems<>(&model), expected);
  EXPECT_EQ(utils::GetTopItems(&model), expected);

  std::vector<CompoundItem*> expected2 = {compound1, compound2};
  EXPECT_EQ(utils::GetTopItems<CompoundItem>(&model), expected2);
}

TEST_F(ModelUtilsTests, FindItems)
{
  TestModel model;
  EXPECT_EQ(utils::FindItems<>(&model).size(), 1);             // because of rootItem
  EXPECT_EQ(utils::FindItems<SessionItem>(&model).size(), 1);  // because of rootItem
  EXPECT_EQ(utils::FindItems<TestCompoundItem>(&model).size(), 0);

  auto parent1 = model.InsertItem<TestCompoundItem>();
  auto property1 = model.InsertItem<PropertyItem>();
  auto parent2 = model.InsertItem<TestCompoundItem>();

  std::vector<TestCompoundItem*> expected2 = {parent1, parent2};
  EXPECT_EQ(utils::FindItems<TestCompoundItem>(&model), expected2);

  auto property2 = model.InsertItem<PropertyItem>(parent1);
  auto property3 = model.InsertItem<PropertyItem>(parent2);

  // order correspond to iteration order
  std::vector<PropertyItem*> expected3 = {property2, property1, property3};
  EXPECT_EQ(utils::FindItems<PropertyItem>(&model), expected3);
}

TEST_F(ModelUtilsTests, PathFromItem)
{
  SessionModel model;

  // unexisting path
  EXPECT_TRUE(utils::PathFromItem(nullptr).GetString().empty());
  // yet another unexisting path
  auto alienItem = std::make_unique<SessionItem>();
  EXPECT_TRUE(utils::PathFromItem(alienItem.get()).GetString().empty());

  // three children beneeth root item
  auto item0 = model.InsertItem<SessionItem>();
  item0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto item1 = model.InsertItem<SessionItem>();
  item1->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto item2 = model.InsertItem<SessionItem>();
  item2->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  EXPECT_EQ(utils::PathFromItem(item0).GetString(), "0");
  EXPECT_EQ(utils::PathFromItem(item1).GetString(), "1");
  EXPECT_EQ(utils::PathFromItem(item2).GetString(), "2");

  // adding granchildren to item0
  auto child00 = model.InsertItem<SessionItem>(item0);
  auto child01 = model.InsertItem<SessionItem>(item0);

  EXPECT_EQ(utils::PathFromItem(child00).GetString(), "0,0");
  EXPECT_EQ(utils::PathFromItem(child01).GetString(), "0,1");

  // adding grandchildren to item2
  auto child20 = model.InsertItem<SessionItem>(item2);
  child20->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  auto child200 = model.InsertItem<SessionItem>(child20);
  auto child201 = model.InsertItem<SessionItem>(child20);

  EXPECT_EQ(utils::PathFromItem(child200).GetString(), "2,0,0");
  EXPECT_EQ(utils::PathFromItem(child201).GetString(), "2,0,1");
}

TEST_F(ModelUtilsTests, ItemFromPath)
{
  SessionModel model;

  // access to non-existing item
  Path non_existing;
  non_existing.Append(8);
  EXPECT_EQ(utils::ItemFromPath(model, non_existing), nullptr);

  auto item0 = model.InsertItem<SessionItem>();
  item0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto item1 = model.InsertItem<SessionItem>();
  item1->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto item2 = model.InsertItem<SessionItem>();
  item2->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  EXPECT_EQ(utils::ItemFromPath(model, Path::CreateFromVector({0})), item0);
  EXPECT_EQ(utils::ItemFromPath(model, Path::CreateFromVector({1})), item1);
  EXPECT_EQ(utils::ItemFromPath(model, Path::CreateFromVector({2})), item2);

  auto child20 = model.InsertItem<SessionItem>(item2);
  child20->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child200 = model.InsertItem<SessionItem>(child20);
  auto child201 = model.InsertItem<SessionItem>(child20);

  EXPECT_EQ(utils::ItemFromPath(model, Path::CreateFromVector({2, 0})), child20);
  EXPECT_EQ(utils::ItemFromPath(model, Path::CreateFromVector({2, 0, 0})), child200);
  EXPECT_EQ(utils::ItemFromPath(model, Path::CreateFromVector({2, 0, 1})), child201);
}

//! Tests item copy when from root item to root item.

TEST_F(ModelUtilsTests, CopyModelItemRootContext)
{
  SessionModel model;

  // create single item with value
  auto item = model.InsertItem<SessionItem>();
  item->SetData(42.0);

  // copying to root item
  auto copy = utils::CopyItem(item, &model, model.GetRootItem(), TagIndex::Append());

  // checking copy
  ASSERT_TRUE(copy != nullptr);
  ASSERT_TRUE(copy != item);
  EXPECT_FALSE(copy->GetIdentifier().empty());
  EXPECT_NE(copy->GetIdentifier(), item->GetIdentifier());
  EXPECT_EQ(copy->Data<double>(), 42.0);
  EXPECT_EQ(model.GetRootItem()->GetAllItems().size(), 2);
  std::vector<SessionItem*> expected = {item, copy};
  EXPECT_EQ(model.GetRootItem()->GetAllItems(), expected);
}

//! Tests item copy from parent to root item.

TEST_F(ModelUtilsTests, CopyParentWithProperty)
{
  SessionModel model;

  // parent with single child and data on ite
  auto parent0 = model.InsertItem<SessionItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);
  auto child0 = model.InsertItem<SessionItem>(parent0);
  child0->SetData(42.0);

  // copying whole parent to root
  auto copy = utils::CopyItem(parent0, &model, model.GetRootItem(), TagIndex::Append());
  auto copy_child = copy->GetItem("defaultTag");

  ASSERT_TRUE(copy != nullptr);
  ASSERT_TRUE(copy_child != nullptr);
  EXPECT_FALSE(copy->GetIdentifier().empty());
  EXPECT_NE(copy->GetIdentifier(), parent0->GetIdentifier());
  EXPECT_EQ(copy_child->Data<double>(), 42.0);
}

//! Tests item copy for property item.

TEST_F(ModelUtilsTests, CopyFreeItem)
{
  SessionModel model;

  // single parent in a model
  auto parent0 = model.InsertItem<SessionItem>();
  parent0->RegisterTag(TagInfo::CreateUniversalTag("defaultTag"), /*set_as_default*/ true);

  // free item
  auto item = std::make_unique<PropertyItem>();
  item->SetData(42.0);

  // copying to parent
  auto copy = utils::CopyItem(item.get(), &model, parent0, TagIndex::Append());
  EXPECT_EQ(copy->Data<double>(), 42.0);
}

//! Attempt to copy property item into the same tag.

TEST_F(ModelUtilsTests, ForbiddenCopy)
{
  SessionModel model;

  // single parent in a model
  auto parent0 = model.InsertItem<SessionItem>();
  parent0->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::GetStaticType()));
  auto property = model.InsertItem<PropertyItem>(parent0, "property_tag");

  // copying property to same property tag is not allowed
  EXPECT_THROW(utils::CopyItem(property, &model, parent0, TagIndex::Append("property_tag")),
               InvalidOperationException);
  EXPECT_EQ(parent0->GetTotalItemCount(), 1);
}

//! FIXME restore test when copy is ready
// TEST_F(ModelUtilsTests, CreateCopy)
//{
//    ToyItems::SampleModel model;
//    auto layer = model.insertItem<ToyItems::LayerItem>();
//    layer->setProperty(ToyItems::LayerItem::P_THICKNESS, 42.0);

//    auto modelCopy = Utils::CreateCopy<ToyItems::SampleModel>(model);
//    auto layerCopy = modelCopy->topItem<ToyItems::LayerItem>();
//    EXPECT_EQ(layerCopy->property<double>(ToyItems::LayerItem::P_THICKNESS), 42.0);

//    // Copied model has unique identifiers
//    EXPECT_FALSE(model.rootItem()->identifier() == modelCopy->rootItem()->identifier());
//    EXPECT_FALSE(layerCopy->identifier() == layer->identifier());
//}

//! FIXME restore test when clone is ready
// TEST_F(ModelUtilsTests, CreateClone)
//{
//    ToyItems::SampleModel model;
//    auto layer = model.insertItem<ToyItems::LayerItem>();
//    layer->setProperty(ToyItems::LayerItem::P_THICKNESS, 42.0);

//    auto modelCopy = Utils::CreateClone<ToyItems::SampleModel>(model);
//    auto layerCopy = modelCopy->topItem<ToyItems::LayerItem>();
//    EXPECT_EQ(layerCopy->property<double>(ToyItems::LayerItem::P_THICKNESS), 42.0);

//    // Copied model has unique identifiers
//    EXPECT_TRUE(layerCopy->identifier() == layer->identifier());

//    // root item by current conveniton still has unique identifier event for cloned model
//    // probably for the uniformity this has to be changed, and test below changed to EXPECT_TRUE
//    // This will require change in JsonModelConverter
//    EXPECT_FALSE(model.rootItem()->identifier() == modelCopy->rootItem()->identifier());
//}
