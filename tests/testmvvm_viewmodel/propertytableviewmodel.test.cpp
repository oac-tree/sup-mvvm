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

#include "mvvm/viewmodel/propertytableviewmodel.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/standarditems/vectoritem.h"

#include <gtest/gtest.h>
#include "toyitems.h"

#include <QSignalSpy>
#include <QStandardItemModel>

using namespace mvvm;

class PropertyTableViewModelTest : public ::testing::Test
{
};

TEST_F(PropertyTableViewModelTest, InitialState)
{
  ApplicationModel model;
  PropertyTableViewModel view_model(&model);
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(QModelIndex()), model.GetRootItem());
}

TEST_F(PropertyTableViewModelTest, BaseItem)
{
  ApplicationModel model;
  model.InsertItem<SessionItem>();

  PropertyTableViewModel view_model(&model);

  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);
}

TEST_F(PropertyTableViewModelTest, PropertyItem)
{
  ApplicationModel model;

  auto parent = model.InsertItem<CompoundItem>();
  auto property = parent->AddProperty("thickness", 42.0);
  parent->RegisterTag(TagInfo::CreateUniversalTag("universal_tag"));
  model.InsertItem<SessionItem>(parent, "universal_tag");

  PropertyTableViewModel view_model(&model);

  // one cell corresponding to single item at property_tag of our parent
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 1);

  view_model.SetRootSessionItem(parent);
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);
}

//! VectorItem in a model.

TEST_F(PropertyTableViewModelTest, VectorItem)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyTableViewModel view_model(&model);

  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 3);

  // switching to vectorItem and checking that it has 3 properties
  view_model.SetRootSessionItem(parent);
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);
}

//! MultiLayer with layers, view model still looks to the RootItem.
//! No MultiLayer should be visible in table.

TEST_F(PropertyTableViewModelTest, MultiLayerAndRootItem)
{
  using namespace TestUtils::ToyItems;

  SampleModel model;
  auto multilayer = model.InsertItem<MultiLayerItem>();
  model.InsertItem<LayerItem>(multilayer);
  model.InsertItem<LayerItem>(multilayer);

  PropertyTableViewModel viewModel(&model);

  // ViewModel should be empty, since we are looking to RootItem.
  EXPECT_EQ(viewModel.rowCount(), 0);
  EXPECT_EQ(viewModel.columnCount(), 0);
}

//! MultiLayer with layers, multilayer is given as root index.

TEST_F(PropertyTableViewModelTest, MultiLayer)
{
  using namespace TestUtils::ToyItems;

  SampleModel model;
  auto multilayer = model.InsertItem<MultiLayerItem>();
  model.InsertItem<LayerItem>(multilayer);
  model.InsertItem<LayerItem>(multilayer);

  PropertyTableViewModel viewModel(&model);
  viewModel.SetRootSessionItem(multilayer);

  EXPECT_EQ(viewModel.rowCount(), 2);     // two layers
  EXPECT_EQ(viewModel.columnCount(), 2);  // layer thickness and color

  EXPECT_EQ(viewModel.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString(),
            QString("Thickness"));
  EXPECT_EQ(viewModel.headerData(1, Qt::Horizontal, Qt::DisplayRole).toString(), QString("Color"));

  // add another layer
  model.InsertItem<LayerItem>(multilayer);
  EXPECT_EQ(viewModel.rowCount(), 3);     // three layers
  EXPECT_EQ(viewModel.columnCount(), 2);  // layer thickness and color

  EXPECT_EQ(viewModel.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString(),
            QString("Thickness"));
  EXPECT_EQ(viewModel.headerData(1, Qt::Horizontal, Qt::DisplayRole).toString(), QString("Color"));

  // switching view model back to model's root, table should be empty
  viewModel.SetRootSessionItem(model.GetRootItem());
  EXPECT_EQ(viewModel.rowCount(), 0);     // two layers
  EXPECT_EQ(viewModel.columnCount(), 0);  // layer thickness and color
}

//! Check signaling of QStandardItemModel to learn from that.
//! Does insertion of row triggers columns-related signaling? Answer: no.

TEST_F(PropertyTableViewModelTest, StandardItemModel)
{
  QStandardItemModel model;
  QStandardItem* parentItem = model.invisibleRootItem();

  QSignalSpy spyAboutInserInsert(&model, &ViewModelBase::rowsAboutToBeInserted);
  QSignalSpy spyInsert(&model, &ViewModelBase::rowsInserted);
  QSignalSpy spyAboutRemove(&model, &ViewModelBase::rowsAboutToBeRemoved);
  QSignalSpy spyRemove(&model, &ViewModelBase::rowsRemoved);
  QSignalSpy spyAboutReset(&model, &ViewModelBase::modelAboutToBeReset);
  QSignalSpy spyReset(&model, &ViewModelBase::modelReset);

  QList<QStandardItem*> items = {new QStandardItem("a"), new QStandardItem("b")};
  parentItem->insertRow(0, items);

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_EQ(model.columnCount(), 2);

  EXPECT_EQ(spyAboutInserInsert.count(), 1);
  EXPECT_EQ(spyInsert.count(), 1);
  EXPECT_EQ(spyAboutRemove.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 0);
  EXPECT_EQ(spyReset.count(), 0);

  QList<QVariant> arguments = spyInsert.takeFirst();
  ASSERT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}

//! Initialize PropertyTableViewModel with empty SessionModel.
//! VectorItem in a model.

TEST_F(PropertyTableViewModelTest, InsertItemSignaling)
{
  ApplicationModel model;
  PropertyTableViewModel view_model(&model);

  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);

  QSignalSpy spyAboutInserInsert(&view_model, &ViewModelBase::rowsAboutToBeInserted);
  QSignalSpy spyInsert(&view_model, &ViewModelBase::rowsInserted);
  QSignalSpy spyAboutRemove(&view_model, &ViewModelBase::rowsAboutToBeRemoved);
  QSignalSpy spyRemove(&view_model, &ViewModelBase::rowsRemoved);
  QSignalSpy spyAboutReset(&view_model, &ViewModelBase::modelAboutToBeReset);
  QSignalSpy spyReset(&view_model, &ViewModelBase::modelReset);
  QSignalSpy spyLayout(&view_model, &ViewModelBase::layoutChanged);

  // inserting item
  model.InsertItem<VectorItem>();
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 3);

  EXPECT_EQ(spyAboutInserInsert.count(), 1);
  EXPECT_EQ(spyInsert.count(), 1);
  EXPECT_EQ(spyAboutRemove.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 0);
  EXPECT_EQ(spyReset.count(), 0);

  // Checking that notification from PropertyTableViewModel::insertRow works,
  // when the model changes from columnsCount=0 to columdCount = 3
  EXPECT_EQ(spyLayout.count(), 1);

  QList<QVariant> arguments = spyInsert.takeFirst();
  ASSERT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);

  arguments = spyAboutInserInsert.takeFirst();
  ASSERT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);

  spyLayout.takeFirst();  // clearing layout signal

  // inserting item
  model.InsertItem<VectorItem>();
  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), 3);

  EXPECT_EQ(spyAboutInserInsert.count(), 1);
  EXPECT_EQ(spyInsert.count(), 1);
  EXPECT_EQ(spyAboutRemove.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 0);
  EXPECT_EQ(spyReset.count(), 0);
  EXPECT_EQ(spyLayout.count(), 0);  // no second call for LayoutChange

  arguments = spyInsert.takeFirst();
  ASSERT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 1);
  EXPECT_EQ(arguments.at(2).value<int>(), 1);
}
