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

#include "mvvm/widgets/topitemstreeview.h"

#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/allitemstreeview.h"

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace ModelView;

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

  auto viewmodel = view.GetViewModel();

  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  view.SetApplicationModel(nullptr);
  model.reset();

  // checking that underlying model was destroyed
  EXPECT_EQ(view.GetViewModel(), nullptr);
}
