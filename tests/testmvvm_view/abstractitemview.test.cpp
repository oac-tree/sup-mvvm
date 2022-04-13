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

#include "mvvm/widgets/abstractitemview.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

class AbstractItemViewTest : public ::testing::Test
{
public:
  class TestView : public mvvm::AbstractItemView
  {
  public:
    explicit TestView(mvvm::ApplicationModel* model)
        : AbstractItemView(CreateViewModel<mvvm::TopItemsViewModel>, new QTreeView, model)
    {
    }
  };

  ApplicationModel m_model;
};

TEST_F(AbstractItemViewTest, InitialState)
{
  TestView view(nullptr);
  EXPECT_EQ(view.GetViewModel(), nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
  EXPECT_TRUE(view.GetSelectedItems().empty());
}

TEST_F(AbstractItemViewTest, ModelInConstructor)
{
  TestView view(&m_model);
  auto viewmodel = view.GetViewModel();
  EXPECT_NE(viewmodel, nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);

  EXPECT_EQ(viewmodel->GetRootSessionItem(), m_model.GetRootItem());
  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 0);
}

TEST_F(AbstractItemViewTest, SetApplicationModel)
{
  TestView view(nullptr);

  auto item = m_model.InsertItem<CompoundItem>();
  item->SetData(42);

  view.SetApplicationModel(&m_model);

  auto viewmodel = view.GetViewModel();

  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
  EXPECT_EQ(viewmodel->GetRootSessionItem(), m_model.GetRootItem());

  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);
}

TEST_F(AbstractItemViewTest, SetItem)
{
  TestView view(nullptr);

  auto item = m_model.InsertItem<CompoundItem>();
  item->SetData(42);

  view.SetItem(item);
  auto viewmodel = view.GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
  EXPECT_EQ(view.GetViewModel()->GetRootSessionItem(), item);

  // no rows and columns since our item plays the role of root item
  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 0);
}
