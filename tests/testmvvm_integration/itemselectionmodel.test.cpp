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

#include "mvvm/widgets/itemselectionmodel.h"

#include "mvvm/model/application_model.h"
#include "mvvm/model/compound_item.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"

#include <gtest/gtest.h>

#include <QSignalSpy>

using namespace mvvm;

class ItemSelectionModelTest : public ::testing::Test
{
public:
  ItemSelectionModelTest() : m_view_model(&m_model), m_selection_model(&m_view_model) {}

  ApplicationModel m_model;
  TopItemsViewModel m_view_model;
  ItemSelectionModel m_selection_model;
};

TEST_F(ItemSelectionModelTest, InitialState)
{
  EXPECT_EQ(m_selection_model.GetSelectedItem(), nullptr);
  EXPECT_TRUE(m_selection_model.GetSelectedItems().empty());
}

TEST_F(ItemSelectionModelTest, SelectItem)
{
  auto item = m_model.InsertItem<mvvm::CompoundItem>();

  QSignalSpy spy_selection_changed(&m_selection_model, &ItemSelectionModel::selectionChanged);
  QSignalSpy spy_selected_item_changed(&m_selection_model,
                                       &ItemSelectionModel::SelectedItemChanged);

  EXPECT_EQ(m_selection_model.GetSelectedItem(), nullptr);

  // selecting an item and checking results
  m_selection_model.SetSelectedItem(item);
  EXPECT_EQ(m_selection_model.GetSelectedItem(), item);
  EXPECT_EQ(m_selection_model.GetSelectedItems(), std::vector<const mvvm::SessionItem*>({item}));

  EXPECT_EQ(spy_selection_changed.count(), 1);
  EXPECT_EQ(spy_selected_item_changed.count(), 1);

  QList<QVariant> arguments = spy_selected_item_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  const auto* selected_item = arguments.at(0).value<const mvvm::SessionItem*>();
  EXPECT_EQ(selected_item, item);

  spy_selection_changed.clear();

  // removing selection

  m_selection_model.SetSelectedItem(nullptr);
  EXPECT_EQ(m_selection_model.GetSelectedItem(), nullptr);
  EXPECT_EQ(spy_selection_changed.count(), 1);

  arguments = spy_selection_changed.takeFirst();
  selected_item = arguments.at(0).value<const mvvm::SessionItem*>();
  EXPECT_EQ(selected_item, nullptr);
}

//! Removing selected and checking notifications

TEST_F(ItemSelectionModelTest, SelectionAfterRemoval)
{
  auto item = m_model.InsertItem<mvvm::CompoundItem>();

  // selecting single item
  m_selection_model.SetSelectedItem(item);

  // checking selections
  EXPECT_EQ(m_selection_model.GetSelectedItem(), item);
  EXPECT_EQ(m_selection_model.GetSelectedItems(), std::vector<const mvvm::SessionItem*>({item}));

  QSignalSpy spy_selection_changed(&m_selection_model, &ItemSelectionModel::selectionChanged);
  QSignalSpy spy_selected_item_changed(&m_selection_model,
                                       &ItemSelectionModel::SelectedItemChanged);

  // removing item
  m_model.RemoveItem(item);

  // signal should emit once and report nullptr as selected item
  EXPECT_EQ(spy_selection_changed.count(), 1);
  EXPECT_EQ(spy_selected_item_changed.count(), 1);

  auto arguments = spy_selected_item_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 1);
  auto selected_procedure = arguments.at(0).value<const mvvm::SessionItem*>();
  EXPECT_EQ(selected_procedure, nullptr);
}
