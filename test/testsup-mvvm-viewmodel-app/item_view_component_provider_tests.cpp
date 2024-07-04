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

#include "mvvm/providers/item_view_component_provider.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

class ItemViewComponentProviderTest : public ::testing::Test
{
public:
  mvvm::ApplicationModel m_model;
};

//! Initial state when no ApplicationModel is set.

TEST_F(ItemViewComponentProviderTest, InitialState)
{
  QTreeView view;

  EXPECT_THROW(
      ItemViewComponentProvider(std::make_unique<mvvm::AllItemsViewModel>(nullptr), nullptr),
      RuntimeException);

  ItemViewComponentProvider provider(std::make_unique<mvvm::AllItemsViewModel>(nullptr), &view);

  EXPECT_EQ(provider.GetView(), &view);
  EXPECT_NE(provider.GetSelectionModel(), nullptr);
  EXPECT_NE(provider.GetSelectionModel()->model(), nullptr);
  EXPECT_NE(provider.GetViewModel(), nullptr);
  EXPECT_EQ(provider.GetSelectedItem(), nullptr);
  EXPECT_TRUE(provider.GetSelectedItems().empty());
}

//! Setting empty ApplicationModel to the provider.

TEST_F(ItemViewComponentProviderTest, SetEmptyModel)
{
  QTreeView view;

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();
  provider->SetSelectedItem(item);

  EXPECT_EQ(provider->GetSelectedItem(), item);
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({item}));
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_item = arguments.at(0).value<mvvm::SessionItem*>();
  EXPECT_EQ(selected_item, item);

  spy_selected.clear();

  // removing selection

  provider->SetSelectedItem(nullptr);
  EXPECT_EQ(provider->GetSelectedItem(), nullptr);
  EXPECT_EQ(spy_selected.count(), 1);

  arguments = spy_selected.takeFirst();
  selected_item = arguments.at(0).value<mvvm::SessionItem*>();
  EXPECT_EQ(selected_item, nullptr);
}

//! Checking selection when acting through the view.

TEST_F(ItemViewComponentProviderTest, SetCurrentIndex)
{
  QTreeView view;

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

  auto item = m_model.InsertItem<mvvm::CompoundItem>();

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  // selecting an item and checking results
  auto indexes = provider->GetViewModel()->GetIndexOfSessionItem(item);
  ASSERT_EQ(indexes.size(), 2);
  provider->GetView()->setCurrentIndex(indexes.at(0));

  EXPECT_EQ(provider->GetSelectedItem(), item);
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({item}));
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_item = arguments.at(0).value<mvvm::SessionItem*>();
  EXPECT_EQ(selected_item, item);
}

//! Selecting whole row and checking list of selected items.
//! There should be no duplications.

TEST_F(ItemViewComponentProviderTest, SelectRow)
{
  QTreeView view;

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, model.get());

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

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

  QSignalSpy spy_selected(provider.get(), &ItemViewComponentProvider::SelectedItemChanged);

  // selecting single item
  provider->SetSelectedItem(property0);

  // checking selections
  EXPECT_EQ(provider->GetSelectedItems(), std::vector<mvvm::SessionItem*>({property0}));

  // checking signaling
  EXPECT_EQ(spy_selected.count(), 1);
  QList<QVariant> arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto item = arguments.at(0).value<mvvm::SessionItem*>();
  EXPECT_EQ(item, property0);

  spy_selected.clear();

  // removing item
  m_model.RemoveItem(property0);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(spy_selected.count(), 1);
  arguments = spy_selected.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  item = arguments.at(0).value<mvvm::SessionItem*>();
  EXPECT_EQ(item, nullptr);
}

//! Delete provider before the view.

TEST_F(ItemViewComponentProviderTest, DeleteProvider)
{
  QTreeView view;
  auto property0 = m_model.InsertItem<mvvm::PropertyItem>();

  auto provider = CreateProvider<mvvm::AllItemsViewModel>(&view, &m_model);

  EXPECT_EQ(view.model(), provider->GetViewModel());
  EXPECT_EQ(view.selectionModel(), provider->GetSelectionModel());
  EXPECT_EQ(view.selectionModel()->model(), provider->GetViewModel());

  // deleting provider
  provider.reset();

  EXPECT_EQ(view.model(), nullptr);

  // provider was using default selectionModel of a view, so it was created by the view and still
  // leaves there
  EXPECT_NE(view.selectionModel(), nullptr);
}
