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

#include "mvvm/widgets/property_flat_view.h"

#include <gtest/gtest.h>

#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QStandardItemModel>

using namespace mvvm;

//! Testing PropertyFlatView.

class PropertyFlatViewTests : public ::testing::Test
{
public:
  static std::vector<std::vector<QWidget*>> GetLayoutWidgets(QGridLayout* layout)
  {
    std::vector<std::vector<QWidget*>> result;

    result.resize(layout->rowCount());
    for (int row = 0; row < layout->rowCount(); ++row)
    {
      for (int col = 0; col < layout->columnCount(); ++col)
      {
        result[row].push_back(layout->itemAtPosition(row, col)->widget());
      }
    }
    return result;
  }
};

//! Model with two items is assigned to PropertyFlatView.
//! Check that grid layout is properly populated with editors.

TEST_F(PropertyFlatViewTests, SetViewModel)
{
  PropertyFlatView flat_view;

  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  QList<QStandardItem*> items = {new QStandardItem("a"), new QStandardItem("b")};
  // first item in a row is a label
  items.at(0)->setEditable(false);
  parent_item->insertRow(0, items);
  // second item in a row carries an integer
  items.at(1)->setData(42, Qt::EditRole);

  flat_view.SetViewModel(&view_model);

  // checking layout children
  auto layout = flat_view.findChild<QGridLayout*>();
  ASSERT_TRUE(layout);

  EXPECT_EQ(layout->rowCount(), 1);
  EXPECT_EQ(layout->columnCount(), 2);
  auto layout_widgets = GetLayoutWidgets(layout);
  ASSERT_EQ(layout_widgets.size(), 1);
  ASSERT_EQ(layout_widgets[0].size(), 2);

  EXPECT_TRUE(dynamic_cast<QLabel*>(layout_widgets[0][0]));
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(layout_widgets[0][1]));
}
