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

#include "mvvm/viewmodel/all_items_viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/data1d_item.h>
#include <mvvm/standarditems/graph_item.h>
#include <mvvm/standarditems/graph_viewport_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/test/folder_test.h>
#include <mvvm/test/toy_items.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace mvvm;

class AllItemsViewModelTest : public mvvm::test::FolderTest
{
public:
  AllItemsViewModelTest() : FolderTest("test_AllItemsViewModel"), m_viewmodel(&m_model) {}

  ApplicationModel m_model;
  AllItemsViewModel m_viewmodel;
};

TEST_F(AllItemsViewModelTest, InitialState)
{
  AllItemsViewModel viewmodel(&m_model);
  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 2);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
  EXPECT_EQ(viewmodel.GetRootSessionItem(), m_model.GetRootItem());
}

TEST_F(AllItemsViewModelTest, GetSessionItemFromIndex)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42.0);

  AllItemsViewModel view_model(&m_model);
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  // accessing first child under the root item
  QModelIndex label_index = view_model.index(0, 0);
  QModelIndex data_index = view_model.index(0, 1);

  EXPECT_EQ(view_model.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
  EXPECT_EQ(view_model.GetSessionItemFromIndex(label_index), item);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(data_index), item);
}

//! Index from single property item.

TEST_F(AllItemsViewModelTest, GetIndexOfSessionItem)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42.0);

  AllItemsViewModel view_model(&m_model);
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  // accessing first child under the root item
  QModelIndex label_index = view_model.index(0, 0);
  QModelIndex data_index = view_model.index(0, 1);

  QModelIndexList expected{label_index, data_index};
  EXPECT_EQ(view_model.GetIndexOfSessionItem(item), expected);
}

//! Single property item in a model.

TEST_F(AllItemsViewModelTest, ModelWithSingleItem)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42.0);

  AllItemsViewModel viewmodel(&m_model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(label_index), item);

  auto data_index = viewmodel.index(0, 1);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(data_index), item);

  // display role of first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item's data
  EXPECT_EQ(viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from item
  auto view_item_label = viewmodel.itemFromIndex(label_index);
  auto view_item_data = viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(viewmodel.FindViews(item),
            std::vector<const ViewItem*>({view_item_label, view_item_data}));

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
}

//! Hidden property item in a model. Current DefaultViewModel implementation deliberately doesn't
//! respect `hidden` property. Item will be shown as usual, test is identical to the test above.

TEST_F(AllItemsViewModelTest, FromPropertyItemWhenHidden)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42.0);
  item->SetVisible(false);

  AllItemsViewModel viewmodel(&m_model);
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(label_index), item);

  auto data_index = viewmodel.index(0, 1);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(data_index), item);

  // display role of first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item's data
  EXPECT_EQ(viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from item
  auto view_item_label = viewmodel.itemFromIndex(label_index);
  auto view_item_data = viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(viewmodel.FindViews(item),
            std::vector<const ViewItem*>({view_item_label, view_item_data}));

  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
}

//! SessionModel is populated with a VectorItem item. The controller is initialised after.

TEST_F(AllItemsViewModelTest, ModelWithVectorItem)
{
  auto vector_item = m_model.InsertItem<VectorItem>();
  vector_item->SetX(1.0);
  vector_item->SetY(2.0);
  vector_item->SetZ(3.0);

  AllItemsViewModel viewmodel(&m_model);

  // the model contains only one entry
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  // accessing first child under the root item
  QModelIndex vector_label_index = viewmodel.index(0, 0);
  QModelIndex vector_data_index = viewmodel.index(0, 1);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(vector_label_index), vector_item);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(vector_data_index), vector_item);

  std::vector<SessionItem*> children = vector_item->GetAllItems();
  for (int row = 0; row < 3; ++row)
  {  // x, y, z
    QModelIndex child_label_index = viewmodel.index(row, 0, vector_label_index);
    QModelIndex child_data_index = viewmodel.index(row, 1, vector_label_index);
    EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_label_index), children[row]);
    EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_data_index), children[row]);
    EXPECT_EQ(viewmodel.data(child_label_index).toString().toStdString(),
              children[row]->GetDisplayName());
    EXPECT_EQ(viewmodel.data(child_data_index).toDouble(),
              children[row]->Data<double>());  // x,y,z coordinates
  }
}

