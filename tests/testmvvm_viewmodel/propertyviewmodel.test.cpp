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

#include "mvvm/viewmodel/propertyviewmodel.h"

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/standarditems/vectoritem.h"
#include "mvvm/model/applicationmodel.h"

#include <gtest/gtest.h>
#include "toyitems.h"

#include <QSignalSpy>

using namespace mvvm;

//! Tests for PropertyViewModel class.

class PropertyViewModelTest : public ::testing::Test
{
};

TEST_F(PropertyViewModelTest, InitialState)
{
  ApplicationModel model;
  PropertyViewModel viewModel(&model);
  EXPECT_EQ(viewModel.rowCount(), 0);
  EXPECT_EQ(viewModel.columnCount(), 0);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(QModelIndex()), model.GetRootItem());
}

TEST_F(PropertyViewModelTest, BaseItem)
{
  ApplicationModel model;
  model.InsertItem<SessionItem>();

  PropertyViewModel viewModel(&model);

  // Root item has default tag and all items considered as top items.
  // PropertyViewModel shouldn't see any items.
  EXPECT_EQ(viewModel.rowCount(), 0);
  EXPECT_EQ(viewModel.columnCount(), 0);
}

TEST_F(PropertyViewModelTest, PropertyItem)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  auto property = parent->AddProperty("thickness", 0.0);

  parent->RegisterTag(TagInfo::CreateUniversalTag("universal_tag"));

  model.InsertItem<SessionItem>(parent, "universal_tag");

  PropertyViewModel viewModel(&model);
  viewModel.SetRootSessionItem(parent);

  // View model should see only property item belonging to parent.
  EXPECT_EQ(viewModel.rowCount(), 1);
  EXPECT_EQ(viewModel.columnCount(), 2);
}

//! VectorItem in a model.

TEST_F(PropertyViewModelTest, VectorItem)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();

  PropertyViewModel viewModel(&model);

  EXPECT_EQ(viewModel.rowCount(), 0);  // root item doesn't have properties
  EXPECT_EQ(viewModel.columnCount(), 0);

  // switching to vectorItem and checking that it has 3 properties
  viewModel.SetRootSessionItem(parent);
  EXPECT_EQ(viewModel.rowCount(), 3);
  EXPECT_EQ(viewModel.columnCount(), 2);
}

//! VectorItem in with hidden component. An item with setVisible(false) set shouldn't appear in a
//! view model. The current implementation is limited and respects this flag only if it was set
//! before the view model creation.

TEST_F(PropertyViewModelTest, VectorItemWithHiddenComponent)
{
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();
  vector_item->GetItem(VectorItem::kY)->SetVisible(false);

  PropertyViewModel viewModel(&model);

  EXPECT_EQ(viewModel.rowCount(), 0);  // root item doesn't have properties
  EXPECT_EQ(viewModel.columnCount(), 0);

  // switching to vectorItem and checking that it has only 2 properties (y-item was skipped).
  viewModel.SetRootSessionItem(vector_item);
  EXPECT_EQ(viewModel.rowCount(), 2);
  EXPECT_EQ(viewModel.columnCount(), 2);

  auto vector_index = QModelIndex();
  auto x_index = viewModel.index(0, 0, vector_index);
  auto z_index = viewModel.index(1, 0, vector_index);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(vector_index), vector_item);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(x_index), vector_item->GetItem(VectorItem::kX));
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(z_index), vector_item->GetItem(VectorItem::kZ));

  // attempt to make P_Y visible again
  vector_item->GetItem(VectorItem::kY)->SetVisible(true);

  // The current implementation is limited: PropertyViewModel doesn't listen for updates
  // in isVisible flag. If flag is changed after the model creation, item still be invisible.

  vector_index = QModelIndex();
  x_index = viewModel.index(0, 0, vector_index);
  z_index = viewModel.index(1, 0, vector_index);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(vector_index), vector_item);
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(x_index), vector_item->GetItem(VectorItem::kX));
  EXPECT_EQ(viewModel.GetSessionItemFromIndex(z_index), vector_item->GetItem(VectorItem::kZ));
}

//! LayerItem in a MultiLayer.

TEST_F(PropertyViewModelTest, LayerInMultiLayerAsRootItem)
{
  using namespace testutils::toyitems;

  SampleModel model;
  auto multilayer = model.InsertItem<MultiLayerItem>();
  auto layer = model.InsertItem<LayerItem>(multilayer);

  PropertyViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(layer);

  // check layer thickness and color
  EXPECT_EQ(viewmodel.rowCount(), 2);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // remove multilayer
  model.RemoveItem(multilayer);

  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 0);
}

//! The data is manipulated through the ApplicationModel. Checking that ViewModel emits signals.

TEST_F(PropertyViewModelTest, SetData)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  auto item = parent->AddProperty("Property", 0.0);

  PropertyViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(parent);

  QSignalSpy spy_data_changed(&viewmodel, &ViewModelBase::dataChanged);

  // modifying data through the composer
  model.SetData(item, 42.0, DataRole::kData);

  ASSERT_EQ(spy_data_changed.count(), 1);

  QModelIndex data_index = viewmodel.index(0, 1);

  QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), viewmodel.index(0, 1));
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), viewmodel.index(0, 1));
  QVector<int> expectedRoles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expectedRoles);

  EXPECT_EQ(viewmodel.data(data_index, Qt::EditRole).toDouble(), 42.0);
}

//! Two ViewModels are looking to the same ApplicationModel.
//! Change through one ViewModel should modify another.

TEST_F(PropertyViewModelTest, SetDataThroughTwoModels)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  auto item = parent->AddProperty("Property", 0.0);

  PropertyViewModel viewmodel1(&model);
  viewmodel1.SetRootSessionItem(parent);

  PropertyViewModel viewmodel2(&model);
  viewmodel2.SetRootSessionItem(parent);

  QSignalSpy spy_data_changed1(&viewmodel1, &ViewModelBase::dataChanged);
  QSignalSpy spy_data_changed2(&viewmodel2, &ViewModelBase::dataChanged);

  // modifying data through the composer
  viewmodel1.setData(viewmodel1.index(0, 1), 42.0, Qt::EditRole);

  EXPECT_EQ(spy_data_changed1.count(), 2);  // FIXME should be 1
  EXPECT_EQ(spy_data_changed2.count(), 1);
}
