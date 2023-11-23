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

#include "mvvm/viewmodel/top_items_viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/test/folder_based_test.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace mvvm;

class TopItemsViewModelTest : public testutils::FolderBasedTest
{
public:
  class LayerItem : public mvvm::CompoundItem
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
  class MultiLayerItem : public mvvm::CompoundItem
  {
  public:
    static inline const std::string Type = "MultiLayer";
    MultiLayerItem() : CompoundItem(Type)
    {
      RegisterTag(mvvm::TagInfo::CreateUniversalTag("Layer", {LayerItem::Type}),
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

  //! Represents a layer, with thickness and color, and possibly populated with particles.

  TopItemsViewModelTest() : FolderBasedTest("test_TopItemsViewModel"), m_viewmodel(&m_model) {}

  TestModel m_model;
  TopItemsViewModel m_viewmodel;
};

TEST_F(TopItemsViewModelTest, InitialState)
{
  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
  EXPECT_EQ(m_viewmodel.GetRootSessionItem(), m_model.GetRootItem());
}

//! Insert LayerItem in empty model.

TEST_F(TopItemsViewModelTest, InsertLayerThenRemove)
{
  QSignalSpy spyInsert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  m_model.InsertItem<LayerItem>();

  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyInsert.count(), 1);

  m_model.TakeItem(m_viewmodel.GetRootSessionItem(), {"", 0});
  EXPECT_EQ(spyRemove.count(), 1);
  EXPECT_EQ(spyInsert.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);
}

//! Hidden LayerItem in a model. An item with setVisible(false) set shouldn't appear in a view
//! model. The current implementation is limited and respects this flag only if it was set before
//! the view model creation.

TEST_F(TopItemsViewModelTest, HidenLayerInModel)
{
  TestModel model;
  // the model with two layers, where one is invisible
  auto layer0 = model.InsertItem<LayerItem>();
  auto layer1 = model.InsertItem<LayerItem>();
  layer0->SetVisible(false);

  TopItemsViewModel viewmodel(&model);

  // TopItemsViewModel should see only one layer.
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 2);
  auto layer1_index = viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(layer1_index), layer1);

  // The current implementation is limited: TopItemsViewModel doesn't listen for updates
  // in isVisible flag. If flag is set after the model creation, item still be visible.

  layer1->SetVisible(false);

  // model still sees the layer
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 2);
  layer1_index = viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(layer1_index), layer1);
}

//! Insert LayerItem in MultiLayer.

TEST_F(TopItemsViewModelTest, InsertLayerInMultiLayerThenRemove)
{
  QSignalSpy spyInsert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  // inserting multilayer
  auto multilayer = m_model.InsertItem<MultiLayerItem>();

  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyInsert.count(), 1);

  // insert layer
  auto layer = m_model.InsertItem<LayerItem>(multilayer);
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyInsert.count(), 2);

  // checking their indices
  auto multilayer_index = m_viewmodel.index(0, 0, QModelIndex());
  auto layer_index = m_viewmodel.index(0, 0, multilayer_index);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(multilayer_index), multilayer);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(layer_index), layer);

  // checking row and columns
  EXPECT_EQ(m_viewmodel.rowCount(multilayer_index), 1);
  EXPECT_EQ(m_viewmodel.columnCount(multilayer_index), 2);
  EXPECT_EQ(m_viewmodel.rowCount(layer_index), 0);
  EXPECT_EQ(m_viewmodel.columnCount(layer_index), 2);

  // removing layer
  m_model.RemoveItem(layer);
  EXPECT_EQ(spyRemove.count(), 1);
  EXPECT_EQ(spyInsert.count(), 2);
  EXPECT_EQ(m_viewmodel.rowCount(multilayer_index), 0);
  EXPECT_EQ(m_viewmodel.columnCount(multilayer_index), 2);
}

//! Insert LayerItem in MultiLayer while multilayer is root item. Then deleting multilayer.

TEST_F(TopItemsViewModelTest, MultiLayerAsRooItem)
{
  // setting up single multilayer playing the role
  auto multilayer = m_model.InsertItem<MultiLayerItem>();
  m_viewmodel.SetRootSessionItem(multilayer);
  QSignalSpy spyInsert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &ViewModelBase::rowsRemoved);
  QSignalSpy spyReset(&m_viewmodel, &ViewModelBase::modelAboutToBeReset);

  // initial conditions
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(QModelIndex()), multilayer);  // our new root

  // adding layer
  m_model.InsertItem<LayerItem>(multilayer);
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyInsert.count(), 1);

  // removing multilayer
  m_model.RemoveItem(multilayer);

  EXPECT_EQ(spyReset.count(), 1);
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);
}
