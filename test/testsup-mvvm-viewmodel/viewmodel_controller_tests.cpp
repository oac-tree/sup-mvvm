/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/providers/viewmodel_controller.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/standard_row_strategies.h>
#include <mvvm/providers/viewmodel_controller_factory.h>
#include <mvvm/providers/viewmodel_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/viewmodel_base.h>

#include <gtest/gtest.h>
#include <testutils/test_strategies.h>

#include <QSignalSpy>

namespace mvvm::test
{

/**
 * @brief Tests for ViewModelController class.
 */
class ViewModelControllerTest : public ::testing::Test
{
public:
  ViewModelControllerTest() = default;

  template <typename ChildrenT = AllChildrenStrategy, typename RowT = LabelDataRowStrategy>
  std::unique_ptr<AbstractViewModelController> CreateController(ISessionModel& model,
                                                                ViewModelBase& view_model)
  {
    return factory::CreateController<ChildrenT, RowT>(&model, &view_model);
  }

  template <typename ChildrenT = AllChildrenStrategy, typename RowT = LabelDataRowStrategy>
  std::unique_ptr<AbstractViewModelController> CreateController(ViewModelBase& view_model)
  {
    return factory::CreateController<ChildrenT, RowT>(nullptr, &view_model);
  }

  //! Returns underlying SessionItem from given ViewItem
  static const SessionItem* GetSessionItem(const ViewItem* view_item)
  {
    return utils::GetItemFromView<SessionItem>(view_item);
  }

  //! Returns underlying SessionItem from the given index.
  const SessionItem* GetSessionItem(const QModelIndex& index) const
  {
    return utils::GetItemFromView<SessionItem>(m_viewmodel.itemFromIndex(index));
  }

  std::vector<const mvvm::ViewItem*> FindViews(const SessionItem* item)
  {
    return utils::FindViewsForItem<SessionItem>(&m_viewmodel, item);
  }

  ApplicationModel m_model;
  ViewModelBase m_viewmodel;
};

//! Controller pointed to empty model.

TEST_F(ViewModelControllerTest, InitialState)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  EXPECT_EQ(controller->GetModel(), &m_model);
  EXPECT_EQ(controller->GetRootItem(), m_model.GetRootItem());

  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 0);

  EXPECT_EQ(mvvm::utils::GetItemFromView<SessionItem>(m_viewmodel.rootItem()),
            m_model.GetRootItem());

  EXPECT_EQ(mvvm::utils::FindViewsForItem(&m_viewmodel, m_model.GetRootItem()),
            std::vector<const mvvm::ViewItem*>({m_viewmodel.rootItem()}));
}

//! Attempt to initialize controller with wrong initial parameters.

TEST_F(ViewModelControllerTest, InvalidControllerInitialization)
{
  // should throw if input parameters are invalid
  {
    EXPECT_THROW(ViewModelController({}), RuntimeException);
  }

  // current approach: if ViewModel is not-empty, it will be cleaned up
  {
    ViewModelBase viewmodel;
    std::vector<std::unique_ptr<mvvm::ViewItem>> children;
    children.emplace_back(std::make_unique<mvvm::ViewItem>());
    viewmodel.appendRow(viewmodel.rootItem(), std::move(children));

    auto controller = CreateController(viewmodel);

    EXPECT_NO_THROW(controller->SetRootItem(m_model.GetRootItem()));
    EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 0);
    EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 0);
  }
}

//! SessionModel is populated with a single item. The controller is initialised after.

TEST_F(ViewModelControllerTest, ModelWithSingleItem)
{
  auto* item = m_model.InsertItem<SessionItem>();
  item->SetData(42.0);

  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  // the model contains only one entry
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = m_viewmodel.index(0, 0, QModelIndex());
  auto view_item_label = m_viewmodel.itemFromIndex(label_index);
  EXPECT_EQ(GetSessionItem(view_item_label), item);

  auto data_index = m_viewmodel.index(0, 1, QModelIndex());
  auto view_item_data = m_viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(GetSessionItem(view_item_data), item);

  // display role of the first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(m_viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item's data
  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from item
  EXPECT_EQ(FindViews(item), std::vector<const ViewItem*>({view_item_label, view_item_data}));
}

//! SessionModel is populated with a VectorItem item. The controller is initialised after.

