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

#include "mvvm/views/all_items_tree_view.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/standard_item_includes.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/views/item_view_component_provider.h>

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

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
  view.SetItem(vector_item);

  // access to internals
  auto selection_model = view.GetTreeView()->selectionModel();
  auto provider = view.GetComponentProvider();
  auto view_model = provider->GetViewModel();

  // selecting item in a widget
  selection_model->select(view_model->GetIndexOfSessionItem(x_item).front(),
                          QItemSelectionModel::SelectCurrent);

  // Changing root item. The problem was chain of signals (AboutToReset, RowIserted), which
  // was triggering persistentModelIndex.
  ASSERT_NO_FATAL_FAILURE(view.SetItem(model.GetRootItem()));
}

TEST_F(AllItemsTreeViewTest, GetSelectedItemsWithNoModel)
{
  AllItemsTreeView view;
  EXPECT_TRUE(view.GetComponentProvider()->GetSelectedItems().empty());
  EXPECT_EQ(view.GetComponentProvider()->GetSelectedItem(), nullptr);
}

TEST_F(AllItemsTreeViewTest, GetSelectedItems)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();
  auto x_item = vector_item->GetItem(VectorItem::kX);
  auto y_item = vector_item->GetItem(VectorItem::kY);
  AllItemsTreeView view(&model);
  view.SetItem(vector_item);

  auto provider = view.GetComponentProvider();

  EXPECT_TRUE(provider->GetSelectedItems().empty());
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);

  view.SetSelectedItem(x_item);
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<SessionItem*>({x_item}));
  EXPECT_EQ(view.GetSelectedItem(), x_item);

  view.SetSelectedItem(y_item);
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<SessionItem*>({y_item}));
  EXPECT_EQ(view.GetSelectedItem(), y_item);

  provider->SetSelectedItems({x_item, y_item});
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<SessionItem*>({x_item, y_item}));
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
  view.SetItem(vector_item);

  auto provider = view.GetComponentProvider();
  auto view_model = provider->GetViewModel();

  auto x_item_index = view_model->index(0, 1);

  QSignalSpy spy_selected(&view, &AllItemsTreeView::SelectedItemChanged);

  // selecting row where xItem is located
  view.GetTreeView()->selectionModel()->select(
      x_item_index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  EXPECT_EQ(provider->GetSelectedItems(), std::vector<SessionItem*>({x_item}));
  EXPECT_EQ(spy_selected.count(), 1);
}

TEST_F(AllItemsTreeViewTest, DestroyModel)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();

  auto vector_item = model->InsertItem<VectorItem>();
  AllItemsTreeView view(model.get());
  view.SetSelectedItem(vector_item);

  auto provider = view.GetComponentProvider();
  auto view_model = provider->GetViewModel();

  EXPECT_EQ(view_model->rowCount(), 1);
  EXPECT_EQ(view_model->columnCount(), 2);

  // destroying the model
  model.reset();

  EXPECT_EQ(provider->GetViewModel(), view_model);

  EXPECT_EQ(view_model->rowCount(), 0);
  EXPECT_EQ(view_model->columnCount(), 2);

  EXPECT_TRUE(provider->GetSelectedItems().empty());
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

  auto provider = view.GetComponentProvider();

  // checking selections
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<SessionItem*>({property0}));

  // checking signaling
  EXPECT_EQ(mvvm::test::GetSendItem<SessionItem*>(spy_selected), property0);

  spy_selected.clear();

  // removing item
  model.RemoveItem(property0);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(mvvm::test::GetSendItem<SessionItem*>(spy_selected), nullptr);
}

TEST_F(AllItemsTreeViewTest, SetNullptrAsModel)
{
  // setting up model and viewmodel
  auto model = std::make_unique<ApplicationModel>();

  auto vector_item = model->InsertItem<VectorItem>();
  AllItemsTreeView view(model.get());

  auto provider = view.GetComponentProvider();
  auto view_model = provider->GetViewModel();

  EXPECT_EQ(view_model->rowCount(), 1);
  EXPECT_EQ(view_model->columnCount(), 2);

  view.SetApplicationModel(nullptr);

  EXPECT_EQ(provider->GetViewModel(), view_model);
  EXPECT_EQ(view_model->rowCount(), 0);
  EXPECT_EQ(view_model->columnCount(), 2);
}
