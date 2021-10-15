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

#include "mvvm/widgets/allitemstreeview.h"

#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/allitemstreeview.h"

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QTreeView>

using namespace ModelView;

//! Testing AllItemsTreeView.

class AllItemsTreeViewTest : public ::testing::Test
{
};

//! Testing root item change, when one of the item is selected (real life bug).

TEST_F(AllItemsTreeViewTest, initialState)
{
  // setting up model and viewmodel
  ApplicationModel model;
  auto vectorItem = model.InsertItem<VectorItem>();
  auto xItem = vectorItem->GetItem(VectorItem::P_X);
  AllItemsTreeView view(&model);
  view.SetRootSessionItem(vectorItem);

  // access to internals
  auto selectionModel = view.GetTreeView()->selectionModel();
  auto viewModel = view.GetViewModel();

  // selecting item in a widget
  selectionModel->select(viewModel->GetIndexOfSessionItem(xItem).front(),
                         QItemSelectionModel::SelectCurrent);

  // Changing root item. The problem was chain of signals (AboutToReset, RowIserted), which
  // was triggering persistentModelIndex.
  ASSERT_NO_FATAL_FAILURE(view.SetRootSessionItem(model.GetRootItem()));
}
