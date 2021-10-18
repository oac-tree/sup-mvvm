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

#include "mvvm/viewmodel/propertyviewmodel.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/standarditems/vectoritem.h"
#include "mvvm/viewmodel/applicationmodel.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests for PropertyViewModel class.

class PropertyViewModelTest : public ::testing::Test
{
public:
  class LayerItem : public ModelView::CompoundItem
  {
  public:
    static inline const std::string Type = "Layer";
    LayerItem() : CompoundItem(Type)
    {
      AddProperty("Thickness", 42.0);
      AddProperty("Color", std::string("black"));
    }
  };

  //! Represents multilayer with collection of layers.
  class MultiLayerItem : public ModelView::CompoundItem
  {
  public:
    static inline const std::string Type = "MultiLayer";
    MultiLayerItem() : CompoundItem(Type)
    {
      RegisterTag(ModelView::TagInfo::CreateUniversalTag("Layer", {LayerItem::Type}),
                  /*set_as_default*/ true);
    }
  };

  class TestModel : public ApplicationModel
  {
  public:
    TestModel() : ApplicationModel("TestModel")
    {
      RegisterItem<LayerItem>();
      RegisterItem<MultiLayerItem>();
    }
  };
};

TEST_F(PropertyViewModelTest, InitialState)
{
  SessionModel model;
  PropertyViewModel viewModel(&model);
  EXPECT_EQ(viewModel.rowCount(), 0);
  EXPECT_EQ(viewModel.columnCount(), 0);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(QModelIndex()), model.GetRootItem());
}

TEST_F(PropertyViewModelTest, BaseItem)
{
  SessionModel model;
  model.InsertItem<SessionItem>();

  PropertyViewModel viewModel(&model);

  // Root item has default tag and all items considered as top items.
  // PropertyViewModel shouldn't see any items.
  EXPECT_EQ(viewModel.rowCount(), 0);
  EXPECT_EQ(viewModel.columnCount(), 0);
}

TEST_F(PropertyViewModelTest, PropertyItem)
{
  SessionModel model;
  auto parent = model.InsertItem<SessionItem>();

  parent->RegisterTag(TagInfo::CreateUniversalTag("universal_tag"));
  parent->RegisterTag(TagInfo::CreatePropertyTag("property_tag", PropertyItem::Type));

  model.InsertItem<SessionItem>(parent, "universal_tag");
  model.InsertItem<PropertyItem>(parent, "property_tag");
  model.InsertItem<SessionItem>(parent, "universal_tag");

  PropertyViewModel viewModel(&model);
  viewModel.SetRootSessionItem(parent);

  // View model should see only property item belonging to parent.
  EXPECT_EQ(viewModel.rowCount(), 1);
  EXPECT_EQ(viewModel.columnCount(), 2);
}

//! VectorItem in a model.

TEST_F(PropertyViewModelTest, VectorItem)
{
  SessionModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyViewModel viewModel(&model);

  EXPECT_EQ(viewModel.rowCount(), 0);  // root item doesn't have properties
  EXPECT_EQ(viewModel.columnCount(), 0);

  // switching to vectorItem and checking that it has 3 properties
  viewModel.SetRootSessionItem(parent);
  EXPECT_EQ(viewModel.rowCount(), 3);
  EXPECT_EQ(viewModel.columnCount(), 2);
}

//! VectorItem in with hidden component. An item with setVisible(false) set shouldn't appear in a
//! view model. The current implementation is limited and respects this flag only if it was set
//! before the view model creation.

TEST_F(PropertyViewModelTest, VectorItemWithHiddenComponent)
{
  SessionModel model;
  auto vector_item = model.InsertItem<VectorItem>();
  vector_item->GetItem(VectorItem::P_Y)->SetVisible(false);

  PropertyViewModel viewModel(&model);

  EXPECT_EQ(viewModel.rowCount(), 0);  // root item doesn't have properties
  EXPECT_EQ(viewModel.columnCount(), 0);

  // switching to vectorItem and checking that it has only 2 properties (y-item was skipped).
  viewModel.SetRootSessionItem(vector_item);
  EXPECT_EQ(viewModel.rowCount(), 2);
  EXPECT_EQ(viewModel.columnCount(), 2);

  auto vector_index = QModelIndex();
  auto x_index = viewModel.index(0, 0, vector_index);
  auto z_index = viewModel.index(1, 0, vector_index);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(vector_index), vector_item);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(x_index), vector_item->GetItem(VectorItem::P_X));
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(z_index), vector_item->GetItem(VectorItem::P_Z));

  // attempt to make P_Y visible again
  vector_item->GetItem(VectorItem::P_Y)->SetVisible(true);

  // The current implementation is limited: PropertyViewModel doesn't listen for updates
  // in isVisible flag. If flag is changed after the model creation, item still be invisible.

  vector_index = QModelIndex();
  x_index = viewModel.index(0, 0, vector_index);
  z_index = viewModel.index(1, 0, vector_index);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(vector_index), vector_item);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(x_index), vector_item->GetItem(VectorItem::P_X));
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(z_index), vector_item->GetItem(VectorItem::P_Z));
}

//! LayerItem in a MultiLayer.

TEST_F(PropertyViewModelTest, LayerInMultiLayerAsRootItem)
{
  TestModel model;
  auto multilayer = model.InsertItem<MultiLayerItem>();
  auto layer = model.InsertItem<LayerItem>(multilayer);

  PropertyViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(layer);

  // check layer thickness and color
  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // remove multilayer
  model.RemoveItem(model.GetRootItem(), {"", 0});

  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 0);
}
