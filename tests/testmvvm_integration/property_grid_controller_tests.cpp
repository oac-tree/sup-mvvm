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

#include "mvvm/widgets/property_grid_controller.h"

#include <gtest/gtest.h>

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
  QStandardItemModel model;
  auto parent_item = model.invisibleRootItem();
  QList<QStandardItem*> items = {new QStandardItem("a"), new QStandardItem("b")};

  // first item in a row is a label
  items.at(0)->setEditable(false);
  parent_item->insertRow(0, items);
  // second item in a raw carries an integet
  items.at(1)->setData(42, Qt::EditRole);

  PropertyGridController controller(&model);

  auto widget0 = controller.CreateWidget(model.index(0, 0));
  auto widget1 = controller.CreateWidget(model.index(0, 1));

  // checking that controller can create proper editors for columns
  EXPECT_NE(dynamic_cast<QLabel*>(widget0.get()), nullptr);
  EXPECT_NE(dynamic_cast<QSpinBox*>(widget1.get()), nullptr);
}

TEST_F(PropertyGridControllerTests, CreateGridForStandardModel)
{
  // preparing the model
  QStandardItemModel model;
  auto parent_item = model.invisibleRootItem();
  QList<QStandardItem*> row0 = {new QStandardItem("a"), new QStandardItem("b"),
                                new QStandardItem("b")};
  QList<QStandardItem*> row1 = {new QStandardItem("a"), new QStandardItem("b"),
                                new QStandardItem("b")};
  parent_item->insertRow(0, row0);
  parent_item->insertRow(0, row1);

  PropertyGridController controller(&model);

  auto editor_grid = controller.CreateGrid();

  EXPECT_EQ(editor_grid.size(), 2);
  EXPECT_EQ(editor_grid[0].size(), 3);
}

// TEST_F(PropertyGridControllerTests, CreateGridForPropertyViewModel)
//{

//}