//! Initialise controller with the empty model. Then insert new item and check that view model
//! hass been updated.

TEST_F(AllItemsViewModelTest, InsertIntoEmptyModel)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(42.0);

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 1);
  EXPECT_EQ(spy_remove.count(), 0);

  auto arguments = spy_insert.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = m_viewmodel.index(0, 0);
  auto view_item_label = m_viewmodel.itemFromIndex(label_index);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(label_index), item);

  auto data_index = m_viewmodel.index(0, 1);
  auto view_item_data = m_viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(data_index), item);

  // display roleof first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(m_viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item'sdata
  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from instruction
  EXPECT_EQ(m_viewmodel.FindViews(item),
            std::vector<const ViewItem*>({view_item_label, view_item_data}));
}

//! Insert three property items in a model, inserted after controller was setup.

TEST_F(AllItemsViewModelTest, InitThenInsertProperties)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_model.InsertItem<PropertyItem>();
  auto item1 = m_model.InsertItem<PropertyItem>();
  auto item2 = m_model.InsertItem<PropertyItem>();

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 3);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(0, 0)), item0);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(1, 0)), item1);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(2, 0)), item2);

  for (int row = 0; row < 3; ++row)
  {
    const auto& arguments = spy_insert.at(row);
    EXPECT_EQ(arguments.size(), 3);
    EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
    EXPECT_EQ(arguments.at(1).value<int>(), row);
    EXPECT_EQ(arguments.at(2).value<int>(), row);
  }
}

//! Inserting property items in reversed order.

TEST_F(AllItemsViewModelTest, InsertInFront)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_model.InsertItem<PropertyItem>();
  auto item1 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), TagIndex::Default(0));

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 2);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 2);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(0, 0)), item1);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(1, 0)), item0);
}

//! Inserting item between two other.

TEST_F(AllItemsViewModelTest, InsertBetween)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), TagIndex::Default(0));
  auto item1 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), TagIndex::Default(1));
  auto item2 =
      m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), TagIndex::Default(1));  // between

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 3);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(0, 0)), item0);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(1, 0)), item2);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(m_viewmodel.index(2, 0)), item1);
}

//! Initialise controller with the empty model. Insert parent and then child into it.

TEST_F(AllItemsViewModelTest, InsertParentAndThenChild)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<SessionItem>(parent);

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 2);
  EXPECT_EQ(spy_remove.count(), 0);

  auto parent_index = m_viewmodel.index(0, 0);
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 1);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(parent_index), parent);

  auto child_index = m_viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(child_index), child);

  const auto& arguments = spy_insert.at(1);
  EXPECT_EQ(arguments.size(), 3);
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), parent_index);
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}

//! Sequence with 3 children. Removing the middle one.

TEST_F(AllItemsViewModelTest, RemoveMiddleChild)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child0 = m_model.InsertItem<SessionItem>(parent, TagIndex::Default(0));
  auto child1 = m_model.InsertItem<SessionItem>(parent, TagIndex::Default(1));
  auto child2 = m_model.InsertItem<SessionItem>(parent, TagIndex::Default(2));

  // one entry (parent)
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  auto parent_index = m_viewmodel.index(0, 0);
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 3);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);

  QSignalSpy spy_insert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  // removing middle child
  m_model.TakeItem(parent, {"", 1});

  // one entry (parent)
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(spy_remove.count(), 1);

  // two remaining children
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 2);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);

  auto child0_index = m_viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(child0_index), child0);

  auto child2_index = m_viewmodel.index(1, 0, parent_index);
  EXPECT_EQ(m_viewmodel.GetSessionItemFromIndex(child2_index), child2);

  QList<QVariant> arguments = spy_remove.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), parent_index);
  EXPECT_EQ(arguments.at(1).value<int>(), 1);
  EXPECT_EQ(arguments.at(2).value<int>(), 1);
}

//! The data is manipulated through the ApplicationModel. Checking that ViewModel emits signals.

