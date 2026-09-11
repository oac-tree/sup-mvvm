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

#include "mvvm/views/item_view_component_provider.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/item_selection.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

namespace mvvm::test
{

class ItemViewComponentProviderTest : public ::testing::Test
{
public:
  std::unique_ptr<ItemViewComponentProvider> CreateProvider(QAbstractItemView* view,
                                                            ISessionModel* model = nullptr)
  {
    return std::make_unique<ItemViewComponentProvider>(
        std::make_unique<AllItemsViewModel>(model ? model : &m_model), view);
  }

  mvvm::ApplicationModel m_model;
};

TEST_F(ItemViewComponentProviderTest, InitialState)
{
  QTreeView view;

  EXPECT_THROW(
      ItemViewComponentProvider(std::make_unique<mvvm::AllItemsViewModel>(nullptr), nullptr),
      RuntimeException);

  const ItemViewComponentProvider provider(std::make_unique<mvvm::AllItemsViewModel>(nullptr),
                                           &view);

  EXPECT_EQ(provider.GetView(), &view);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);
  EXPECT_NE(provider.GetSelectionModel()->model(), nullptr);
  EXPECT_NE(provider.GetViewModel(), nullptr);
  EXPECT_TRUE(provider.GetSelection().IsEmpty());
  EXPECT_EQ(provider.GetLastProxyModel(), nullptr);
  EXPECT_TRUE(provider.GetProxyModelChain().empty());

  const QItemSelectionModel::SelectionFlags expected_flags =
      QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;

  EXPECT_EQ(provider.GetSelectionFlags(), expected_flags);
}

TEST_F(ItemViewComponentProviderTest, SetEmptyApplicationModel)
{
  QTreeView view;
  auto provider = CreateProvider(&view);

  EXPECT_EQ(provider->GetView(), &view);
  EXPECT_NE(provider->GetSelectionModel(), nullptr);
  EXPECT_EQ(provider->GetSelectionModel(), view.selectionModel());
  EXPECT_NE(provider->GetViewModel(), nullptr);
  EXPECT_NE(dynamic_cast<mvvm::AllItemsViewModel*>(provider->GetViewModel()), nullptr);
  EXPECT_TRUE(provider->GetSelection().IsEmpty());

  EXPECT_EQ(provider->GetView()->model(), provider->GetViewModel());
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());
}

TEST_F(ItemViewComponentProviderTest, SetNonEmptyModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  auto provider = CreateProvider(&view);

  EXPECT_EQ(provider->GetView(), &view);
  EXPECT_NE(provider->GetSelectionModel(), nullptr);

  auto viewmodel = provider->GetViewModel();

  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);
  EXPECT_EQ(viewmodel->GetRootSessionItem(), m_model.GetRootItem());

  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);
}

TEST_F(ItemViewComponentProviderTest, SetItemWithoutModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::VectorItem>();

  auto provider = CreateProvider(&view);

  provider->SetItem(item);

  auto viewmodel = provider->GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 3);  // x, y, z
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);

  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());
}

TEST_F(ItemViewComponentProviderTest, SetItemAfterSetModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  auto provider = CreateProvider(&view);

  auto viewmodel = provider->GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);

  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());

  // setting new item as root item
  provider->SetItem(item);

  auto new_viewmodel = provider->GetViewModel();

  // in the current implementation view model stays the same
  EXPECT_EQ(viewmodel, new_viewmodel);
  EXPECT_EQ(provider->GetView()->model(), new_viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), new_viewmodel);

  EXPECT_EQ(new_viewmodel->GetRootSessionItem(), item);

  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);

  // no rows and columns since our item plays the role of root item
  EXPECT_EQ(new_viewmodel->rowCount(QModelIndex()), 0);
  EXPECT_EQ(new_viewmodel->columnCount(QModelIndex()), 2);
}

TEST_F(ItemViewComponentProviderTest, SetNullptrAfterSetModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  auto provider = CreateProvider(&view);

  auto viewmodel = provider->GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);

  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());

  // resetting the model
  provider->SetApplicationModel(nullptr);

  // it doesn't remove viewmodel and selection model
  EXPECT_EQ(provider->GetViewModel(), viewmodel);
  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), viewmodel);

  // viewmodel now shows itself empty
  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 0);
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);
}

