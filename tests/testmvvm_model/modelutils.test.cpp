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

#include "mvvm/model/modelutils.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/model/path.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ModelUtilsTest : public ::testing::Test
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
    TestModel() : SessionModel("TestModel") { RegisterItem<TestCompoundItem>(); }
  };
};

TEST_F(ModelUtilsTest, FindItems)
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

TEST_F(ModelUtilsTest, PathFromItem)
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

TEST_F(ModelUtilsTest, ItemFromPath)
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

TEST_F(ModelUtilsTest, HasSignals)
{
  SessionModel model;
  EXPECT_FALSE(utils::HasSignals(&model));

  ApplicationModel application_model;
  EXPECT_TRUE(utils::HasSignals(&application_model));
}

//! FIXME restore test when copy is ready
// TEST_F(ModelUtilsTest, CreateCopy)
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
// TEST_F(ModelUtilsTest, CreateClone)
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

//! FIXME restore test  MoveUp
// TEST_F(ModelUtilsTest, MoveItemUp)
//{
//    ToyItems::SampleModel model;

//    auto multilayer = model.insertItem<ToyItems::MultiLayerItem>();
//    auto layer0 = model.insertItem<ToyItems::LayerItem>(multilayer);
//    auto layer1 = model.insertItem<ToyItems::LayerItem>(multilayer);
//    auto layer2 = model.insertItem<ToyItems::LayerItem>(multilayer);

//    std::vector<SessionItem*> expected = {layer0, layer1, layer2};

//    // original layout
//    EXPECT_EQ(multilayer->getItems(ToyItems::MultiLayerItem::T_LAYERS), expected);

//    // moving top layer up doesn't change the order
//    Utils::MoveUp(layer0);
//    EXPECT_EQ(multilayer->getItems(ToyItems::MultiLayerItem::T_LAYERS), expected);

//    // moving bottom layer up does change the order
//    Utils::MoveUp(layer2);
//    expected = {layer0, layer2, layer1};
//    EXPECT_EQ(multilayer->getItems(ToyItems::MultiLayerItem::T_LAYERS), expected);
//}

//! FIXME restore test MoveDown
// TEST_F(ModelUtilsTest, MoveItemDown)
//{
//    ToyItems::SampleModel model;

//    auto multilayer = model.insertItem<ToyItems::MultiLayerItem>();
//    auto layer0 = model.insertItem<ToyItems::LayerItem>(multilayer);
//    auto layer1 = model.insertItem<ToyItems::LayerItem>(multilayer);
//    auto layer2 = model.insertItem<ToyItems::LayerItem>(multilayer);

//    std::vector<SessionItem*> expected = {layer0, layer1, layer2};

//    // original layout
//    EXPECT_EQ(multilayer->getItems(ToyItems::MultiLayerItem::T_LAYERS), expected);

//    // moving bottom layer down doesn't change the order
//    Utils::MoveDown(layer2);
//    EXPECT_EQ(multilayer->getItems(ToyItems::MultiLayerItem::T_LAYERS), expected);

//    // moving top layer down doesn't change the order
//    Utils::MoveDown(layer0);
//    expected = {layer1, layer0, layer2};
//    EXPECT_EQ(multilayer->getItems(ToyItems::MultiLayerItem::T_LAYERS), expected);
//}