TEST_F(AllItemsViewModelTest, SetData)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(0.0);

  QSignalSpy spy_data_changed(&m_viewmodel, &ViewModelBase::dataChanged);

  // modifying data through the composer
  m_model.SetData(item, 42.0, DataRole::kData);

  EXPECT_EQ(spy_data_changed.count(), 1);

  QModelIndex data_index = m_viewmodel.index(0, 1);

  QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), m_viewmodel.index(0, 1));
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), m_viewmodel.index(0, 1));
  QVector<int> expectedRoles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expectedRoles);

  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), 42.0);
}

//! The data is manipulated through the view_model. Checking that the view_model emits the signal,
//! and that the change is propagated through the composer.

TEST_F(AllItemsViewModelTest, SetDataThroughModel)
{
  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(0.0);

  QSignalSpy spy_data_changed(&m_viewmodel, &ViewModelBase::dataChanged);

  auto item_index = m_viewmodel.index(0, 1);

  // modifying data through the composer
  m_viewmodel.setData(item_index, 42.0, Qt::EditRole);

  // FIXME Should be one. Currently reported two times because of first emit from
  // ViewModelBase::setData, and second emit from ViewModelController::OnDataChanged.
  EXPECT_EQ(spy_data_changed.count(), 2);  // should be 1

  QModelIndex data_index = m_viewmodel.index(0, 1);

  QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), m_viewmodel.index(0, 1));
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), m_viewmodel.index(0, 1));
  QVector<int> expectedRoles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expectedRoles);
}

//! Two ViewModels are looking to the same ApplicationModel.
//! Change through one ViewModel should modify another.

TEST_F(AllItemsViewModelTest, SetDataThroughTwoModels)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  item->SetData(0.0);

  AllItemsViewModel viewmodel1(&model);
  AllItemsViewModel viewmodel2(&model);

  QSignalSpy spy_data_changed1(&viewmodel1, &ViewModelBase::dataChanged);
  QSignalSpy spy_data_changed2(&viewmodel2, &ViewModelBase::dataChanged);

  // modifying data through the composer
  viewmodel1.setData(viewmodel1.index(0, 1), 42.0, Qt::EditRole);

  EXPECT_EQ(spy_data_changed1.count(), 2);  // FIXME should be 1
  EXPECT_EQ(spy_data_changed2.count(), 1);
}

//! Single property item in ViewModel with various appearance flags.

TEST_F(AllItemsViewModelTest, PropertyItemAppearance)
{
  ApplicationModel model;

  // default item
  auto item1 = model.InsertItem<PropertyItem>();
  item1->SetData(42.0);

  // disabled item
  auto item2 = model.InsertItem<PropertyItem>();
  item2->SetData(42.0);
  item2->SetEnabled(false);  // our convention: gray color, read only

  // read only item
  auto item3 = model.InsertItem<PropertyItem>();
  item3->SetData(42.0);
  item3->SetEditable(false);  // our convention: normal color, read only

  // making view model
  AllItemsViewModel viewModel(&model);

  // In tests below is important that SessionItem::isEnabled==false means that item is
  // shown in gray color. While QStandardItem::isEnabled means "no interaction".
  // In our case QStandardItem::isEnabled should be always true.

  // ViewLabel and ViewDataItem of item1
  EXPECT_FALSE(viewModel.flags(viewModel.index(0, 0)) & Qt::ItemIsEditable);
  EXPECT_TRUE(viewModel.flags(viewModel.index(0, 1)) & Qt::ItemIsEditable);

  // ViewLabel and ViewDataItem of item2
  EXPECT_FALSE(viewModel.flags(viewModel.index(1, 0)) & Qt::ItemIsEditable);
  EXPECT_TRUE(viewModel.flags(viewModel.index(1, 1)) & Qt::ItemIsEditable);

  // ViewLabel and ViewDataItem of item3
  EXPECT_FALSE(viewModel.flags(viewModel.index(2, 0)) & Qt::ItemIsEditable);
  EXPECT_FALSE(viewModel.flags(viewModel.index(2, 1)) & Qt::ItemIsEditable);
}

//! Signals in ViewModel when property item changes its appearance.

