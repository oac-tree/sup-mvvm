/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/viewmodel/viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/standard_row_strategies.h>
#include <mvvm/providers/viewitem.h>
#include <mvvm/providers/viewmodel_controller_factory.h>

#include <gtest/gtest.h>

#include <QColor>
#include <QSignalSpy>
#include <memory>

namespace mvvm::test
{

/**
 * @brief Tests ViewModel class.
 *
 * This are tests for ViewModel API only. Full functionality is covered in all details in
 * all_items_viewmodel.test.cpp and viewmodel_base_tests.cpp
 */
class ViewModelTest : public ::testing::Test
{
public:
  /**
   * @brief The TestViewModel class is a simple viewmodel for API testing.
   */
  class TestViewModel : public ViewModel
  {
  public:
    explicit TestViewModel(SessionModel* model)
    {
      auto controller =
          factory::CreateController<AllChildrenStrategy, LabelDataRowStrategy>(model, this);
      SetController(std::move(controller));
    }
  };

  ViewModelTest() { m_model.InsertItem<PropertyItem>(); }

  ApplicationModel m_model;
};

TEST_F(ViewModelTest, InitialState)
{
  ViewModel view_model;
  EXPECT_EQ(view_model.GetModel(), nullptr);
  EXPECT_EQ(view_model.rowCount(QModelIndex()), 0);
  EXPECT_EQ(view_model.columnCount(QModelIndex()), 0);
  EXPECT_EQ(view_model.GetRootSessionItem(), nullptr);
  EXPECT_THROW(view_model.GetSessionItemFromIndex(QModelIndex()), RuntimeException);
  EXPECT_EQ(view_model.GetViewItemFromIndex(QModelIndex()), nullptr);

  const SessionItem item;
  EXPECT_EQ(view_model.FindViews(&item), std::vector<const ViewItem*>());
  EXPECT_EQ(view_model.GetIndexOfSessionItem(&item), QModelIndexList());
}

TEST_F(ViewModelTest, GetSessionItem)
{
  TestViewModel view_model(&m_model);
  EXPECT_EQ(view_model.GetRootSessionItem(), m_model.GetRootItem());

  EXPECT_EQ(view_model.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
  EXPECT_EQ(view_model.GetSessionItemFromIndex(view_model.index(0, 0, QModelIndex())),
            m_model.GetRootItem()->GetItem({"", 0}));
}

TEST_F(ViewModelTest, GetViewItemFromIndex)
{
  TestViewModel view_model(&m_model);
  // behavior duplicates QStandardItemModel. I would make it view_model->rootItem()
  EXPECT_EQ(view_model.GetViewItemFromIndex(QModelIndex()), nullptr);
  EXPECT_EQ(view_model.GetViewItemFromIndex(view_model.index(0, 0, QModelIndex())),
            view_model.rootItem()->GetChild(0, 0));
}

TEST_F(ViewModelTest, FindViews)
{
  TestViewModel view_model(&m_model);
  EXPECT_EQ(view_model.FindViews(m_model.GetRootItem()),
            std::vector<const ViewItem*>({view_model.rootItem()}));

  EXPECT_EQ(view_model.FindViews(m_model.GetRootItem()->GetItem({"", 0})),
            std::vector<const ViewItem*>(
                {view_model.rootItem()->GetChild(0, 0), view_model.rootItem()->GetChild(0, 1)}));
}

TEST_F(ViewModelTest, GetIndexOfSessionItem)
{
  const TestViewModel view_model(&m_model);

  // accessing first child under the root item
  const QModelIndex label_index = view_model.index(0, 0, QModelIndex());
  const QModelIndex data_index = view_model.index(0, 1, QModelIndex());

  QModelIndexList expected{label_index, data_index};
  EXPECT_EQ(view_model.GetIndexOfSessionItem(m_model.GetRootItem()->GetItem({"", 0})), expected);

  // FIXME Is this behavior correct? Might be having QModelIndex() in a list is more consistent.
  EXPECT_EQ(view_model.GetIndexOfSessionItem(m_model.GetRootItem()), QModelIndexList());
}

//! Setting a presentation-only role (background) through the view model emits dataChanged, even
//! though the role is not propagated to the underlying SessionItem and there is no controller
//! notification.
TEST_F(ViewModelTest, SetBackgroundRoleEmitsDataChanged)
{
  TestViewModel view_model(&m_model);

  const QModelIndex data_index = view_model.index(0, 1, QModelIndex());

  QSignalSpy spy_data_changed(&view_model, &ViewModelBase::dataChanged);

  EXPECT_TRUE(view_model.setData(data_index, QColor(Qt::yellow), Qt::BackgroundRole));

  // the view model emitted the change itself
  ASSERT_EQ(spy_data_changed.count(), 1);
  const QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), data_index);
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), data_index);
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), QVector<int>({Qt::BackgroundRole}));

  // the role is retrievable from the view model
  EXPECT_EQ(view_model.data(data_index, Qt::BackgroundRole).value<QColor>(), QColor(Qt::yellow));
}

//! Setting the foreground role through the view model emits dataChanged and is not propagated to
//! the underlying SessionItem (no kTextColor role appears on it).
TEST_F(ViewModelTest, SetForegroundRoleIsNotPropagatedToSessionItem)
{
  TestViewModel view_model(&m_model);

  const QModelIndex data_index = view_model.index(0, 1, QModelIndex());
  auto session_item = view_model.GetSessionItemFromIndex(data_index);
  ASSERT_NE(session_item, nullptr);

  QSignalSpy spy_data_changed(&view_model, &ViewModelBase::dataChanged);

  EXPECT_TRUE(view_model.setData(data_index, QColor(Qt::red), Qt::ForegroundRole));

  ASSERT_EQ(spy_data_changed.count(), 1);
  const QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), QVector<int>({Qt::ForegroundRole}));

  EXPECT_EQ(view_model.data(data_index, Qt::ForegroundRole).value<QColor>(), QColor(Qt::red));

  // the foreground role has a SessionItem equivalent (kTextColor), but the presentation-only path
  // must not write it to the SessionItem
  EXPECT_FALSE(session_item->HasData(DataRole::kTextColor));
}

}  // namespace mvvm::test
