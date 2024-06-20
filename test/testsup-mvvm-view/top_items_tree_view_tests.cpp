/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/views/top_items_tree_view.h"

#include <mvvm/model/application_model.h>
#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

//! Testing TopItemsTreeView.

class TopItemsTreeViewTest : public ::testing::Test
{
};

//! Testing SetData after different SetRootSessionItem (real-life bug).

//! Assign property item, then assign nullptr and destroy the model.

TEST_F(TopItemsTreeViewTest, SetNullptrAsItem)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();
  auto vector_item0 = model->InsertItem<VectorItem>();
  vector_item0->SetX(42.0);

  TopItemsTreeView view;
  view.SetApplicationModel(model.get());

  auto viewmodel = view.GetComponentProvider()->GetViewModel();

  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  view.SetApplicationModel(nullptr);
  model.reset();

  ASSERT_EQ(view.GetComponentProvider()->GetViewModel(), viewmodel);
  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 2);
  EXPECT_EQ(viewmodel->GetModel(), nullptr);
}

TEST_F(TopItemsTreeViewTest, DestroyModel)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();

  auto vector_item = model->InsertItem<VectorItem>();
  TopItemsTreeView view(model.get());
  view.SetSelectedItem(vector_item);

  auto viewmodel = view.GetComponentProvider()->GetViewModel();
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  // destroying the model
  model.reset();

  EXPECT_EQ(view.GetComponentProvider()->GetViewModel(), viewmodel);

  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  EXPECT_TRUE(view.GetComponentProvider()->GetSelectedItems().empty());
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
}

TEST_F(TopItemsTreeViewTest, GetSelectedItems)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();
  auto vector_item0 = model->InsertItem<VectorItem>();
  auto vector_item1 = model->InsertItem<VectorItem>();
  auto vector_item2 = model->InsertItem<VectorItem>();

  TopItemsTreeView view(model.get());

  EXPECT_TRUE(view.GetComponentProvider()->GetSelectedItems().empty());
  EXPECT_EQ(view.GetSelectedItem(), nullptr);

  view.SetSelectedItem(vector_item0);
  EXPECT_EQ(view.GetComponentProvider()->GetSelectedItems(),
            std::vector<SessionItem*>({vector_item0}));
  EXPECT_EQ(view.GetSelectedItem(), vector_item0);

  view.GetComponentProvider()->SetSelectedItems({vector_item0, vector_item1});
  EXPECT_EQ(view.GetComponentProvider()->GetSelectedItems(),
            std::vector<SessionItem*>({vector_item0, vector_item1}));
  EXPECT_EQ(view.GetSelectedItem(), vector_item0);
}
