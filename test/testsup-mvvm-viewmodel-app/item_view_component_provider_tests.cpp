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

#include "mvvm/providers/item_view_component_provider.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>

#include <gtest/gtest.h>

#include <QDebug>
#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

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

//! Initial state when no ApplicationModel is set.

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
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider.GetSelectedItems().empty());
  EXPECT_EQ(provider.GetLastProxyModel(), nullptr);
  EXPECT_TRUE(provider.GetProxyModelChain().empty());
}

//! Setting empty ApplicationModel to the provider.

TEST_F(ItemViewComponentProviderTest, SetEmptyModel)
{
  QTreeView view;
  auto provider = CreateProvider(&view);

  EXPECT_EQ(provider->GetView(), &view);
  EXPECT_NE(provider->GetSelectionModel(), nullptr);
  EXPECT_NE(provider->GetViewModel(), nullptr);
  EXPECT_NE(dynamic_cast<mvvm::AllItemsViewModel*>(provider->GetViewModel()), nullptr);
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider->GetSelectedItems().empty());

  EXPECT_EQ(provider->GetView()->model(), provider->GetViewModel());
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());
}

//! Setting ApplicationModel with single item to the provider.

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
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  EXPECT_EQ(viewmodel->GetRootSessionItem(), m_model.GetRootItem());

  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);
}

//! No model initialisation, setting an item.

TEST_F(ItemViewComponentProviderTest, SetItem)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::VectorItem>();

  auto provider = CreateProvider(&view);

  provider->SetItem(item);

  auto viewmodel = provider->GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(), 3);  // x, y, z
  EXPECT_EQ(viewmodel->columnCount(), 2);

  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());
}

//! Initialising provider with application model, then changing root item.

TEST_F(ItemViewComponentProviderTest, SetItemAfterSetModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  auto provider = CreateProvider(&view);

  auto viewmodel = provider->GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

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

  EXPECT_EQ(provider->GetSelectedItem(), nullptr);

  // no rows and columns since our item plays the role of root item
  EXPECT_EQ(new_viewmodel->rowCount(), 0);
  EXPECT_EQ(new_viewmodel->columnCount(), 2);
}

//! Initialising provider with application model, then setting it to nullptr.

TEST_F(ItemViewComponentProviderTest, SetNullptrAfterSetModel)
{
  QTreeView view;

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetData(42);

  auto provider = CreateProvider(&view);

  auto viewmodel = provider->GetViewModel();
  ASSERT_NE(viewmodel, nullptr);
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), provider->GetViewModel());

  // resetting the model
  provider->SetApplicationModel(nullptr);

  // it doesn't remove viewmodel and selection model
  EXPECT_EQ(provider->GetViewModel(), viewmodel);
  EXPECT_EQ(provider->GetView()->model(), viewmodel);
  EXPECT_EQ(provider->GetSelectionModel()->model(), viewmodel);

  // viewmodel now shows itself empty
  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 2);
}

//! Attempt to set one item after another, when they belongs to different models
//! (real life bug)

TEST_F(ItemViewComponentProviderTest, SetItemAfterItem)
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
  EXPECT_EQ(provider->GetViewModel()->rowCount(), 1);

  // setting item from the second model
  EXPECT_NO_THROW(provider->SetItem(item2));
  EXPECT_EQ(provider->GetViewModel()->rowCount(), 0);
}

TEST_F(ItemViewComponentProviderTest, SelectItem)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  provider->SetSelectedItem(item);

  EXPECT_EQ(provider->GetSelectedItem(), item);
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({item}));

  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selected), item);

  spy_selected.clear();

  // removing selection
  provider->SetSelectedItem(nullptr);
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);

  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selected), nullptr);
}

//! Checking selection when acting through the view.

