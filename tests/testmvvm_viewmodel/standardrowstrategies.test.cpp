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
#include "mvvm/viewmodelbase/viewitem.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

#include <gtest/gtest.h>

namespace
{
const int expected_column_count = 2;
const QStringList expected_labels = QStringList() << "Name"
                                                  << "Value";
}  // namespace

using namespace ModelView;

class LabelDataRowStrategyTest : public ::testing::Test
{
};

TEST_F(LabelDataRowStrategyTest, InitialState)
{
  LabelDataRowStrategy constructor;
  EXPECT_EQ(constructor.ConstructRow(nullptr).size(), 0);
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);
}

//! Checks row construction for standard top level item, like Level, MultiLayer etc.

TEST_F(LabelDataRowStrategyTest, TopLevelItem)
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
  EXPECT_EQ(Utils::GetContext<SessionItem>(view_item), &item);

  view_item = items.at(1).get();
  EXPECT_FALSE(view_item->data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());
}

//! Checks row construction for property item.

TEST_F(LabelDataRowStrategyTest, PropertyItem)
{
  SessionItem item;
  std::string expected_name{"test_name"};
  item.SetData(expected_name);

  LabelDataRowStrategy constructor;
  auto items = constructor.ConstructRow(&item);
  EXPECT_EQ(items.size(), expected_column_count);
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);

  // first cell with Type is readonly
  auto view_item = items.at(0).get();
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), item.GetType());
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());
  EXPECT_FALSE(view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(view_item->setData(QString("bbb"), Qt::EditRole));

  // second cell has editable name
  view_item = items.at(1).get();
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(view_item->data(Qt::EditRole).toString().toStdString(), expected_name);
  // by current convention it is not possible to change display role
  EXPECT_FALSE(view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(view_item->data(Qt::EditRole).toString().toStdString(), expected_name);
  // but it is possible to set a new name, hen editable name and display name changes
  EXPECT_TRUE(view_item->setData(QString("bbb"), Qt::EditRole));
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), "bbb");
  EXPECT_EQ(view_item->data(Qt::EditRole).toString().toStdString(), "bbb");
}
