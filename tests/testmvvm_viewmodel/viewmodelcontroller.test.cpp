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

#include "mvvm/viewmodel/viewmodelcontroller.h"

#include "test_utils.h"

#include "mvvm/model/compounditem.h"
#include "mvvm/model/modelcomposer.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/standarditems/vectoritem.h"
#include "mvvm/viewmodel/modeleventnotifier.h"
#include "mvvm/viewmodelbase/viewmodelbase.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

#include <gtest/gtest.h>

#include <QDebug>
#include <QSignalSpy>

using namespace ModelView;

//! Tests for ViewModelController class.

class ViewModelControllerTest : public ::testing::Test
{
public:
  ViewModelControllerTest()
      : m_controller(&m_model, &m_viewmodel)
      , m_notifier(&m_controller)
      , m_composer(&m_model, &m_notifier)
  {
    m_controller.Init();
  }

  //! Returns underlying SessionItem from given ViewItem
  static const SessionItem* GetSessionItem(const ViewItem* view_item)
  {
    return Utils::GetContext<SessionItem>(view_item);
  }

  //! Returns underlying SessionItem from the given index.
  const SessionItem* GetSessionItem(const QModelIndex& index) const
  {
    return Utils::GetContext<SessionItem>(m_viewmodel.itemFromIndex(index));
  }

  std::vector<ModelView::ViewItem*> FindViews(SessionItem* item)
  {
    return Utils::FindViews<SessionItem>(&m_viewmodel, item);
  }

  SessionModel m_model;
  ViewModelBase m_viewmodel;
  ViewModelController m_controller;
  ModelEventNotifier m_notifier;
  ModelComposer m_composer;
};

//! Empty model.

TEST_F(ViewModelControllerTest, EmptyProcedure)
{
  m_controller.Init();

  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 0);

  EXPECT_EQ(ModelView::Utils::GetContext<SessionItem>(m_viewmodel.rootItem()),
            m_model.GetRootItem());

  EXPECT_EQ(ModelView::Utils::FindViews(&m_viewmodel, m_model.GetRootItem()),
            std::vector<ModelView::ViewItem*>({m_viewmodel.rootItem()}));
}

//! Attempt to initialize controller with wrong initial parameters.

TEST_F(ViewModelControllerTest, InvalidControllerInitialization)
{
  // should throw if input parameters are invalid
  {
    ViewModelController controller(nullptr, nullptr);
    EXPECT_THROW(controller.Init(), std::runtime_error);
  }

  // should throw if viewmodel is not empty
  {
    std::vector<std::unique_ptr<ModelView::ViewItem>> children;
    children.emplace_back(std::make_unique<ModelView::ViewItem>());
    m_viewmodel.appendRow(m_viewmodel.rootItem(), std::move(children));
    ViewModelController controller(&m_model, &m_viewmodel);
    EXPECT_THROW(controller.Init(), std::runtime_error);
  }
}

//! SessionModel is populated with a single item. The controller is initialised after.

TEST_F(ViewModelControllerTest, ModelWithSingleItem)
{
  auto* item = m_model.InsertItem<SessionItem>();
  item->SetData(42.0);

  ViewModelController controller(&m_model, &m_viewmodel);
  controller.Init();

  // the model contains only one entry
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = m_viewmodel.index(0, 0);
  auto* view_item_label = m_viewmodel.itemFromIndex(label_index);
  EXPECT_EQ(GetSessionItem(view_item_label), item);

  auto data_index = m_viewmodel.index(0, 1);
  auto view_item_data = m_viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(GetSessionItem(view_item_data), item);

  // display roleof first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(m_viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item'sdata
  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from item
  EXPECT_EQ(FindViews(item), std::vector<ViewItem*>({view_item_label, view_item_data}));
}

//! SessionModel is populated with a VectorItem item. The controller is initialised after.

TEST_F(ViewModelControllerTest, ModelWithVectorItem)
{
  auto vector_item = m_model.InsertItem<VectorItem>();
  vector_item->SetX(1.0);
  vector_item->SetY(2.0);
  vector_item->SetZ(3.0);

  ViewModelController controller(&m_model, &m_viewmodel);
  controller.Init();

  // the model contains only one entry
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  // accessing first child under the root item
  QModelIndex vector_label_index = m_viewmodel.index(0, 0);
  QModelIndex vector_data_index = m_viewmodel.index(0, 1);
  EXPECT_EQ(GetSessionItem(vector_label_index), vector_item);
  EXPECT_EQ(GetSessionItem(vector_data_index), vector_item);

  std::vector<SessionItem*> children = vector_item->GetAllItems();
  for (int row = 0; row < 3; ++row)
  {  // x, y, z
    QModelIndex child_label_index = m_viewmodel.index(row, 0, vector_label_index);
    QModelIndex child_data_index = m_viewmodel.index(row, 1, vector_label_index);
    EXPECT_EQ(GetSessionItem(child_label_index), children[row]);
    EXPECT_EQ(GetSessionItem(child_data_index), children[row]);
    EXPECT_EQ(m_viewmodel.data(child_label_index).toString().toStdString(),
              children[row]->GetDisplayName());
    EXPECT_EQ(m_viewmodel.data(child_data_index).toDouble(),
              children[row]->Data<double>());  // x,y,z coordinates
  }
}

