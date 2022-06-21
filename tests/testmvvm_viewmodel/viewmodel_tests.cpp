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

#include "mvvm/viewmodel/viewmodel.h"

#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include "mvvm/viewmodel/standard_children_strategies.h"
#include "mvvm/viewmodel/standard_row_strategies.h"
#include "mvvm/viewmodel/viewmodel_controller.h"
#include "mvvm/viewmodelbase/viewitem.h"

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

//! Tests ViewModel class.
//! This are tests for ViewModel API only. Full functionality is covered in all details in
//! defaultviewmodel.test.cpp and viewmodel_base_tests.cpp

class ViewModelTests : public ::testing::Test
{
public:
  //! Simple TestModel to validate ViewModel own API
  class TestViewModel : public ViewModel
  {
  public:
    explicit TestViewModel(SessionModel *model)
    {
      auto controller = std::make_unique<ViewModelController>(model, this);
      controller->SetChildrenStrategy(std::make_unique<AllChildrenStrategy>());
      controller->SetRowStrategy(std::make_unique<LabelDataRowStrategy>());
      SetController(std::move(controller));
    }
  };

  ViewModelTests() { m_model.InsertItem<PropertyItem>(); }

  SessionModel m_model;
};

//! The map is initially empty.

TEST_F(ViewModelTests, InitialState)
{
  ViewModel view_model;
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 0);
  EXPECT_EQ(view_model.GetRootSessionItem(), nullptr);
  EXPECT_THROW(view_model.GetSessionItemFromIndex(QModelIndex()), std::runtime_error);
  EXPECT_EQ(view_model.GetViewItemFromIndex(QModelIndex()), nullptr);

  SessionItem item;
  EXPECT_EQ(view_model.FindViews(&item), std::vector<ViewItem *>());
  EXPECT_EQ(view_model.GetIndexOfSessionItem(&item), QModelIndexList());
}

TEST_F(ViewModelTests, GetSessionItem)
{
  TestViewModel view_model(&m_model);
  EXPECT_EQ(view_model.GetRootSessionItem(), m_model.GetRootItem());

  EXPECT_EQ(view_model.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
  EXPECT_EQ(view_model.GetSessionItemFromIndex(view_model.index(0, 0)),
            m_model.GetRootItem()->GetItem("", 0));
}

TEST_F(ViewModelTests, GetViewItemFromIndex)
{
  TestViewModel view_model(&m_model);
  // behavior duplicates QStandardItemModel. I would make it view_model->rootItem()
  EXPECT_EQ(view_model.GetViewItemFromIndex(QModelIndex()), nullptr);
  EXPECT_EQ(view_model.GetViewItemFromIndex(view_model.index(0, 0)),
            view_model.rootItem()->child(0, 0));
}

TEST_F(ViewModelTests, FindViews)
{
  TestViewModel view_model(&m_model);
  EXPECT_EQ(view_model.FindViews(m_model.GetRootItem()),
            std::vector<ViewItem *>({view_model.rootItem()}));

  EXPECT_EQ(view_model.FindViews(m_model.GetRootItem()->GetItem("", 0)),
            std::vector<ViewItem *>(
                {view_model.rootItem()->child(0, 0), view_model.rootItem()->child(0, 1)}));
}

TEST_F(ViewModelTests, GetIndexOfSessionItem)
{
  TestViewModel view_model(&m_model);

  // accessing first child under the root item
  QModelIndex labelIndex = view_model.index(0, 0);
  QModelIndex dataIndex = view_model.index(0, 1);

  QModelIndexList expected{labelIndex, dataIndex};
  EXPECT_EQ(view_model.GetIndexOfSessionItem(m_model.GetRootItem()->GetItem("", 0)), expected);

  // FIXME Is this behavior correct? Might be having QModelIndex() in a list is more consistent.
  EXPECT_EQ(view_model.GetIndexOfSessionItem(m_model.GetRootItem()), QModelIndexList());
}