TEST_F(AllItemsViewModelTest, PropertyItemAppearanceChanged)
{
  ApplicationModel model;

  // default item
  auto item = model.InsertItem<PropertyItem>();
  item->SetData(42.0);

  // setting up ViewModel and spying it's dataChanged signals
  AllItemsViewModel viewModel(&model);
  auto labelView = viewModel.itemFromIndex(viewModel.index(0, 0));
  auto dataView = viewModel.itemFromIndex(viewModel.index(0, 1));
  QSignalSpy spyDataChanged(&viewModel, &AllItemsViewModel::dataChanged);

  // Changing item appearance
  item->SetEnabled(false);
  EXPECT_EQ(spyDataChanged.count(), 2);  // change in LabelView and DataView

  // first pack of arguments is related to ViewLabelItem
  QList<QVariant> arguments = spyDataChanged.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  auto index1 = arguments.at(0).value<QModelIndex>();
  auto index2 = arguments.at(1).value<QModelIndex>();
  auto roles = arguments.at(2).value<QVector<int>>();
  EXPECT_EQ(index1, viewModel.indexFromItem(labelView));
  EXPECT_EQ(index2, viewModel.indexFromItem(labelView));
  QVector<int> expected_roles = {Qt::ForegroundRole};
  EXPECT_EQ(roles, expected_roles);

  // second pack of arguments is related to ViewDataItem
  arguments = spyDataChanged.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  index1 = arguments.at(0).value<QModelIndex>();
  index2 = arguments.at(1).value<QModelIndex>();
  roles = arguments.at(2).value<QVector<int>>();
  EXPECT_EQ(index1, viewModel.indexFromItem(dataView));
  EXPECT_EQ(index2, viewModel.indexFromItem(dataView));

  expected_roles = {Qt::ForegroundRole};

  EXPECT_EQ(roles, expected_roles);
}

//! Signals in ViewModel when property item changes its tooltips.

TEST_F(AllItemsViewModelTest, TooltipChanged)
{
  ApplicationModel model;

  // default item
  auto item = model.InsertItem<PropertyItem>();
  item->SetData(42.0);
  item->SetToolTip("abc");

  // setting up ViewModel and spying it's dataChanged signals
  AllItemsViewModel viewModel(&model);
  auto labelView = viewModel.itemFromIndex(viewModel.index(0, 0));
  auto dataView = viewModel.itemFromIndex(viewModel.index(0, 1));

  EXPECT_EQ(viewModel.data(viewModel.index(0, 0), Qt::ToolTipRole).toString(), QString("abc"));
  EXPECT_EQ(viewModel.data(viewModel.index(0, 1), Qt::ToolTipRole).toString(), QString("abc"));

  QSignalSpy spyDataChanged(&viewModel, &AllItemsViewModel::dataChanged);

  // Changing tooltip
  item->SetToolTip("abc2");
  EXPECT_EQ(spyDataChanged.count(), 2);  // change in LabelView and DataView

  // first pack of arguments is related to ViewLabelItem
  QList<QVariant> arguments = spyDataChanged.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  auto index1 = arguments.at(0).value<QModelIndex>();
  auto index2 = arguments.at(1).value<QModelIndex>();
  auto roles = arguments.at(2).value<QVector<int>>();
  EXPECT_EQ(index1, viewModel.indexFromItem(labelView));
  EXPECT_EQ(index2, viewModel.indexFromItem(labelView));
  QVector<int> expected_roles = {Qt::ToolTipRole};
  EXPECT_EQ(roles, expected_roles);

  // second pack of arguments is related to ViewDataItem
  arguments = spyDataChanged.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  index1 = arguments.at(0).value<QModelIndex>();
  index2 = arguments.at(1).value<QModelIndex>();
  roles = arguments.at(2).value<QVector<int>>();
  EXPECT_EQ(index1, viewModel.indexFromItem(dataView));
  EXPECT_EQ(index2, viewModel.indexFromItem(dataView));

  expected_roles = {Qt::ToolTipRole};
  EXPECT_EQ(roles, expected_roles);
}

//! Setting property item as ROOT item.