TEST_F(ViewModelControllerTest, ModelWithVectorItem)
{
  auto vector_item = m_model.InsertItem<VectorItem>();
  vector_item->SetX(1.0);
  vector_item->SetY(2.0);
  vector_item->SetZ(3.0);

  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  // the model contains only one entry
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  // accessing first child under the root item
  const QModelIndex vector_label_index = m_viewmodel.index(0, 0, QModelIndex());
  const QModelIndex vector_data_index = m_viewmodel.index(0, 1, QModelIndex());
  EXPECT_EQ(GetSessionItem(vector_label_index), vector_item);
  EXPECT_EQ(GetSessionItem(vector_data_index), vector_item);

  std::vector<SessionItem*> children = vector_item->GetAllItems();
  for (int row = 0; row < 3; ++row)
  {  // x, y, z
    const QModelIndex child_label_index = m_viewmodel.index(row, 0, vector_label_index);
    const QModelIndex child_data_index = m_viewmodel.index(row, 1, vector_label_index);
    EXPECT_EQ(GetSessionItem(child_label_index), children[row]);
    EXPECT_EQ(GetSessionItem(child_data_index), children[row]);
    EXPECT_EQ(m_viewmodel.data(child_label_index, Qt::DisplayRole).toString().toStdString(),
              children[row]->GetDisplayName());
    EXPECT_EQ(m_viewmodel.data(child_data_index, Qt::DisplayRole).toDouble(),
              children[row]->Data<double>());  // x,y,z coordinates
  }
}

//! SessionModel is populated with a VectorItem item. The controller is initialised after.
//! VectorItem is used as new rootItem.

TEST_F(ViewModelControllerTest, ModelWithVectorItemAsRootItem)
{
  auto vector_item = m_model.InsertItem<VectorItem>();
  vector_item->SetX(1.0);
  vector_item->SetY(2.0);
  vector_item->SetZ(3.0);

  auto controller = CreateController(m_viewmodel);
  controller->SetRootItem(vector_item);

  // the model contains only one entry
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 3);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  std::vector<SessionItem*> children = vector_item->GetAllItems();
  for (int row = 0; row < 3; ++row)
  {  // x, y, z
    const QModelIndex child_label_index = m_viewmodel.index(row, 0, QModelIndex());
    const QModelIndex child_data_index = m_viewmodel.index(row, 1, QModelIndex());
    EXPECT_EQ(GetSessionItem(child_label_index), children[row]);
    EXPECT_EQ(GetSessionItem(child_data_index), children[row]);
    EXPECT_EQ(m_viewmodel.data(child_label_index, Qt::DisplayRole).toString().toStdString(),
              children[row]->GetDisplayName());
    EXPECT_EQ(m_viewmodel.data(child_data_index, Qt::DisplayRole).toDouble(),
              children[row]->Data<double>());  // x,y,z coordinates
  }
}

//! Initialise controller with the empty model. Then insert new item and check that view model
//! has been updated.

TEST_F(ViewModelControllerTest, InsertIntoEmptyModel)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  const QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

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
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  // default controller constructs a row consisting from item label (display name) and data
  auto label_index = m_viewmodel.index(0, 0, QModelIndex());
  auto* view_item_label = m_viewmodel.itemFromIndex(label_index);
  EXPECT_EQ(GetSessionItem(view_item_label), item);

  auto data_index = m_viewmodel.index(0, 1, QModelIndex());
  auto view_item_data = m_viewmodel.itemFromIndex(data_index);
  EXPECT_EQ(GetSessionItem(view_item_data), item);

  // display roleof first item in a row  should coincide with item's DisplayName
  EXPECT_EQ(m_viewmodel.data(label_index, Qt::DisplayRole).toString().toStdString(),
            item->GetDisplayName());

  // edit role of second item in a row  should coincide with item'sdata
  EXPECT_EQ(m_viewmodel.data(data_index, Qt::EditRole).toDouble(), item->Data<double>());

  // Finding view from instruction
  EXPECT_EQ(FindViews(item), std::vector<const ViewItem*>({view_item_label, view_item_data}));
}

//! Insert three property items in a model, inserted after controller was setup.

TEST_F(ViewModelControllerTest, InitThenInsertProperties)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  const QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_model.InsertItem<PropertyItem>();
  auto item1 = m_model.InsertItem<PropertyItem>();
  auto item2 = m_model.InsertItem<PropertyItem>();

  // checking signaling
  ASSERT_EQ(spy_insert.count(), 3);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 3);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  EXPECT_EQ(GetSessionItem(m_viewmodel.index(0, 0, QModelIndex())), item0);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(1, 0, QModelIndex())), item1);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(2, 0, QModelIndex())), item2);

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
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  const QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_model.InsertItem<PropertyItem>();
  auto item1 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 0});

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 2);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 2);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  EXPECT_EQ(GetSessionItem(m_viewmodel.index(0, 0, QModelIndex())), item1);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(1, 0, QModelIndex())), item0);
}

