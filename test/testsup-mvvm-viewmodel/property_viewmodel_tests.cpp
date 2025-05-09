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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/property_viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

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
  EXPECT_EQ(viewModel.columnCount(), 2);
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
  EXPECT_EQ(viewModel.columnCount(), 2);
}

TEST_F(PropertyViewModelTest, PropertyItem)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  auto& property = parent->AddProperty("thickness", 0.0);

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
  EXPECT_EQ(viewModel.columnCount(), 2);

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
  EXPECT_EQ(viewModel.columnCount(), 2);

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
  using namespace mvvm::test::toyitems;

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
  EXPECT_EQ(viewmodel.columnCount(), 2);
}

//! The data is manipulated through the ApplicationModel. Checking that ViewModel emits signals.

TEST_F(PropertyViewModelTest, SetData)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  auto& item = parent->AddProperty("Property", 0.0);

  PropertyViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(parent);

  QSignalSpy spy_data_changed(&viewmodel, &ViewModelBase::dataChanged);

  // modifying data through the composer
  model.SetData(&item, 42.0, DataRole::kData);

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
  auto& item = parent->AddProperty("Property", 0.0);

  PropertyViewModel viewmodel1(&model);
  viewmodel1.SetRootSessionItem(parent);

  PropertyViewModel viewmodel2(&model);
  viewmodel2.SetRootSessionItem(parent);

  QSignalSpy spy_data_changed1(&viewmodel1, &ViewModelBase::dataChanged);
  QSignalSpy spy_data_changed2(&viewmodel2, &ViewModelBase::dataChanged);

  // modifying data through the composer
  viewmodel1.setData(viewmodel1.index(0, 1), 42.0, Qt::EditRole);

  EXPECT_EQ(spy_data_changed1.count(), 1);
  EXPECT_EQ(spy_data_changed2.count(), 1);
}

//! Property editor is looking into the layer.
//! We add particles to the layer and check that no signal is emited, and editor still shows only
//! layer properties (real-life bug).

TEST_F(PropertyViewModelTest, LayerPropertyWhileInsertingParticle)
{
  using namespace mvvm::test::toyitems;

  SampleModel model;
  auto layer = model.InsertItem<LayerItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(layer);

  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), 2);

  QSignalSpy spyAboutInserInsert(&view_model, &ViewModelBase::rowsAboutToBeInserted);
  QSignalSpy spyInsert(&view_model, &ViewModelBase::rowsInserted);
  QSignalSpy spyAboutRemove(&view_model, &ViewModelBase::rowsAboutToBeRemoved);
  QSignalSpy spyRemove(&view_model, &ViewModelBase::rowsRemoved);
  QSignalSpy spyAboutReset(&view_model, &ViewModelBase::modelAboutToBeReset);
  QSignalSpy spyReset(&view_model, &ViewModelBase::modelReset);
  QSignalSpy spyLayout(&view_model, &ViewModelBase::layoutChanged);

  model.InsertItem<ParticleItem>(layer);

  // Inertion of particle shouldn't change signals and properties
  EXPECT_EQ(spyAboutInserInsert.count(), 0);
  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyAboutRemove.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 0);
  EXPECT_EQ(spyReset.count(), 0);

  EXPECT_EQ(view_model.rowCount(), 2);
  EXPECT_EQ(view_model.columnCount(), 2);
}

//! View model is set to look at VectorItem.
//! Then ApplicationModel is reset

TEST_F(PropertyViewModelTest, ShowVectorItemThenClearThenShowAnother)
{
  ApplicationModel model;
  auto vector0 = model.InsertItem<VectorItem>();

  PropertyViewModel view_model(&model);
  view_model.SetRootSessionItem(vector0);

  QSignalSpy spy_about_reset(&view_model, &ViewModelBase::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &ViewModelBase::modelReset);

  EXPECT_EQ(view_model.rowCount(), 3);
  EXPECT_EQ(view_model.columnCount(), 2);

  auto vector_index = QModelIndex();
  auto x_index_label = view_model.index(0, 0, vector_index);
  auto x_index_value = view_model.index(0, 1, vector_index);
  EXPECT_TRUE(view_model.setData(x_index_value, QVariant(42.1), Qt::EditRole));

  model.Clear();

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 2);

  auto vector1 = model.InsertItem<VectorItem>();
  // property model doesn't see vector1 if it is not root
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 2);

  view_model.SetRootSessionItem(vector1);
  EXPECT_EQ(view_model.rowCount(), 3);
  EXPECT_EQ(view_model.columnCount(), 2);

  vector_index = QModelIndex();
  x_index_label = view_model.index(0, 0, vector_index);
  x_index_value = view_model.index(0, 1, vector_index);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(vector_index), vector1);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(x_index_label), vector1->GetItem(VectorItem::kX));
  EXPECT_EQ(view_model.GetSessionItemFromIndex(x_index_value), vector1->GetItem(VectorItem::kX));

  EXPECT_TRUE(view_model.setData(x_index_value, QVariant(43.3), Qt::EditRole));
  EXPECT_DOUBLE_EQ(vector1->X(), 43.3);
}

//! Inserting item in an empty tag which is between property tags. One of property tags is hidden.
//! This is to validate that item inserted in empty tag appears in the right order (real life bug).
//! FIXME make example less verbose after refactoring of AddProperty machinery.

TEST_F(PropertyViewModelTest, InsertIntoEmptyTag)
{
  ApplicationModel model;

  auto parent = model.InsertItem<CompoundItem>();

  // for the moment, AddProperty doesn't trigger signaling
  // so we have to initialise viewmodel later

  auto& height_property = parent->AddProperty("height", 42);
  auto& hidden_property = parent->AddProperty("name", "abc").SetVisible(false);
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto& width_property = parent->AddProperty("width", 0);

  PropertyViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(parent);

  QSignalSpy spy_insert(&viewmodel, &ViewModelBase::rowsInserted);

  EXPECT_EQ(parent->GetAllItems(),
            std::vector<SessionItem*>({&height_property, &hidden_property, &width_property}));

  // we construct an item which is mimiking a dynamic property item
  // will be refactored
  auto child = std::make_unique<SessionItem>();
  child->SetFlag(Appearance::kProperty, true);
  auto child_ptr = model.InsertItem(std::move(child), parent, {"ITEMS", 0});
  EXPECT_EQ(parent->GetAllItems(), std::vector<SessionItem*>({&height_property, &hidden_property,
                                                              child_ptr, &width_property}));

  EXPECT_EQ(spy_insert.count(), 1);

  auto parent_index = QModelIndex();               // our parent is a new root
  EXPECT_EQ(viewmodel.rowCount(parent_index), 3);  // hidden item is not here
  EXPECT_EQ(viewmodel.columnCount(parent_index), 2);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(parent_index), parent);

  auto child_index0 = viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_index0), &height_property);

  auto child_index1 = viewmodel.index(1, 0, parent_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_index1), child_ptr);

  auto child_index2 = viewmodel.index(2, 0, parent_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_index2), &width_property);
}