//! Attempt to set one item after another, when they belongs to different models
//! (real life bug)
TEST_F(ItemViewComponentProviderTest, SetItemAfterItemWhenTheyBelongsToDifferentModels)
{
  QTreeView view;

  mvvm::ApplicationModel model1;
  mvvm::ApplicationModel model2;

  auto item1 = model1.InsertItem<mvvm::CompoundItem>();
  item1->AddBranch<mvvm::VectorItem>("position");

  auto item2 = model2.InsertItem<mvvm::CompoundItem>();

  auto provider = CreateProvider(&view);

  // setting item from the first model
  provider->SetItem(item1);

  EXPECT_EQ(provider->GetViewModel()->GetRootSessionItem(), item1);
  EXPECT_EQ(provider->GetViewModel()->rowCount(QModelIndex()), 1);

  // setting item from the second model
  EXPECT_NO_THROW(provider->SetItem(item2));
  EXPECT_EQ(provider->GetViewModel()->rowCount(QModelIndex()), 0);
}

TEST_F(ItemViewComponentProviderTest, SelectItem)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectionChanged);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();

  const ItemSelection expected_selection(item);
  provider->SetSelection(expected_selection);

  EXPECT_EQ(provider->GetSelection(), expected_selection);

  EXPECT_EQ(mvvm::test::TakeValue<ItemSelection>(spy_selected), ItemSelection(item));

  spy_selected.clear();

  // setting the same item again
  provider->SetSelection(ItemSelection(item));
  EXPECT_EQ(spy_selected.count(), 0);  // no signal should be emitted

  // removing selection
  provider->SetSelection(ItemSelection(nullptr));
  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);

  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selected), ItemSelection(nullptr));
}

TEST_F(ItemViewComponentProviderTest, SelectOneItemThenAnother)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectionChanged);

  auto item0 = m_model.InsertItem<mvvm::CompoundItem>();
  auto item1 = m_model.InsertItem<mvvm::CompoundItem>();

  const ItemSelection expected_selection0(item0);
  provider->SetSelection(expected_selection0);

  EXPECT_EQ(provider->GetSelection(), expected_selection0);

  EXPECT_EQ(spy_selected.count(), 1);
  EXPECT_EQ(mvvm::test::TakeValue<ItemSelection>(spy_selected), ItemSelection(item0));

  // changing selection
  const ItemSelection expected_selection1(item1);
  provider->SetSelection(expected_selection1);

  EXPECT_EQ(provider->GetSelection().GetItems().size(), 1);

  EXPECT_EQ(spy_selected.count(), 1);
  EXPECT_EQ(mvvm::test::TakeValue<ItemSelection>(spy_selected), ItemSelection(item1));

  // validating selection directly via indexes
  auto selection_model = provider->GetSelectionModel();
  EXPECT_EQ(view.selectionModel(), selection_model);

  auto selected_indexes = selection_model->selectedIndexes();
  EXPECT_EQ(selected_indexes.size(), 2);  // two columns
  auto viewmodel = provider->GetViewModel();

  auto index0 = provider->GetViewModel()->index(1, 0, QModelIndex());
  auto index1 = provider->GetViewModel()->index(1, 1, QModelIndex());
  EXPECT_EQ(selected_indexes.at(0), index0);
  EXPECT_EQ(selected_indexes.at(1), index1);
}

//! Selecting one item, then another, after the first selection has been committed into the
//! selection model's permanent ranges. This mimics what happens in a real application, where
//! between two SetSelection calls the view (or any other actor) issues a selection command without
//! the QItemSelectionModel::Current flag, which finalizes the transient "current selection". The
//! second selection must replace the first one, not add to it.