TEST_F(ViewModelControllerTest, InsertTemBetweenTwoItems)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  const QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto item0 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 0});
  auto item1 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 1});
  auto item2 = m_model.InsertItem<PropertyItem>(m_model.GetRootItem(), {"", 1});  // between

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 3);

  // checking model layout
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 3);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  EXPECT_EQ(GetSessionItem(m_viewmodel.index(0, 0, QModelIndex())), item0);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(1, 0, QModelIndex())), item2);
  EXPECT_EQ(GetSessionItem(m_viewmodel.index(2, 0, QModelIndex())), item1);
}

TEST_F(ViewModelControllerTest, InsertParentAndThenChildIntoEmptyModel)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  const QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child = m_model.InsertItem<SessionItem>(parent);

  // checking signaling
  EXPECT_EQ(spy_insert.count(), 2);
  EXPECT_EQ(spy_remove.count(), 0);

  auto parent_index = m_viewmodel.index(0, 0, QModelIndex());
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

TEST_F(ViewModelControllerTest, RemoveSingleTopItem)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  auto item = m_model.InsertItem<PropertyItem>();

  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);

  m_model.RemoveItem(item);
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 0);

  ASSERT_EQ(spy_insert.count(), 0);
  ASSERT_EQ(spy_remove.count(), 1);

  const QList<QVariant> arguments = spy_remove.takeFirst();
  ASSERT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), QModelIndex());
  EXPECT_EQ(arguments.at(1).value<int>(), 0);
  EXPECT_EQ(arguments.at(2).value<int>(), 0);
}

TEST_F(ViewModelControllerTest, RemoveMiddleChildFromItemWithThreeChildren)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  auto parent = m_model.InsertItem<CompoundItem>();
  parent->RegisterTag(TagInfo::CreateUniversalTag("ITEMS"), /*set_as_default*/ true);
  auto child0 = m_model.InsertItem<SessionItem>(parent, {"", 0});
  auto child1 = m_model.InsertItem<SessionItem>(parent, {"", 1});
  (void)child1;
  auto child2 = m_model.InsertItem<SessionItem>(parent, {"", 2});

  // one entry (parent)
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  auto parent_index = m_viewmodel.index(0, 0, QModelIndex());
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 3);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);

  const QSignalSpy spyInsert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  QSignalSpy spyRemove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  // removing middle child
  m_model.TakeItem(parent, {"", 1});

  // one entry (parent)
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyRemove.count(), 1);

  // two remaining children
  EXPECT_EQ(m_viewmodel.rowCount(parent_index), 2);
  EXPECT_EQ(m_viewmodel.columnCount(parent_index), 2);

  auto child0_index = m_viewmodel.index(0, 0, parent_index);
  EXPECT_EQ(GetSessionItem(child0_index), child0);

  auto child2_index = m_viewmodel.index(1, 0, parent_index);
  EXPECT_EQ(GetSessionItem(child2_index), child2);

  const QList<QVariant> arguments = spyRemove.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex &parent, int first, int last
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), parent_index);
  EXPECT_EQ(arguments.at(1).value<int>(), 1);
  EXPECT_EQ(arguments.at(2).value<int>(), 1);
}

//! Taking child from one container to insert it after in the another.
//! Checking that controller correctly "forgets" that children was already served (real-life-bug).
TEST_F(ViewModelControllerTest, TakeChildThenInsert)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  auto container0 = m_model.InsertItem<ContainerItem>();
  auto container1 = m_model.InsertItem<ContainerItem>();
  auto child0 = m_model.InsertItem<VectorItem>(container0, TagIndex::Append());

  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 2);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  auto container0_index = m_viewmodel.index(0, 0, QModelIndex());
  auto container1_index = m_viewmodel.index(1, 0, QModelIndex());

  EXPECT_EQ(m_viewmodel.rowCount(container0_index), 1);
  EXPECT_EQ(m_viewmodel.columnCount(container0_index), 2);
  EXPECT_EQ(m_viewmodel.rowCount(container1_index), 0);
  EXPECT_EQ(m_viewmodel.columnCount(container1_index), 0);

  const QSignalSpy spyInsert(&m_viewmodel, &mvvm::ViewModelBase::rowsInserted);
  const QSignalSpy spyRemove(&m_viewmodel, &mvvm::ViewModelBase::rowsRemoved);

  // taking child, but keeping it
  auto taken = m_model.TakeItem(container0, {"", 0});

  EXPECT_EQ(spyInsert.count(), 0);
  EXPECT_EQ(spyRemove.count(), 1);

  // inserting it to another parent
  EXPECT_NO_FATAL_FAILURE(m_model.InsertItem(std::move(taken), container1, TagIndex::Append()));

  // vectorItem and its 3 coordinate property items are inserted atomically
  EXPECT_EQ(spyInsert.count(), 1);
  EXPECT_EQ(spyRemove.count(), 1);

  EXPECT_EQ(m_viewmodel.rowCount(container0_index), 0);
  EXPECT_EQ(m_viewmodel.columnCount(container0_index), 0);
  EXPECT_EQ(m_viewmodel.rowCount(container1_index), 1);
  EXPECT_EQ(m_viewmodel.columnCount(container1_index), 2);
}

