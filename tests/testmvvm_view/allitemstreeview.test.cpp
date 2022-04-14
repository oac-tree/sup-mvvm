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

#include "mvvm/widgets/allitemstreeview.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

//! Testing AllItemsTreeView.

class AllItemsTreeViewTest : public ::testing::Test
{
};

//! Testing root item change, when one of the item is selected (real life bug).

TEST_F(AllItemsTreeViewTest, ChangeRootItemWhenSelected)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();
  auto x_item = vector_item->GetItem(VectorItem::kX);
  AllItemsTreeView view(&model);
  view.SetRootSessionItem(vector_item);

  // access to internals
  auto selection_model = view.GetView()->selectionModel();
  auto view_model = view.GetViewModel();

  // selecting item in a widget
  selection_model->select(view_model->GetIndexOfSessionItem(x_item).front(),
                          QItemSelectionModel::SelectCurrent);

  // Changing root item. The problem was chain of signals (AboutToReset, RowIserted), which
  // was triggering persistentModelIndex.
  ASSERT_NO_FATAL_FAILURE(view.SetRootSessionItem(model.GetRootItem()));
}

TEST_F(AllItemsTreeViewTest, GetSelectedItemsWithNoModel)
{
  AllItemsTreeView view;
  EXPECT_TRUE(view.GetSelectedItems().empty());
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
}

TEST_F(AllItemsTreeViewTest, GetSelectedItems)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();
  auto x_item = vector_item->GetItem(VectorItem::kX);
  auto y_item = vector_item->GetItem(VectorItem::kY);
  AllItemsTreeView view(&model);
  view.SetRootSessionItem(vector_item);

  EXPECT_TRUE(view.GetSelectedItems().empty());
  EXPECT_EQ(view.GetSelectedItem(), nullptr);

  view.SetSelectedItem(x_item);
  EXPECT_EQ(view.GetSelectedItems(), std::vector<SessionItem*>({x_item}));
  EXPECT_EQ(view.GetSelectedItem(), x_item);

  view.SetSelectedItem(y_item);
  EXPECT_EQ(view.GetSelectedItems(), std::vector<SessionItem*>({y_item}));
  EXPECT_EQ(view.GetSelectedItem(), y_item);

  view.SetSelectedItems({x_item, y_item});
  EXPECT_EQ(view.GetSelectedItems(), std::vector<SessionItem*>({x_item, y_item}));
  EXPECT_EQ(view.GetSelectedItem(), x_item);
}

//! Selecting whole row and checking list of selected items.
//! There should be no duplications.

TEST_F(AllItemsTreeViewTest, SelectRow)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();

  auto x_item = vector_item->GetItem(VectorItem::kX);

  AllItemsTreeView view(&model);
  view.SetRootSessionItem(vector_item);

  auto x_item_index = view.GetViewModel()->index(0, 1);

  QSignalSpy spy_selected(&view, &AllItemsTreeView::SelectedItemChanged);

  // selecting row where xItem is located
  view.GetView()->selectionModel()->select(
      x_item_index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  EXPECT_EQ(view.GetSelectedItems(), std::vector<SessionItem*>({x_item}));
  EXPECT_EQ(spy_selected.count(), 1);
}

TEST_F(AllItemsTreeViewTest, DestroyModel)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();

  auto vector_item = model->InsertItem<VectorItem>();
  AllItemsTreeView view(model.get());
  view.SetSelectedItem(vector_item);

  auto viewmodel = view.GetViewModel();
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  // destroying the model
  model.reset();

  EXPECT_EQ(view.GetViewModel(), viewmodel);

  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 0);

  EXPECT_TRUE(view.GetSelectedItems().empty());
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
}

//! Removing selected and checking notifications

TEST_F(AllItemsTreeViewTest, SelectionAfterRemoval)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto property0 = model.InsertItem<PropertyItem>();

  AllItemsTreeView view(&model);

  QSignalSpy spy_selected(&view, &AllItemsTreeView::SelectedItemChanged);

  // selecting single item
  view.SetSelectedItem(property0);

  // checking selections
  EXPECT_EQ(view.GetSelectedItems(), std::vector<SessionItem*>({property0}));

  // checking signaling
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto item = arguments.at(0).value<SessionItem*>();
  EXPECT_EQ(item, property0);

  spy_selected.clear();

  // removing item
  model.RemoveItem(property0);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(spy_selected.count(), 1);
  arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  item = arguments.at(0).value<SessionItem*>();
  EXPECT_EQ(item, nullptr);
}

TEST_F(AllItemsTreeViewTest, SetNullptrAsModel)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();

  auto vector_item = model->InsertItem<VectorItem>();
  AllItemsTreeView view(model.get());

  auto viewmodel = view.GetViewModel();
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  view.SetApplicationModel(nullptr);

  EXPECT_EQ(view.GetViewModel(), nullptr);
}
