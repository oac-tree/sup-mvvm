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

#include "mvvm/viewmodel/viewmodel_iterate_helper.h"

#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

#include <QModelIndexList>
#include <QStandardItemModel>

namespace mvvm::test
{

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

class ViewModelIterateHelperTest : public ::testing::Test
{
public:
  /**
   * @brief The convenience
   */
  struct TestData
  {
    std::unique_ptr<QStandardItemModel> model;
    QStandardItem* item0{nullptr};
    QStandardItem* item1{nullptr};
    QStandardItem* item2{nullptr};
    QStandardItem* item3{nullptr};
  };

  static QList<QStandardItem*> CreateItemRow(const QString& name1, const QString& name2 = {})
  {
    QList<QStandardItem*> result = {new QStandardItem(name1), new QStandardItem(name2)};
    result.at(0)->setData(name1, Qt::EditRole);
    return result;
  }

  /**
   * @brief Creates test model with several branches
   *
   * item0
   *   item1
   *   item2
   *     item3
   */
  static TestData CreateTestData()
  {
    TestData data;
    data.model = std::make_unique<QStandardItemModel>();

    data.item0 = data.model->invisibleRootItem();

    auto row = CreateItemRow("item1");
    data.item1 = row.at(0);
    data.item0->insertRow(0, row);

    row = CreateItemRow("item2");
    data.item2 = row.at(0);
    data.item0->insertRow(1, row);

    row = CreateItemRow("item3");
    data.item3 = row.at(0);
    data.item2->insertRow(0, row);

    return data;
  }
};

TEST_F(ViewModelIterateHelperTest, iterate)
{
  QStandardItemModel model;

  model.setColumnCount(2);
  QStandardItem* parentItem = model.invisibleRootItem();

  auto row1 = GetStandardItems({1, 2});
  parentItem->appendRow(row1);
  row1.at(0)->appendRow(GetStandardItems({3, 4}));

  auto row2 = GetStandardItems({10, 20});
  parentItem->appendRow(row2);

  const std::vector<int> expected = {1, 2, 3, 4, 10, 20};
  std::vector<int> result;

  iterate_model(&model, QModelIndex(),
                [&](const QModelIndex& index)
                {
                  auto item = model.itemFromIndex(index);
                  result.push_back(item->data(Qt::EditRole).value<int>());
                });

  EXPECT_EQ(result, expected);
}

TEST_F(ViewModelIterateHelperTest, IterateFirstColumn)
{
  auto data = std::move(CreateTestData());

  std::vector<QModelIndex> visited_indices;
  auto on_index = [&visited_indices](const auto& index) { visited_indices.push_back(index); };

  IterateFirstColumn(*data.model, QModelIndex(), on_index);

  EXPECT_EQ(QModelIndex{}, data.item0->index());

  const std::vector<QModelIndex> expected_indices = {data.item0->index(), data.item1->index(),
                                                     data.item2->index(), data.item3->index()};

  EXPECT_EQ(visited_indices, expected_indices);
}

}  // namespace mvvm::test