TEST_F(ItemViewComponentProviderTest, SetCurrentIndex)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  // selecting an item and checking results
  auto indexes = provider->GetViewModel()->GetIndexOfSessionItem(item);
  ASSERT_EQ(indexes.size(), 2);
  provider->GetView()->setCurrentIndex(indexes.at(0));

  EXPECT_EQ(provider->GetSelectedItem(), item);
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({item}));

  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selected), item);
}

//! Selecting whole row and checking list of selected items.
//! There should be no duplications.

TEST_F(ItemViewComponentProviderTest, SelectRow)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  auto vector_item = m_model.InsertItem<mvvm::VectorItem>();

  auto x_item = vector_item->GetItem(mvvm::VectorItem::kX);

  provider->SetItem(vector_item);

  auto x_item_index = provider->GetViewModel()->index(0, 1);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  // selecting row where xItem is located
  view.selectionModel()->select(x_item_index,
                                QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({x_item}));
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
  EXPECT_EQ(viewmodel->rowCount(), 1);
  EXPECT_EQ(viewmodel->columnCount(), 2);

  // destroying the model
  model.reset();

  EXPECT_EQ(provider->GetViewModel(), viewmodel);

  EXPECT_EQ(viewmodel->rowCount(), 0);
  EXPECT_EQ(viewmodel->columnCount(), 2);
  EXPECT_EQ(viewmodel->GetRootSessionItem(), nullptr);

  EXPECT_TRUE(provider->GetSelectedItems().empty());
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
}

//! Removing selected and checking notifications.

TEST_F(ItemViewComponentProviderTest, SelectionAfterRemoval)
{
  QTreeView view;
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  // selecting single item
  provider->SetSelectedItem(property0);

  // checking selections
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({property0}));

  // checking signaling
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selected), property0);

  spy_selected.clear();

  // removing item
  m_model.RemoveItem(property0);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::SessionItem*>(spy_selected), nullptr);
}

//! Delete provider before the view.

TEST_F(ItemViewComponentProviderTest, DeleteProvider)
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
TEST_F(ItemViewComponentProviderTest, FilterNameProxy)
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

//! Testing provider in the presence of proxy model.
//! Same test as above, with focus on SetSelected/GetSelected.
TEST_F(ItemViewComponentProviderTest, FilterNameProxySetSelected)
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

  EXPECT_EQ(provider->GetSelectedItem(), nullptr);

  provider->SetSelectedItem(property0);
  EXPECT_EQ(provider->GetSelectedItem(), property0);

  // filtering out first item "A"; items "AB" and "ABC" should be there
  proxy_ptr->SetPattern("AB");
  EXPECT_EQ(proxy_ptr->rowCount(QModelIndex()), 2);
  EXPECT_EQ(proxy_ptr->columnCount(QModelIndex()), 2);

  // after filtering, selecte item has gone
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  provider->SetSelectedItem(property2);
  EXPECT_EQ(provider->GetSelectedItem(), property2);
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
  provider->SetSelectedItem(property0);
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  provider->SetSelectedItem(property1);
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  provider->SetSelectedItem(property2);
  EXPECT_EQ(provider->GetSelectedItem(), property2);
}

//! Setting the new model leads to the change in view's underlying selection model. This test
//! checks, that after adding new proxy model, provinder still sends ItemSelected signals.
TEST_F(ItemViewComponentProviderTest, SelectItemAfterModelSwitch)
{
  QTreeView view;

  auto provider = CreateProvider(&view);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  item->SetDisplayName("ABC");
  provider->SetSelectedItem(item);

  EXPECT_EQ(spy_selected.count(), 1);

  spy_selected.clear();

  // removing selection
  provider->SetSelectedItem(nullptr);
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  EXPECT_EQ(spy_selected.count(), 1);

  spy_selected.clear();

  auto proxy0 = std::make_unique<FilterNameViewModel>();
  provider->AddProxyModel(std::move(proxy0));

  provider->SetSelectedItem(item);
  EXPECT_EQ(spy_selected.count(), 1);
}
