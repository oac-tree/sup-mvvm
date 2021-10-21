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

#include "mvvm/widgets/propertytreeview.h"

#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/allitemstreeview.h"

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace ModelView;

//! Testing PropertyTreeViewTest.

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
      model.SetData(vector_item0->GetItem(VectorItem::P_X), 42.0, DataRole::kData));
}
