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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodelbase/viewmodel_base_utils.h"

#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

#include <QModelIndexList>
#include <QStandardItemModel>

namespace
{
QList<QStandardItem*> GetStandardItems(const std::vector<int>& data)
{
  QList<QStandardItem*> result;

  for (auto x : data)
  {
    result.append(new QStandardItem(QString::number(x)));
  }

  return result;
}
}  // namespace

using namespace mvvm;

class ViewModelBaseUtilsTest : public ::testing::Test
{
};

//! Validate Utils::iterate_model function with user callback.

TEST_F(ViewModelBaseUtilsTest, iterate)
{
  QStandardItemModel model;

  model.setColumnCount(2);
  QStandardItem* parentItem = model.invisibleRootItem();

  auto row1 = GetStandardItems({1, 2});
  parentItem->appendRow(row1);
  row1.at(0)->appendRow(GetStandardItems({3, 4}));

  auto row2 = GetStandardItems({10, 20});
  parentItem->appendRow(row2);

  std::vector<int> expected = {1, 2, 3, 4, 10, 20};
  std::vector<int> result;

  utils::iterate_model(&model, QModelIndex(),
                       [&](const QModelIndex& index)
                       {
                         auto item = model.itemFromIndex(index);
                         result.push_back(item->data(Qt::EditRole).value<int>());
                       });

  EXPECT_EQ(result, expected);
}
