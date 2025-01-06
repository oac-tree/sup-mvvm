/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/filter_name_viewmodel.h"

#include <gtest/gtest.h>

#include <QStandardItem>

using namespace mvvm;

//! Tests for FilterNameViewModel class.

class FilterNameViewModelTest : public ::testing::Test
{
public:
  static QList<QStandardItem*> CreateItemRow(const QString& name)
  {
    QList<QStandardItem*> result = {new QStandardItem("a")};
    result.at(0)->setData(name, Qt::DisplayRole);
    return result;
  }
};

TEST_F(FilterNameViewModelTest, SimplePattern)
{
  QStandardItemModel source_view_model;
  auto parent_item = source_view_model.invisibleRootItem();
  parent_item->insertRow(0, CreateItemRow("A"));
  parent_item->insertRow(1, CreateItemRow("AB"));
  parent_item->insertRow(2, CreateItemRow("ABC"));

  FilterNameViewModel proxy;
  proxy.setSourceModel(&source_view_model);
  proxy.SetPattern("AB");

  EXPECT_EQ(proxy.rowCount(), 2);
  EXPECT_EQ(proxy.data(proxy.index(0, 0, QModelIndex()), Qt::DisplayRole).toString(),
            QString("AB"));
  EXPECT_EQ(proxy.data(proxy.index(1, 0, QModelIndex()), Qt::DisplayRole).toString(),
            QString("ABC"));
}