TEST_F(AllItemsViewModelTest, SetPropertyItemAsRoot)
{
  ApplicationModel model;
  AllItemsViewModel view_model(&model);

  QSignalSpy spy_about_reset(&view_model, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &AllItemsViewModel::modelReset);

  auto item = model.InsertItem<PropertyItem>();
  view_model.SetRootSessionItem(item);

  // new root item doesn't have children
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 2);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(view_model.GetRootSessionItem(), item);
}

//! Setting property item as ROOT item.
//! Same as above, only view model was initialized after.

TEST_F(AllItemsViewModelTest, SetPropertyItemAsRootAfter)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  AllItemsViewModel view_model(&model);
  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);

  QSignalSpy spy_about_reset(&view_model, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &AllItemsViewModel::modelReset);
  QSignalSpy spy_insert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  view_model.SetRootSessionItem(item);

  // new root item doesn't have children
  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 2);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(spy_remove.count(), 0);

  EXPECT_EQ(view_model.GetRootSessionItem(), item);
}

//! Setting top level item as ROOT item (case parent and children).

TEST_F(AllItemsViewModelTest, SetCompoundAsRootItem)
{
  ApplicationModel model;
  AllItemsViewModel view_model(&model);

  auto item = model.InsertItem<CompoundItem>();
  item->AddProperty("thickness", 42.0);
  item->AddProperty<VectorItem>("position");
  item->AddProperty("radius", 43.0);

  QSignalSpy spy_about_reset(&view_model, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &AllItemsViewModel::modelReset);
  QSignalSpy spy_insert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  view_model.SetRootSessionItem(item);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(spy_remove.count(), 0);

  EXPECT_EQ(view_model.rowCount(), 3);
  EXPECT_EQ(view_model.columnCount(), 2);

  EXPECT_EQ(view_model.GetRootSessionItem(), item);

  // checking vector item
  auto index_of_vector_item = view_model.index(1, 0);
  EXPECT_EQ(view_model.rowCount(index_of_vector_item), 3);
  EXPECT_EQ(view_model.columnCount(index_of_vector_item), 2);
}

//! Setting vector item as ROOT item.

TEST_F(AllItemsViewModelTest, SetVectorItemAsRoot)
{
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();

  AllItemsViewModel view_model(&model);
  view_model.SetRootSessionItem(vector_item);

  EXPECT_EQ(view_model.rowCount(), 3);
  EXPECT_EQ(view_model.columnCount(), 2);

  EXPECT_EQ(view_model.GetRootSessionItem(), vector_item);
}

//! Setting nullptr as new item.

TEST_F(AllItemsViewModelTest, SetNullptrAsNewRoot)
{
  ApplicationModel model;
  auto vector_item = model.InsertItem<VectorItem>();

  AllItemsViewModel view_model(&model);
  view_model.SetRootSessionItem(vector_item);

  EXPECT_EQ(view_model.rowCount(), 3);
  EXPECT_EQ(view_model.columnCount(), 2);

  EXPECT_EQ(view_model.GetRootSessionItem(), vector_item);

  QSignalSpy spy_about_reset(&view_model, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &AllItemsViewModel::modelReset);
  QSignalSpy spy_insert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  view_model.SetRootSessionItem(nullptr);
  EXPECT_EQ(view_model.GetRootSessionItem(), nullptr);
  EXPECT_EQ(view_model.GetModel(), nullptr);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(spy_remove.count(), 0);

  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 2);
}

//! Setting the model, and then different model

TEST_F(AllItemsViewModelTest, SetAnotherModel)
{
  ApplicationModel model1;
  ApplicationModel model2;
  auto vector_item = model2.InsertItem<VectorItem>();

  AllItemsViewModel view_model(&model1);
  view_model.SetModel(&model1);

  EXPECT_EQ(view_model.GetModel(), &model1);
  EXPECT_EQ(view_model.GetRootSessionItem(), model1.GetRootItem());

  EXPECT_EQ(view_model.rowCount(), 0);
  EXPECT_EQ(view_model.columnCount(), 2);

  QSignalSpy spy_about_reset(&view_model, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&view_model, &AllItemsViewModel::modelReset);
  QSignalSpy spy_insert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  view_model.SetModel(&model2);
  EXPECT_EQ(view_model.GetModel(), &model2);
  EXPECT_EQ(view_model.GetRootSessionItem(), model2.GetRootItem());

  // modifying original model
  model1.InsertItem<VectorItem>();

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
  // no reaction on modification in previous model
  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(spy_remove.count(), 0);

  EXPECT_EQ(view_model.rowCount(), 1);
  EXPECT_EQ(view_model.columnCount(), 2);
}