TEST_F(ViewModelControllerTest, SetData)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  auto item = m_model.InsertItem<PropertyItem>();
  item->SetData(0.0);

  QSignalSpy spy_data_changed(&m_viewmodel, &ViewModelBase::dataChanged);

  m_model.SetData(item, 42.0, DataRole::kData);

  EXPECT_EQ(spy_data_changed.count(), 1);

  const QModelIndex dataIndex = m_viewmodel.index(0, 1, QModelIndex());

  const QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), m_viewmodel.index(0, 1, QModelIndex()));
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), m_viewmodel.index(0, 1, QModelIndex()));
  const QVector<int> expectedRoles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expectedRoles);
}

TEST_F(ViewModelControllerTest, SetPropertyItemAsRoot)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  auto item = m_model.InsertItem<PropertyItem>();

  controller->SetRootItem(item);

  // new root item doesn't have children
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 0);
}

TEST_F(ViewModelControllerTest, SetCompoundWithChildAsRootItem)
{
  auto controller = CreateController(m_model, m_viewmodel);

  auto item = m_model.InsertItem<CompoundItem>();
  item->AddProperty("thickness", 42.0);
  item->AddProperty<VectorItem>("position");
  item->AddProperty("radius", 43.0);

  controller->SetRootItem(item);

  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 3);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 2);

  // checking vector item
  auto index_of_vector_item = m_viewmodel.index(1, 0, QModelIndex());
  EXPECT_EQ(m_viewmodel.rowCount(index_of_vector_item), 3);
  EXPECT_EQ(m_viewmodel.columnCount(index_of_vector_item), 2);
}

TEST_F(ViewModelControllerTest, OnModelReset)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  m_model.InsertItem<SessionItem>();
  m_model.InsertItem<SessionItem>();
  m_model.InsertItem<SessionItem>();

  const QSignalSpy spy_about_reset(&m_viewmodel, &ViewModelBase::modelAboutToBeReset);
  const QSignalSpy spy_reset(&m_viewmodel, &ViewModelBase::modelReset);
  const QSignalSpy spy_remove(&m_viewmodel, &ViewModelBase::rowsRemoved);
  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);

  m_model.Clear();

  EXPECT_EQ(spy_about_reset.count(), 1);
  EXPECT_EQ(spy_reset.count(), 1);
  EXPECT_EQ(spy_remove.count(), 0);
  EXPECT_EQ(spy_insert.count(), 0);
  EXPECT_EQ(m_viewmodel.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_viewmodel.columnCount(QModelIndex()), 0);
}

//! Real life scenario: initially empty SessionModel, apply ::clean, and then start to insert item.
TEST_F(ViewModelControllerTest, OnEmptyModelResetAndContinue)
{
  auto controller = CreateController(m_model, m_viewmodel);
  (void)controller;

  const QSignalSpy spy_reset(&m_viewmodel, &ViewModelBase::modelReset);
  m_model.Clear();

  EXPECT_EQ(spy_reset.count(), 1);

  // inserting new item
  const QSignalSpy spy_insert(&m_viewmodel, &ViewModelBase::rowsInserted);
  m_model.InsertItem<SessionItem>();

  EXPECT_EQ(spy_insert.count(), 1);
}

TEST_F(ViewModelControllerTest, GetHorizontalHeaderLabels)
{
  auto controller = CreateController(m_model, m_viewmodel);

  const QStringList expected_labels = QStringList() << "Name"
                                                    << "Value";
  EXPECT_EQ(controller->GetHorizontalHeaderLabels(), expected_labels);
}

TEST_F(ViewModelControllerTest, BrokenRowStrategy)
{
  // We are deliberatly constructing controller based on broken row strategy, which always returns
  // empty rows. This is abnormal situation when RowStrategy sees an unknown item for which it
  // doesn't know what to do. We do not want empty rows to be inserted in the model, so we expect an
  // exception.

  auto controller =
      CreateController<AllChildrenStrategy, test::EmptyRowTestStrategy>(m_model, m_viewmodel);
  (void)controller;
  EXPECT_THROW(m_model.InsertItem<SessionItem>(), InvalidOperationException);
}

}  // namespace mvvm::test
