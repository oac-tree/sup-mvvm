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

#include "mvvm/widgets/abstractitemview.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <gtest/gtest.h>

#include "abstractitemview.test.h"

#include <QSignalSpy>
#include <QTreeView>

using namespace mvvm;

class AbstractItemViewTest : public ::testing::Test
{
public:
  ApplicationModel m_model;
};

TEST_F(AbstractItemViewTest, InitialState)
{
  testutils::TestView view(nullptr);
  EXPECT_EQ(view.GetViewModel(), nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);
  EXPECT_TRUE(view.GetSelectedItems().empty());
}

TEST_F(AbstractItemViewTest, ModelInConstructor)
{
  testutils::TestView view(&m_model);
  EXPECT_NE(view.GetViewModel(), nullptr);
  EXPECT_EQ(view.GetSelectedItem(), nullptr);

  //  EXPECT_EQ(view.GetViewModel()->GetRootSessionItem(), m_model.GetRootItem());
}

// TEST_F(AbstractItemViewTest, StateAfterSetup)
//{
//   ItemViewBase view;

//  auto tree_view = new QTreeView;
//  view.SetView(tree_view);
//  EXPECT_EQ(view.GetView(), tree_view);

//  auto view_model = std::make_unique<TopItemsViewModel>(&m_model);
//  auto view_model_ptr = view_model.get();
//  view.SetViewModel(std::move(view_model));
//  EXPECT_EQ(view.GetViewModel(), view_model_ptr);

//  EXPECT_EQ(view.GetSelectedItem(), nullptr);
//  EXPECT_TRUE(view.GetSelectedItems().empty());
//}

// TEST_F(ItemViewBaseTest, SelectItem)
//{
//   ItemViewBase view;

//  view.SetView(new QTreeView);
//  view.SetViewModel(std::make_unique<TopItemsViewModel>(&m_model));

//  QSignalSpy spy_selected(&view, &ItemViewBase::SelectedItemChanged);

//  auto item = m_model.InsertItem<CompoundItem>();
//  view.SetSelectedItem(item);

//  EXPECT_EQ(view.GetSelectedItem(), item);
//  EXPECT_EQ(view.GetSelectedItems(), std::vector<const SessionItem*>({item}));
//  EXPECT_EQ(spy_selected.count(), 1);
//  QList<QVariant> arguments = spy_selected.takeFirst();
//  EXPECT_EQ(arguments.size(), 1);
//  auto selected_item = arguments.at(0).value<const mvvm::SessionItem*>();
//  EXPECT_EQ(selected_item, item);

//  spy_selected.clear();

//  // removing selection

//  view.SetSelectedItem(nullptr);
//  EXPECT_EQ(view.GetSelectedItem(), nullptr);
//  EXPECT_EQ(spy_selected.count(), 1);

//  arguments = spy_selected.takeFirst();
//  selected_item = arguments.at(0).value<const mvvm::SessionItem*>();
//  EXPECT_EQ(selected_item, nullptr);
//}

////! Checking selection when acting through the view.

// TEST_F(ItemViewBaseTest, SetCurrentIndex)
//{
//   ItemViewBase view;

//  view.SetView(new QTreeView);
//  view.SetViewModel(std::make_unique<TopItemsViewModel>(&m_model));

//  auto item = m_model.InsertItem<CompoundItem>();

//  QSignalSpy spy_selected(&view, &ItemViewBase::SelectedItemChanged);

//  // selecting an item and checking results
//  auto indexes = view.GetViewModel()->GetIndexOfSessionItem(item);
//  ASSERT_EQ(indexes.size(), 2);
//  view.GetView()->setCurrentIndex(indexes.at(0));

//  EXPECT_EQ(view.GetSelectedItem(), item);
//  EXPECT_EQ(view.GetSelectedItems(), std::vector<const SessionItem*>({item}));
//  EXPECT_EQ(spy_selected.count(), 1);
//  QList<QVariant> arguments = spy_selected.takeFirst();
//  EXPECT_EQ(arguments.size(), 1);
//  auto selected_item = arguments.at(0).value<const mvvm::SessionItem*>();
//  EXPECT_EQ(selected_item, item);
//}