TEST_F(ItemViewComponentProviderTest, SelectOneItemThenAnotherAfterSelectionCommitted)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  auto item0 = m_model.InsertItem<mvvm::CompoundItem>();
  auto item1 = m_model.InsertItem<mvvm::CompoundItem>();

  provider->SetSelection(ItemSelection(item0));
  EXPECT_EQ(provider->GetSelection(), ItemSelection(item0));

  // Commit the transient "current selection" into the permanent ranges by issuing a selection
  // command without the QItemSelectionModel::Current flag (as the view does in a real application).
  provider->GetSelectionModel()->select(QItemSelection(), QItemSelectionModel::Select);
  EXPECT_EQ(provider->GetSelection(), ItemSelection(item0));

  // changing selection; the second selection must replace the committed first one
  provider->SetSelection(ItemSelection(item1));

  EXPECT_EQ(provider->GetSelection().GetItems().size(), 1);
  EXPECT_EQ(provider->GetSelection(), ItemSelection(item1));
}

TEST_F(ItemViewComponentProviderTest, SetCurrentIndexInViewAndCheckSelection)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectionChanged);

  // selecting an item and checking results
  auto indexes = provider->GetViewModel()->GetIndexOfSessionItem(item);
  ASSERT_EQ(indexes.size(), 2);
  provider->GetView()->setCurrentIndex(indexes.at(0));

  EXPECT_EQ(provider->GetSelection().GetItem(), item);

  EXPECT_EQ(mvvm::test::TakeValue<ItemSelection>(spy_selected), ItemSelection(item));
}

TEST_F(ItemViewComponentProviderTest, SelectWholeRowAndCheckResultingSelection)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  auto vector_item = m_model.InsertItem<mvvm::VectorItem>();

  auto x_item = vector_item->FindItem(mvvm::VectorItem::kX);

  provider->SetItem(vector_item);

  auto x_item_index = provider->GetViewModel()->index(0, 1, QModelIndex());

  const QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectionChanged);

  // selecting row where xItem is located
  view.selectionModel()->select(x_item_index,
                                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  EXPECT_EQ(provider->GetSelection().GetItems(), std::vector<const mvvm::SessionItem*>({x_item}));
  EXPECT_EQ(spy_selected.count(), 1);
}

TEST_F(ItemViewComponentProviderTest, DestroyModel)
{
  QTreeView view;

  // setting up model and viewmodel
  auto model = std::make_unique<mvvm::ApplicationModel>();

  auto vector_item = model->InsertItem<mvvm::VectorItem>();

  auto provider = CreateProvider(&view, model.get());

  auto viewmodel = provider->GetViewModel();
  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);

  // destroying the model
  model.reset();

  EXPECT_EQ(provider->GetViewModel(), viewmodel);

  EXPECT_EQ(viewmodel->rowCount(QModelIndex()), 0);
  EXPECT_EQ(viewmodel->columnCount(QModelIndex()), 2);
  EXPECT_EQ(viewmodel->GetRootSessionItem(), nullptr);

  EXPECT_TRUE(provider->GetSelection().IsEmpty());
}

TEST_F(ItemViewComponentProviderTest, CheckSelectionChangeAfterItemRemoval)
{
  QTreeView view;
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectionChanged);

  // selecting single item
  provider->SetSelection(ItemSelection(property0));

  // checking selections
  EXPECT_EQ(provider->GetSelection().GetItems(),
            std::vector<const mvvm::SessionItem*>({property0}));

  // checking signaling
  EXPECT_EQ(mvvm::test::TakeValue<ItemSelection>(spy_selected), ItemSelection(property0));

  spy_selected.clear();

  // removing item
  m_model.RemoveItem(property0);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(spy_selected), ItemSelection());
}

TEST_F(ItemViewComponentProviderTest, DeleteProviderBeforeView)
{
  QTreeView view;
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();

  auto provider = CreateProvider(&view);

  EXPECT_EQ(view.model(), provider->GetViewModel());
  EXPECT_EQ(view.selectionModel(), provider->GetSelectionModel());
  EXPECT_EQ(view.selectionModel()->model(), provider->GetViewModel());

  // deleting provider
  provider.reset();

  EXPECT_EQ(view.model(), nullptr);

  // provider has own selectionModel, and it dies together with provider
  EXPECT_EQ(view.selectionModel(), nullptr);
}