//! Initialise controller with the empty model. Then insert new item and check that view model
//! hass been updated.

TEST_F(ViewModelControllerTest, InsertIntoEmptyModel)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item = m_composer.InsertItem<PropertyItem>();
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
  auto* view_item_label = m_viewmodel.itemFromIndex(label_index);
  EXPECT_EQ(GetSessionItem(view_item_label), item);

  auto data_index = m_viewmodel.index(0, 1);
  auto view_item_data = m_viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(GetSessionItem(view_item_data), item);

  // display roleof first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(m_viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item'sdata
  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from instruction
  EXPECT_EQ(FindViews(item), std::vector<ViewItem*>({view_item_label, view_item_data}));
}

//! Insert three property items in a model, inserted after controller was setup.

TEST_F(ViewModelControllerTest, InitThenInsertProperties)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_composer.InsertItem<PropertyItem>();
  auto item1 = m_composer.InsertItem<PropertyItem>();
  auto item2 = m_composer.InsertItem<PropertyItem>();

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 3);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(GetSessionItem(m_viewmodel.index(0, 0)), item0);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(1, 0)), item1);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(2, 0)), item2);

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

TEST_F(ViewModelControllerTest, InsertInFront)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_composer.InsertItem<PropertyItem>();
  auto item1 = m_composer.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 0});

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 2);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 2);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(GetSessionItem(m_viewmodel.index(0, 0)), item1);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(1, 0)), item0);
}

//! Inserting item between two other.

TEST_F(ViewModelControllerTest, InsertBetween)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_composer.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 0});
  auto item1 = m_composer.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 1});
  auto item2 = m_composer.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 1});  // between

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 3);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(), 3);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(GetSessionItem(m_viewmodel.index(0, 0)), item0);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(1, 0)), item2);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(2, 0)), item1);
}

//! Initialise controller with the empty model. Insert parent and then child into it.

TEST_F(ViewModelControllerTest, InsertParentAndThenChild)
{
  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto parent = m_composer.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child = m_composer.InsertItem<SessionItem>(parent);

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 2);
  EXPECT_EQ(spy_remove.count(), 0);

  auto parent_index = m_viewmodel.index(0, 0);
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 1);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);
  EXPECT_EQ(GetSessionItem(parent_index), parent);

  auto child_index = m_viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(GetSessionItem(child_index), child);

  const auto& arguments = spy_insert.at(1);
  EXPECT_EQ(arguments.size(), 3);
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), parent_index);
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}

//! Removing single top level item.

TEST_F(ViewModelControllerTest, RemoveSingleTopItem)
{
  auto item = m_composer.InsertItem<PropertyItem>();

  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  m_composer.RemoveItem(m_model.GetRootItem(), {"", 0});
  EXPECT_EQ(m_viewmodel.rowCount(), 0);
  EXPECT_EQ(m_viewmodel.columnCount(), 0);

  ASSERT_EQ(spy_insert.count(), 0);
  ASSERT_EQ(spy_remove.count(), 1);

  QList<QVariant> arguments = spy_remove.takeFirst();
  ASSERT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}

//! Sequence with 3 children. Removing the middle one.

TEST_F(ViewModelControllerTest, RemoveMiddleChild)
{
  auto parent = m_composer.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child0 = m_composer.InsertItem<SessionItem>(parent, {"", 0});
  auto child1 = m_composer.InsertItem<SessionItem>(parent, {"", 1});
  auto child2 = m_composer.InsertItem<SessionItem>(parent, {"", 2});

  // one entry (parent)
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  auto parent_index = m_viewmodel.index(0, 0);
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 3);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);

  QSignalSpy spyInsert(&m_viewmodel, &ModelView::ViewModelBase::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &ModelView::ViewModelBase::rowsRemoved);

  // inserting children between two other
  m_composer.RemoveItem(parent, {"", 1});

  // one entry (parent)
  EXPECT_EQ(m_viewmodel.rowCount(), 1);
  EXPECT_EQ(m_viewmodel.columnCount(), 2);

  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyRemove.count(), 1);

  // two remaining children
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 2);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);

  auto child0_index = m_viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(GetSessionItem(child0_index), child0);

  auto child2_index = m_viewmodel.index(1, 0, parent_index);
  EXPECT_EQ(GetSessionItem(child2_index), child2);

  QList<QVariant> arguments = spyRemove.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), parent_index);
  EXPECT_EQ(arguments.at(1).value<int>(), 1);
  EXPECT_EQ(arguments.at(2).value<int>(), 1);
}

//! Sequence with 3 children. Removing the middle one.

TEST_F(ViewModelControllerTest, SetData)
{
  auto item = m_composer.InsertItem<PropertyItem>();
  item->SetData(0.0);

  QSignalSpy spy_data_changed(&m_viewmodel, &ViewModelBase::dataChanged);

  // modifying data through the composer
  m_composer.SetData(item, 42.0, DataRole::kData);

  EXPECT_EQ(spy_data_changed.count(), 1);

  QModelIndex dataIndex = m_viewmodel.index(0, 1);

  QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), m_viewmodel.index(0, 1));
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), m_viewmodel.index(0, 1));
  QVector<int> expectedRoles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expectedRoles);
}