//! Inserting two VectorItems. Setting second VectorItem as root item.
//! Removing first VectorItem. ViewModel should remain unchanged, no signals issued.

TEST_F(AllItemsViewModelTest, RemoveItemAboveCustomRootItem)
{
  auto vector_item0 = m_model.InsertItem<VectorItem>();
  auto vector_item1 = m_model.InsertItem<VectorItem>();

  m_viewmodel.SetRootSessionItem(vector_item1);

  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(m_viewmodel.GetRootSessionItem(), vector_item1);

  QSignalSpy spy_about_reset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&m_viewmodel, &AllItemsViewModel::modelReset);
  QSignalSpy spy_about_remove(&m_viewmodel, &AllItemsViewModel::rowsAboutToBeRemoved);
  QSignalSpy spy_remove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);

  m_model.TakeItem(m_model.GetRootItem(), {"", 0});

  EXPECT_EQ(spy_about_reset.count(), 0);
  EXPECT_EQ(spy_reset.count(), 0);
  EXPECT_EQ(spy_about_remove.count(), 0);
  EXPECT_EQ(spy_remove.count(), 0);

  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
}

//! Inserting VectorItem and setting it as root item.
//! Removing VectorItem, model has to reset to empty state.

TEST_F(AllItemsViewModelTest, RemoveCustomRootItem)
{
  auto vector_item = m_model.InsertItem<VectorItem>();

  m_viewmodel.SetRootSessionItem(vector_item);

  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  QSignalSpy spy_about_reset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&m_viewmodel, &AllItemsViewModel::modelReset);
  QSignalSpy spy_about_remove(&m_viewmodel, &AllItemsViewModel::rowsAboutToBeRemoved);
  QSignalSpy spy_remove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);

  // removing vector_item
  m_model.RemoveItem(vector_item);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(m_viewmodel.GetRootSessionItem(), nullptr);
}

//! Inserting grandparent -> parent -> child, setting `parent` as new root item.
//! Removing grandparent. The viewmodel should reset.

TEST_F(AllItemsViewModelTest, RemoveFarAncestor)
{
  auto grandparent = m_model.InsertItem<CompoundItem>();
  grandparent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto parent = m_model.InsertItem<SessionItem>(grandparent);
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child0 = m_model.InsertItem<SessionItem>(parent);
  auto child1 = m_model.InsertItem<SessionItem>(parent);

  m_viewmodel.SetRootSessionItem(parent);

  QSignalSpy spy_about_reset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&m_viewmodel, &AllItemsViewModel::modelReset);
  QSignalSpy spy_about_remove(&m_viewmodel, &AllItemsViewModel::rowsAboutToBeRemoved);
  QSignalSpy spy_remove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);

  // removing grandparent
  m_model.RemoveItem(grandparent);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(m_viewmodel.GetRootSessionItem(), nullptr);
}

//! On model reset.

TEST_F(AllItemsViewModelTest, OnModelReset)
{
  m_model.InsertItem<SessionItem>();
  m_model.InsertItem<SessionItem>();
  m_model.InsertItem<SessionItem>();

  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  QSignalSpy spy_about_reset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&m_viewmodel, &AllItemsViewModel::modelReset);

  m_model.Clear({});
  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
}

//! On model destroyed.

TEST_F(AllItemsViewModelTest, OnModelDestroyed)
{
  auto model = std::make_unique<ApplicationModel>();
  model->InsertItem<SessionItem>();

  AllItemsViewModel viewmodel(model.get());
  EXPECT_EQ(viewmodel.GetModel(), model.get());
  EXPECT_EQ(viewmodel.GetRootSessionItem(), model->GetRootItem());
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 2);

  model.reset();
  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 2);
  EXPECT_EQ(viewmodel.GetModel(), nullptr);
  EXPECT_EQ(viewmodel.GetRootSessionItem(), nullptr);
}

