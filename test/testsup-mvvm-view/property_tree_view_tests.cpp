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

#include "mvvm/views/property_tree_view.h"

#include <mvvm/model/application_model.h>
#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

//! Testing PropertyTreeView.

class PropertyTreeViewTest : public ::testing::Test
{
};

//! Testing SetData after different SetRootSessionItem (real-life bug).

TEST_F(PropertyTreeViewTest, ChangeRootAndSetData)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto vector_item0 = model.InsertItem<VectorItem>();
  auto vector_item1 = model.InsertItem<VectorItem>();

  PropertyTreeView view;
  view.SetItem(vector_item0);

  // Setting second time different item. It will lead to the ViewModel recreation.
  // All controllers should be correctly unsubscribed.
  view.SetItem(vector_item1);

  ASSERT_NO_FATAL_FAILURE(
      model.SetData(vector_item0->GetItem(VectorItem::kX), 42.0, DataRole::kData));
}

//! Assign property item, then assign nullptr and destroy the model.

TEST_F(PropertyTreeViewTest, SetNullptrAsItem)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();
  auto vector_item0 = model->InsertItem<VectorItem>();
  vector_item0->SetX(42.0);

  PropertyTreeView view;
  view.SetItem(vector_item0);

  auto viewmodel = view.GetComponentProvider()->GetViewModel();

  EXPECT_EQ(viewmodel->rowCount(), 3);
  EXPECT_EQ(viewmodel->columnCount(), 2);
  auto x_item_data_index = viewmodel->index(0, 1, QModelIndex());
  EXPECT_EQ(viewmodel->data(x_item_data_index).toDouble(), 42.0);

  view.SetItem(nullptr);
  model.reset();

  ASSERT_EQ(view.GetComponentProvider()->GetViewModel(), viewmodel);
  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 2);
  EXPECT_EQ(viewmodel->GetModel(), nullptr);
}

TEST_F(PropertyTreeViewTest, DestroyModel)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();

  auto vector_item = model->InsertItem<VectorItem>();
  PropertyTreeView view;
  view.SetItem(vector_item);
  view.SetSelectedItem(vector_item);

  auto viewmodel = view.GetComponentProvider()->GetViewModel();
  EXPECT_EQ(viewmodel->rowCount(), 3);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  // destroying the model
  model.reset();

  EXPECT_EQ(view.GetComponentProvider()->GetViewModel(), viewmodel);

  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  EXPECT_TRUE(view.GetComponentProvider()->GetSelectedItems().empty());
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
}
