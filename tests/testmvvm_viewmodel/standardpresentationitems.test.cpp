/**************************************************************************
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

#include "mvvm/viewmodel/standardpresentationitems.h"

#include "mvvm/model/sessionitem.h"

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

class StandardPresentationItemsTest : public ::testing::Test
{
public:
  class TestPresentation : public SessionItemPresentation
  {
  public:
    TestPresentation(SessionItem* item, int role) : SessionItemPresentation(item, role) {}
  };
};

TEST_F(StandardPresentationItemsTest, SessionItemPresentationInitialState)
{
  SessionItem item;
  TestPresentation presentation(&item, 42);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), 42);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemDataForNoData)
{
  SessionItem item;

  DataPresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemDataForDouble)
{
  const double expected{42.0};

  SessionItem item;
  item.SetData(expected);

  DataPresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_EQ(presentation.Data(Qt::EditRole), expected);
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), expected);
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemSetDataForDouble)
{
  SessionItem item;
  item.SetData(42.0);

  DataPresentationItem presentation(&item);

  const double new_data{43.0};

  // not possible to set DisplayRole
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::DisplayRole));

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QVariant::fromValue(new_data));  // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));     // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<double>(), new_data);  // new data

  // attempt to set same data twice
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));

  // it is not allowed to set another type of data
  QVariant not_allowed_value("Layer");
  EXPECT_THROW(presentation.SetData(not_allowed_value, Qt::EditRole), std::runtime_error);
}

//! FIXME uncomment after EditorRole implementation

//! ViewDataItem::data method for QColor.
//! Checks that the data method is correctly forwarded to underlying SessionItem.

// TEST_F(StandardViewItemsTest, ViewDataItem_dataForColor)
//{
//     // create SessionItem with data on board
//     SessionItem item;
//     QVariant expected = QVariant::fromValue(QColor(Qt::green));
//     EXPECT_TRUE(item.setData(expected));

//    ViewDataItem viewItem(&item);
//    EXPECT_EQ(viewItem.data(Qt::EditRole), expected);
//    EXPECT_EQ(viewItem.data(Qt::DisplayRole), expected);
//    EXPECT_EQ(viewItem.data(Qt::DecorationRole), expected);
//}

//! ViewDataItem::setData for QColor.
//! Checks that the setData method is correctly forwarded to underlying SessionItem.

// TEST_F(StandardViewItemsTest, ViewDataItem_setDataForColor)
//{
//     // create SessionItem with data on board
//     SessionItem item;
//     QVariant expected = QVariant::fromValue(QColor(Qt::green));
//     EXPECT_TRUE(item.setData(expected));

//    // initialize viewItem with sessionItem and set the data
//    ViewDataItem viewItem(&item);
//    QVariant new_data = QVariant::fromValue(QColor(Qt::red));
//    EXPECT_TRUE(viewItem.setData(new_data, Qt::EditRole));
//    EXPECT_EQ(viewItem.data(Qt::DisplayRole), new_data);    // new data
//    EXPECT_EQ(viewItem.data(Qt::EditRole), new_data);       // new data
//    EXPECT_EQ(viewItem.data(Qt::DecorationRole), new_data); // new data

//    // SessionItem itself should have new data
//    EXPECT_EQ(item.data<QVariant>(), new_data); // new data

//    // it is not allowed to set another type of data to ViewDataItem
//    QVariant not_allowed_value("Layer");
//    EXPECT_THROW(viewItem.setData(not_allowed_value, Qt::EditRole), std::runtime_error);
//}

//! Testing tooltip tole.

TEST_F(StandardPresentationItemsTest, DataPresentationItemTooltipRole)
{
  SessionItem item;

  DataPresentationItem presentation(&item);
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(presentation.Data(Qt::ToolTipRole).toString(), QString("abc"));
}

//! Testing color of item depending on enable/disable status.

TEST_F(StandardPresentationItemsTest, DataPresentationItemForegroundRole)
{
  SessionItem item;
  item.SetData(42);

  DataPresentationItem presentation(&item);

  // Enabled item doesn't have valid foreground role.
  // This means that the color of text is left for a view to decide.
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  item.SetEnabled(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));

  item.SetEnabled(true);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  // Hidden item has the same color by default. It is up to the view to decide how to show "hidden"
  // item.

  item.SetVisible(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));
}

//! Testing color of item depending on enable/disable status.

TEST_F(StandardPresentationItemsTest, DataPresentationItemCheckStateRole)
{
  // Item carrying integer doesn't have CheckStateRole
  SessionItem item1;
  item1.SetData(42);
  DataPresentationItem presentation1(&item1);
  EXPECT_FALSE(presentation1.Data(Qt::CheckStateRole).isValid());

  // Item carrying bool has a CheckStateRole
  SessionItem item2;
  item2.SetData(false);
  DataPresentationItem presentation2(&item2);
  EXPECT_TRUE(presentation2.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

 item2.SetData(true);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
}