//! Testing provider in the presence of proxy model. We create the model with 3 items, and then
//! filtering out the first item. The provider should let us access two remaining items via view
//! indexes.
TEST_F(ItemViewComponentProviderTest, AddProxyModelToFilterNames)
{
  QTreeView view;

  // model with 3 items
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();
  property0->SetDisplayName("A");
  auto property1 = m_model.InsertItem<mvvm::PropertyItem>();
  property1->SetDisplayName("AB");
  auto property2 = m_model.InsertItem<mvvm::PropertyItem>();
  property2->SetDisplayName("ABC");

  // provider charged with the proxy model
  auto provider = CreateProvider(&view);
  auto proxy = std::make_unique<FilterNameViewModel>();
  auto proxy_ptr = proxy.get();
  provider->AddProxyModel(std::move(proxy));

  // view is looking to the proxy
  EXPECT_EQ(provider->GetLastProxyModel(), proxy_ptr);
  EXPECT_EQ(view.model(), proxy_ptr);
  EXPECT_EQ(provider->GetProxyModelChain(), std::vector<QAbstractProxyModel*>({proxy_ptr}));

  // proxy model sees all items
  EXPECT_EQ(proxy_ptr->rowCount(QModelIndex()), 3);
  EXPECT_EQ(proxy_ptr->columnCount(QModelIndex()), 2);

  // filtering out first item "A"; items "AB" and "ABC" should be there
  proxy_ptr->SetPattern("AB");
  EXPECT_EQ(proxy_ptr->rowCount(QModelIndex()), 2);
  EXPECT_EQ(proxy_ptr->columnCount(QModelIndex()), 2);

  // view indexes of items "AB" and "ABC"
  auto displayname_index0 = proxy_ptr->index(0, 0, QModelIndex());
  auto value_index0 = proxy_ptr->index(0, 1, QModelIndex());
  auto displayname_index1 = proxy_ptr->index(1, 0, QModelIndex());
  auto value_index1 = proxy_ptr->index(1, 1, QModelIndex());

  // we can get access to the original property items via new indexes
  EXPECT_EQ(provider->GetItemFromViewIndex(displayname_index0), property1);
  EXPECT_EQ(provider->GetItemFromViewIndex(value_index0), property1);
  EXPECT_EQ(provider->GetItemFromViewIndex(displayname_index1), property2);
  EXPECT_EQ(provider->GetItemFromViewIndex(value_index1), property2);

  // we can get new view indices back from items
  EXPECT_TRUE(provider->GetViewIndexes(property0).empty());  // was filtered out
  EXPECT_EQ(provider->GetViewIndexes(property1),
            QModelIndexList({displayname_index0, value_index0}));
  EXPECT_EQ(provider->GetViewIndexes(property2),
            QModelIndexList({displayname_index1, value_index1}));
}

//! Testing provider in the presence of proxy model. Same test as above, with focus on
//! SetSelected/GetSelected.
TEST_F(ItemViewComponentProviderTest, AddProxyModelToFilterNamesAndCheckSelection)
{
  QTreeView view;

  // model with 3 items
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();
  property0->SetDisplayName("A");
  auto property1 = m_model.InsertItem<mvvm::PropertyItem>();
  property1->SetDisplayName("AB");
  auto property2 = m_model.InsertItem<mvvm::PropertyItem>();
  property2->SetDisplayName("ABC");

  // provider charged with the proxy model
  auto provider = CreateProvider(&view);
  auto proxy = std::make_unique<FilterNameViewModel>();
  auto proxy_ptr = proxy.get();
  provider->AddProxyModel(std::move(proxy));

  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);

  provider->SetSelection(ItemSelection(property0));
  EXPECT_EQ(provider->GetSelection().GetItem(), property0);

  // filtering out first item "A"; items "AB" and "ABC" should be there
  proxy_ptr->SetPattern("AB");
  EXPECT_EQ(proxy_ptr->rowCount(QModelIndex()), 2);
  EXPECT_EQ(proxy_ptr->columnCount(QModelIndex()), 2);

  // after filtering, selecte item has gone
  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);
  provider->SetSelection(ItemSelection(property2));
  EXPECT_EQ(provider->GetSelection().GetItem(), property2);
}

