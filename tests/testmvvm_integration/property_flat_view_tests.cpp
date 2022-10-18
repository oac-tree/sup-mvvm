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
#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/property_table_viewmodel.h>

#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTextEdit>

using namespace mvvm;

//! Testing PropertyFlatView.

class PropertyFlatViewTests : public ::testing::Test
{
public:
  //! Returns 2D vector with widgets populating QGridLayout.
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

  //! Create row made of two items, carrying a string, and the integer value.
  static QList<QStandardItem*> CreateItemRow(const QString& name, int value)
  {
    QList<QStandardItem*> result = {new QStandardItem("a"), new QStandardItem("b")};
    result.at(0)->setData(name, Qt::EditRole);
    result.at(0)->setEditable(false);
    result.at(1)->setData(value, Qt::EditRole);
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
  parent_item->insertRow(0, CreateItemRow("name", 42));

  flat_view.SetViewModel(&view_model);

  // checking layout children
  auto layout = flat_view.findChild<QGridLayout*>();
  ASSERT_TRUE(layout);

  EXPECT_EQ(layout->rowCount(), 1);
  EXPECT_EQ(layout->columnCount(), 2);
  auto layout_widgets = GetLayoutWidgets(layout);
  ASSERT_EQ(layout_widgets.size(), 1);
  ASSERT_EQ(layout_widgets[0].size(), 2);

  qDebug() << layout_widgets[0][0];
  EXPECT_TRUE(dynamic_cast<QLabel*>(layout_widgets[0][0]));
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(layout_widgets[0][1]));
}

//! Model with two items is assigned to PropertyFlatView.
//! Check that grid layout is updated when the model gets new rows.

TEST_F(PropertyFlatViewTests, GridLayoutUpdate)
{
  PropertyFlatView flat_view;

  QStandardItemModel view_model;
  auto parent_item = view_model.invisibleRootItem();
  parent_item->insertRow(0, CreateItemRow("name", 42));

  flat_view.SetViewModel(&view_model);

  // checking layout children
  auto layout = flat_view.findChild<QGridLayout*>();
  ASSERT_TRUE(layout);

  EXPECT_EQ(layout->rowCount(), 1);
  EXPECT_EQ(layout->columnCount(), 2);

  // after adding more rows, layout changes
  parent_item->insertRow(0, CreateItemRow("name", 42));

  EXPECT_EQ(layout->rowCount(), 2);
  EXPECT_EQ(layout->columnCount(), 2);

  auto layout_widgets = GetLayoutWidgets(layout);
  ASSERT_EQ(layout_widgets.size(), 2);
  ASSERT_EQ(layout_widgets[0].size(), 2);

  EXPECT_TRUE(dynamic_cast<QLabel*>(layout_widgets[1][0]));
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(layout_widgets[1][1]));
}

TEST_F(PropertyFlatViewTests, CreateGridForPropertyTableViewModel)
{
  ApplicationModel model;
  model.InsertItem<VectorItem>();
  model.InsertItem<VectorItem>();

  PropertyTableViewModel view_model(&model);

  PropertyFlatView flat_view;
  flat_view.SetViewModel(&view_model);

  auto layout = flat_view.findChild<QGridLayout*>();

  auto layout_widgets = GetLayoutWidgets(layout);
  ASSERT_EQ(layout_widgets.size(), 2);
  ASSERT_EQ(layout_widgets[0].size(), 3);
}
