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

#include "mvvm/viewmodel/standardrowstrategies.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/viewmodelutils.h"
#include "mvvm/viewmodelbase/viewitem.h"

#include <gtest/gtest.h>

namespace
{
const int expected_column_count = 2;
const QStringList expected_labels = QStringList() << "Name"
                                                  << "Value";
}  // namespace

using namespace mvvm;

class StandardRowStrategiesTest : public ::testing::Test
{
};

TEST_F(StandardRowStrategiesTest, LabelDataRowStrategyInitialState)
{
  LabelDataRowStrategy constructor;
  EXPECT_EQ(constructor.ConstructRow(nullptr).size(), 0);
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);
}

//! Checks row construction for standard top level item, like Level, MultiLayer etc.

TEST_F(StandardRowStrategiesTest, LabelDataRowStrategyRowForTopLevelItem)
{
  SessionItem item;

  LabelDataRowStrategy constructor;
  auto items = constructor.ConstructRow(&item);
  EXPECT_EQ(items.size(), expected_column_count);  // label and empty items
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);

  // non editable display name
  auto view_item = items.at(0).get();
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), item.GetType());
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());
  EXPECT_FALSE(view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(view_item->setData(QString("bbb"), Qt::EditRole));
  EXPECT_EQ(utils::GetItemFromView<SessionItem>(view_item), &item);

  view_item = items.at(1).get();
  EXPECT_FALSE(view_item->data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());
}

//! Checks row construction for property item.

TEST_F(StandardRowStrategiesTest, LabelDataRowStrategyRowForPropertyItem)
{
  SessionItem item;
  std::string expected_name{"test_name"};
  item.SetData(expected_name);

  LabelDataRowStrategy constructor;
  auto items = constructor.ConstructRow(&item);
  EXPECT_EQ(items.size(), expected_column_count);
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);

  // first cell with Type is readonly
  auto label_view_item = items.at(0).get();
  EXPECT_EQ(label_view_item->data(Qt::DisplayRole).toString().toStdString(), item.GetType());
  EXPECT_FALSE(label_view_item->data(Qt::EditRole).isValid());
  EXPECT_FALSE(label_view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(label_view_item->setData(QString("bbb"), Qt::EditRole));

  // second cell has editable name
  auto data_view_item = items.at(1).get();
  EXPECT_EQ(data_view_item->data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(data_view_item->data(Qt::EditRole).toString().toStdString(), expected_name);
  // by current convention it is not possible to change display role
  EXPECT_FALSE(data_view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_EQ(data_view_item->data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(data_view_item->data(Qt::EditRole).toString().toStdString(), expected_name);
  // but it is possible to set a new data, then editable data and display data changes
  EXPECT_TRUE(data_view_item->setData(QString("bbb"), Qt::EditRole));
  EXPECT_EQ(data_view_item->data(Qt::DisplayRole).toString().toStdString(), "bbb");
  EXPECT_EQ(data_view_item->data(Qt::EditRole).toString().toStdString(), "bbb");
}
