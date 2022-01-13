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

#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

#include "mvvm/standarditems/vectoritem.h"

#include <gtest/gtest.h>

#include <QModelIndexList>
#include <QStandardItemModel>

namespace
{
QList<QStandardItem*> GetStandardItems(const std::vector<int>& data)
{
  QList<QStandardItem*> result;

  for (auto x : data)
    result.append(new QStandardItem(QString::number(x)));

  return result;
}
}  // namespace

using namespace mvvm;

class ViewModelBaseUtilsTest : public ::testing::Test
{
public:
  class TestItem
  {
  };
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

  Utils::iterate_model(&model, QModelIndex(),
                       [&](const QModelIndex& index)
                       {
                         auto item = model.itemFromIndex(index);
                         result.push_back(item->data(Qt::EditRole).value<int>());
                       });

  EXPECT_EQ(result, expected);
}

//! Validate Utils::GetPresentaiton function.

TEST_F(ViewModelBaseUtilsTest, GetPresentation)
{
  TestItem wait;
  auto presentation = std::make_unique<PresentationItem<TestItem>>(&wait);

  auto expected_ptr = presentation.get();
  mvvm::ViewItem item(std::move(presentation));
  EXPECT_EQ(Utils::GetPresentation<TestItem>(&item), expected_ptr);
}

//! Validate Utils::GetPresentation function.

TEST_F(ViewModelBaseUtilsTest, GetContext)
{
  TestItem wait;

  auto presentation = std::make_unique<PresentationItem<TestItem>>(&wait);
  mvvm::ViewItem item(std::move(presentation));
  EXPECT_EQ(Utils::GetContext<TestItem>(&item), &wait);

  // Context of the arbitrary item should be nullptr
  mvvm::ViewItem item2;
  EXPECT_EQ(Utils::GetContext<TestItem>(&item2), nullptr);
}

//! Validate Utils::GetItem
//! FIXME refactor GetContext and GetItem method, they are errorprone
//! The result depend on the way PresentationItem has been created

TEST_F(ViewModelBaseUtilsTest, GetItem)
{
  VectorItem item;

  auto presentation1 = std::make_unique<PresentationItem<SessionItem>>(&item);
  mvvm::ViewItem view_item1(std::move(presentation1));

  EXPECT_EQ(Utils::GetContext<SessionItem>(&view_item1), &item);
  EXPECT_NE(Utils::GetContext<VectorItem>(&view_item1), &item);
  EXPECT_EQ(Utils::GetItem<VectorItem>(&view_item1), &item);

  auto presentation2 = std::make_unique<PresentationItem<VectorItem>>(&item);
  mvvm::ViewItem view_item2(std::move(presentation2));

  EXPECT_NE(Utils::GetContext<SessionItem>(&view_item2), &item);
  EXPECT_EQ(Utils::GetContext<VectorItem>(&view_item2), &item);
  EXPECT_NE(Utils::GetItem<VectorItem>(&view_item2), &item);
}

//! Validate Utils::FindViews function.

TEST_F(ViewModelBaseUtilsTest, FindViews)
{
  mvvm::ViewModelBase viewmodel;
  TestItem instruction;

  EXPECT_TRUE(Utils::FindViews<TestItem>(&viewmodel, &instruction).empty());

  EXPECT_TRUE(Utils::FindViews<TestItem>(&viewmodel, nullptr).empty());

  // more tests for FindViews in ViewModelControllerTest
}

//! Tests of CreateLabelPresentation.

TEST_F(ViewModelBaseUtilsTest, CreateLabelPresentation)
{
  TestItem item;

  std::string expected_label{"SomeLabel"};
  auto presentation = mvvm::Utils::CreateLabelPresentation(&item, expected_label);

  // item has a display role, which coincide with the label, the rest is blocked
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), expected_label);
  EXPECT_FALSE(presentation->Data(Qt::EditRole).isValid());

  EXPECT_FALSE(presentation->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(presentation->SetData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(presentation->Data(Qt::DisplayRole).toString().toStdString(), expected_label);
}

//! Tests of CreateLabelPresentation.

TEST_F(ViewModelBaseUtilsTest, CreateLabelViewItem)
{
  TestItem item;

  std::string expected_label{"SomeLabel"};
  auto view_item = mvvm::Utils::CreateLabelViewItem(&item, expected_label);

  // item has a display role, which coincide with the label, the rest is blocked
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), expected_label);
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());

  EXPECT_FALSE(view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(view_item->setData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), expected_label);

  EXPECT_EQ(mvvm::Utils::GetContext<TestItem>(view_item.get()), &item);
}