//! Testing provider in the presence of two proxy model. We create the model with 3 items, and then
//! filtering out two of them. The provider should let us access remaining item via view
//! indexes.
TEST_F(ItemViewComponentProviderTest, TwoProxyModels)
{
  QTreeView view;

  // model with 3 items
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();
  property0->SetDisplayName("A");
  auto property1 = m_model.InsertItem<mvvm::PropertyItem>();
  property1->SetDisplayName("AB");
  auto property2 = m_model.InsertItem<mvvm::PropertyItem>();
  property2->SetDisplayName("ABC");

  // provider charged with the proxy model
  auto provider = CreateProvider(&view);

  auto proxy0 = std::make_unique<FilterNameViewModel>();
  auto proxy0_ptr = proxy0.get();
  provider->AddProxyModel(std::move(proxy0));

  auto proxy1 = std::make_unique<FilterNameViewModel>();
  auto proxy1_ptr = proxy1.get();
  provider->AddProxyModel(std::move(proxy1));

  // view is looking to the proxy
  EXPECT_EQ(provider->GetLastProxyModel(), proxy1_ptr);
  EXPECT_EQ(view.model(), proxy1_ptr);
  EXPECT_EQ(provider->GetProxyModelChain(),
            std::vector<QAbstractProxyModel*>({proxy0_ptr, proxy1_ptr}));

  // proxy model sees all items
  EXPECT_EQ(proxy1_ptr->rowCount(QModelIndex()), 3);
  EXPECT_EQ(proxy1_ptr->columnCount(QModelIndex()), 2);

  // filtering out first item "A"; then second item "AB". "ABC" should be there
  proxy0_ptr->SetPattern("AB");
  proxy1_ptr->SetPattern("ABC");

  EXPECT_EQ(proxy1_ptr->rowCount(QModelIndex()), 1);
  EXPECT_EQ(proxy1_ptr->columnCount(QModelIndex()), 2);

  // view indexes of item "ABC"
  auto displayname_index2 = proxy1_ptr->index(0, 0, QModelIndex());
  auto value_index2 = proxy1_ptr->index(0, 1, QModelIndex());

  // we can get access to the original property items via new indexes
  EXPECT_EQ(provider->GetItemFromViewIndex(displayname_index2), property2);
  EXPECT_EQ(provider->GetItemFromViewIndex(value_index2), property2);

  // we can get new view indices back from the item
  EXPECT_TRUE(provider->GetViewIndexes(property0).empty());  // was filtered out
  EXPECT_TRUE(provider->GetViewIndexes(property1).empty());  // was filtered out
  EXPECT_EQ(provider->GetViewIndexes(property2),
            QModelIndexList({displayname_index2, value_index2}));

  // only property2 is selectable, others are filtered out
  provider->SetSelection(ItemSelection(property0));
  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);
  provider->SetSelection(ItemSelection(property1));
  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);
  provider->SetSelection(ItemSelection(property2));
  EXPECT_EQ(provider->GetSelection().GetItem(), property2);
}

//! Setting the new model leads to the change in view's underlying selection model. This test
//! checks, that after adding new proxy model, provider still sends ItemSelected signals.
TEST_F(ItemViewComponentProviderTest, SelectItemAfterModelSwitch)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectionChanged);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetDisplayName("ABC");
  provider->SetSelection(ItemSelection(item));

  EXPECT_EQ(spy_selected.count(), 1);

  spy_selected.clear();

  // removing selection
  provider->SetSelection(ItemSelection());
  EXPECT_EQ(provider->GetSelection().GetItem(), nullptr);
  EXPECT_EQ(spy_selected.count(), 1);

  spy_selected.clear();

  auto proxy0 = std::make_unique<FilterNameViewModel>();
  provider->AddProxyModel(std::move(proxy0));

  provider->SetSelection(ItemSelection(item));
  EXPECT_EQ(spy_selected.count(), 1);
}

}  // namespace mvvm::test
