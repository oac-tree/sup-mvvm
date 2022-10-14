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

#include "mvvm/viewmodel/all_items_viewmodel.h"
#include "mvvm/viewmodel/property_table_viewmodel.h"
#include "mvvm/viewmodel/property_viewmodel.h"
#include "mvvm/widgets/property_grid_controller.h"

#include <gtest/gtest.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/vector_item.h>

#include <QDebug>
#include <QLabel>
#include <QSignalSpy>
#include <QSpinBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QWidget>
#include <iostream>

using namespace mvvm;

//! Testing PropertyGridController.

class PropertyGridControllerTests : public ::testing::Test
{
};

//! Checking method CreateWidget.
//! Use QStandardItemModel with single row with label and data.

TEST_F(PropertyGridControllerTests, CreateWidget)
{
  // preparing the model
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  QList<QStandardItem*> items = {new QStandardItem("a"), new QStandardItem("b")};

  // first item in a row is a label
  items.at(0)->setEditable(false);
  parent_item->insertRow(0, items);
  // second item in a row carries an integer
  items.at(1)->setData(42, Qt::EditRole);

  PropertyGridController controller(&view_model);

  auto widget0 = controller.CreateWidget(view_model.index(0, 0));
  auto widget1 = controller.CreateWidget(view_model.index(0, 1));

  // checking that controller can create proper editors for columns
  EXPECT_NE(dynamic_cast<QLabel*>(widget0.get()), nullptr);
  EXPECT_NE(dynamic_cast<QSpinBox*>(widget1.get()), nullptr);
}

//! Checking method CreateGrid.
//! Use QStandardItemModel with two rows and three columns.

TEST_F(PropertyGridControllerTests, CreateGridForStandardModel)
{
  // preparing the model
  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  QList<QStandardItem*> row0 = {new QStandardItem("a"), new QStandardItem("b"),
                                new QStandardItem("b")};
  QList<QStandardItem*> row1 = {new QStandardItem("a"), new QStandardItem("b"),
                                new QStandardItem("b")};
  parent_item->insertRow(0, row0);
  parent_item->insertRow(0, row1);

  PropertyGridController controller(&view_model);

  auto editor_grid = controller.CreateGrid();

  EXPECT_EQ(editor_grid.size(), 2);
  EXPECT_EQ(editor_grid[0].size(), 3);
}

//! Checking method CreateGrid.
//! Use PropertyViewModel and single VectorItem in it.

TEST_F(PropertyGridControllerTests, CreateGridForPropertyViewModel)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(parent);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateGrid();

  EXPECT_EQ(editor_grid.size(), 3);
  EXPECT_EQ(editor_grid[0].size(), 2);
}

//! Checking method CreateGrid.
//! Use PropertyTableViewModel with two VectorItem in it.

TEST_F(PropertyGridControllerTests, CreateGridForPropertyTableViewModel)
{
  ApplicationModel model;
  model.InsertItem<VectorItem>();
  model.InsertItem<VectorItem>();

  PropertyTableViewModel view_model(&model);

  PropertyGridController controller(&view_model);
  auto editor_grid = controller.CreateGrid();

  EXPECT_EQ(editor_grid.size(), 2);
  EXPECT_EQ(editor_grid[0].size(), 3);
}

//! Checking controller signaling while inserting item through the model.

TEST_F(PropertyGridControllerTests, GridChanged)
{
  ApplicationModel model;

  AllItemsViewModel view_model(&model);

  PropertyGridController controller(&view_model);

  QSignalSpy spy_grid_changed(&controller, &PropertyGridController::GridChanged);

  model.InsertItem<PropertyItem>();

  EXPECT_EQ(spy_grid_changed.count(), 1);
}