TEST_F(AllItemsViewModelTest, horizontalLabels)
{
  EXPECT_EQ(m_viewmodel.headerData(0, Qt::Horizontal, Qt::DisplayRole).toString(), QString("Name"));
  EXPECT_EQ(m_viewmodel.headerData(1, Qt::Horizontal, Qt::DisplayRole).toString(),
            QString("Value"));
}

//! Testing ViewModel signals while loading data with the help of XML document. Model is empty.

TEST_F(AllItemsViewModelTest, XmlDocumentLoadEmptyModel)
{
  const auto file_path = GetFilePath("XmlDocumentLoadEmptyModel.xml");
  XmlDocument document({&m_model});
  document.Save(file_path);

  QSignalSpy spy_insert(&m_viewmodel, &AllItemsViewModel::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);
  QSignalSpy spy_about_reset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spy_reset(&m_viewmodel, &AllItemsViewModel::modelReset);

  document.Load(file_path);

  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(spy_remove.count(), 0);
  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
}

//! Testing ViewModel signals while loading data with the help of XML document. Model contains item.

TEST_F(AllItemsViewModelTest, XmlDocumentLoadModel)
{
  const auto file_path = GetFilePath("XmlDocumentLoadModel.xml");

  // preparing initial file
  {
    ApplicationModel initial_model;
    auto item = initial_model.InsertItem<PropertyItem>();
    item->SetData(42);

    XmlDocument document({&initial_model});
    document.Save(file_path);
  }

  // loading into our empty model
  XmlDocument document({&m_model});

  QSignalSpy spyInsert(&m_viewmodel, &AllItemsViewModel::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);
  QSignalSpy spyAboutReset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spyReset(&m_viewmodel, &AllItemsViewModel::modelReset);

  document.Load(file_path);

  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 1);
  EXPECT_EQ(spyReset.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
}

//! Testing view model after restoring from XML document.

TEST_F(AllItemsViewModelTest, VectorItemInXmlDocument)
{
  const auto file_path = GetFilePath("VectorItemInXmlDocument.xml");

  // preparing initial file
  {
    ApplicationModel initial_model;
    auto item = initial_model.InsertItem<VectorItem>();

    XmlDocument document({&initial_model});
    document.Save(file_path);
  }

  // loading into our empty model
  XmlDocument document({&m_model});

  QSignalSpy spyInsert(&m_viewmodel, &AllItemsViewModel::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);
  QSignalSpy spyAboutReset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spyReset(&m_viewmodel, &AllItemsViewModel::modelReset);

  document.Load(file_path);

  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 1);
  EXPECT_EQ(spyReset.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
}

//! Testing view model after restoring from XML document.
//! VectorItem is made root item. Test demonstrates that controller is capable
//! to restore old rootSessionItem on onModelReset signal

TEST_F(AllItemsViewModelTest, VectorItemAsRootInXmlDocument)
{
  const auto file_path = GetFilePath("VectorItemAsRootInXmlDocument.xml");

  auto vector_item = m_model.InsertItem<VectorItem>();

  m_viewmodel.SetRootSessionItem(vector_item);
  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
  EXPECT_EQ(m_viewmodel.GetRootSessionItem(), vector_item);

  // saving the document
  XmlDocument document({&m_model});
  document.Save(file_path);

  QSignalSpy spyInsert(&m_viewmodel, &AllItemsViewModel::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);
  QSignalSpy spyAboutReset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spyReset(&m_viewmodel, &AllItemsViewModel::modelReset);

  // Loading the document: will rewrite m_model and trigger m_viewmodel rebuild.
  document.Load(file_path);

  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyRemove.count(), 0);
  EXPECT_EQ(spyAboutReset.count(), 1);
  EXPECT_EQ(spyReset.count(), 1);

  // viewmodel now is looking to vectorItem as root ViewItem
  EXPECT_EQ(m_viewmodel.GetRootSessionItem(),
            m_model.GetRootItem()->GetAllItems().at(0));  // vectorItem

  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);
}

//! Moving item from one parent to another.

TEST_F(AllItemsViewModelTest, MoveItemFromOneParentToAnother)
{
  auto container0 = m_model.InsertItem<ContainerItem>();
  auto property = m_model.InsertItem<PropertyItem>(container0, TagIndex::Append());
  auto container1 = m_model.InsertItem<ContainerItem>();

  EXPECT_EQ(m_viewmodel.rowCount(), 2);
  EXPECT_EQ(m_viewmodel.rowCount(m_viewmodel.index(0, 0)), 1);

  QSignalSpy spyInsert(&m_viewmodel, &AllItemsViewModel::rowsInserted);
  QSignalSpy spyAboutInsert(&m_viewmodel, &AllItemsViewModel::rowsAboutToBeInserted);
  QSignalSpy spyRemove(&m_viewmodel, &AllItemsViewModel::rowsRemoved);
  QSignalSpy spyAboutRemove(&m_viewmodel, &AllItemsViewModel::rowsAboutToBeRemoved);
  QSignalSpy spyAboutReset(&m_viewmodel, &AllItemsViewModel::modelAboutToBeReset);
  QSignalSpy spyReset(&m_viewmodel, &AllItemsViewModel::modelReset);

  m_model.MoveItem(property, container1, TagIndex::Append());

  EXPECT_EQ(spyInsert.count(), 1);
  EXPECT_EQ(spyAboutInsert.count(), 1);
  EXPECT_EQ(spyRemove.count(), 1);
  EXPECT_EQ(spyAboutRemove.count(), 1);
}

//! Real life bug. One container with Data1DItem's, one ViewportItem with single graph.
//! DefaultViewModel is looking on ViewPortItem. Graph is deleted first.

TEST_F(AllItemsViewModelTest, DeleteGraphVromViewport)
{
  ApplicationModel model;

  // creating data container and single Data1DItem in it
  auto data_container = model.InsertItem<ContainerItem>();
  auto data_item = model.InsertItem<Data1DItem>(data_container);
  data_item->SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  data_item->SetValues(std::vector<double>({1.0, 2.0, 3.0}));

  // creating Viewport with single graph
  auto viewport_item = model.InsertItem<GraphViewportItem>();
  auto graph_item = model.InsertItem<GraphItem>(viewport_item);
  graph_item->SetDataItem(data_item);

  AllItemsViewModel viewmodel(&model);
  viewmodel.SetRootSessionItem(viewport_item);

  // validating that we see graph at propeer index
  EXPECT_EQ(viewmodel.rowCount(), 3);  // X, Y and Graph
  QModelIndex graph_index = viewmodel.index(2, 0);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(graph_index), graph_item);

  // removing graph item
  model.RemoveItem(graph_item);
  EXPECT_EQ(viewmodel.rowCount(), 2);  // X, Y

  graph_item = model.InsertItem<GraphItem>(viewport_item);
  EXPECT_EQ(viewmodel.rowCount(), 3);  // X, Y, graph
}

//! Inserting item in an empty tag.
//! This is to validate that item inserted in empty tag appears in the right order (real life bug).

TEST_F(AllItemsViewModelTest, InsertIntoEmptyTag)
{
  ApplicationModel model;

  auto parent = model.InsertItem<CompoundItem>();

  // for the moment, AddProperty doesn't trigger signaling
  // so we have to initialise viewmodel later

  auto& property = parent->AddProperty("name", "abc");
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);

  AllItemsViewModel viewmodel(&model);

  QSignalSpy spy_insert(&viewmodel, &ViewModelBase::rowsInserted);

  EXPECT_EQ(parent->GetAllItems(), std::vector<SessionItem*>({&property}));

  auto child = model.InsertItem<SessionItem>(parent);
  EXPECT_EQ(parent->GetAllItems(), std::vector<SessionItem*>({&property, child}));

  EXPECT_EQ(spy_insert.count(), 1);

  auto parent_index = viewmodel.index(0, 0);
  EXPECT_EQ(viewmodel.rowCount(parent_index), 2);
  EXPECT_EQ(viewmodel.columnCount(parent_index), 2);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(parent_index), parent);

  auto child_index1 = viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_index1), &property);

  auto child_index2 = viewmodel.index(1, 0, parent_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(child_index2), child);
}
