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

#include "mvvm/viewmodel/viewmodel_utils.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/filter_name_viewmodel.h>
#include <mvvm/viewmodel/property_table_viewmodel.h>
#include <mvvm/viewmodel/viewitem_factory.h>

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

class ViewModelUtilsTest : public ::testing::Test
{
public:
};

TEST_F(ViewModelUtilsTest, GetItemFromView)
{
  VectorItem item;

  auto view = CreateDataViewItem(&item);
  EXPECT_EQ(utils::GetItemFromView<VectorItem>(view.get()), &item);
  EXPECT_EQ(utils::GetItemFromView(nullptr), nullptr);
}

TEST_F(ViewModelUtilsTest, ItemRoleToQtRole)
{
  // DATA role of SessionItem should be translated to two Qt roles (edit and display)
  auto roles = utils::ItemRoleToQtRole(DataRole::kData);
  QVector<int> expected = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(roles, expected);

  // Appearance roles of SessionItem on Qt site means color
  roles = utils::ItemRoleToQtRole(DataRole::kAppearance);
  expected = {Qt::ForegroundRole};
  EXPECT_EQ(roles, expected);

  // tooltip role
  roles = utils::ItemRoleToQtRole(DataRole::kTooltip);
  expected = {Qt::ToolTipRole};
  EXPECT_EQ(roles, expected);
}

TEST_F(ViewModelUtilsTest, ItemTextColorRole)
{
  SessionItem item;

  // no color defined for item by default
  auto variant = utils::TextColorRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetEnabled(false);
  variant = utils::TextColorRole(item);
  EXPECT_EQ(variant.value<QColor>(), QColor(Qt::gray));

  item.SetEnabled(true);

  item.SetFlag(Appearance::kHighlighted, true);
  variant = utils::TextColorRole(item);
  EXPECT_EQ(variant.value<QColor>(), QColor(Qt::darkRed));

  item.SetData("#ff0000", DataRole::kTextColor);
  variant = utils::TextColorRole(item);
  EXPECT_EQ(variant.value<QColor>(), QColor(Qt::red));
}

TEST_F(ViewModelUtilsTest, ItemCheckStateRole)
{
  SessionItem item;

  // no color defined for item by default
  auto variant = utils::CheckStateRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetData(true);
  EXPECT_EQ(utils::CheckStateRole(item).value<int>(), Qt::Checked);

  item.SetData(false);
  EXPECT_EQ(utils::CheckStateRole(item).value<int>(), Qt::Unchecked);
}

TEST_F(ViewModelUtilsTest, StringDataDecorationRole)
{
  SessionItem item;

  // no color defined for item by default
  auto variant = utils::DecorationRole(item);
  EXPECT_FALSE(variant.isValid());

  const QColor expected(Qt::green);
  item.SetData(expected.name().toStdString());

  // just a color name doesn't generate color-based decoration role
  EXPECT_FALSE(utils::DecorationRole(item).isValid());

  // setting ColorEditor as default editor type, will generate proper decoration role
  item.SetEditorType(::mvvm::constants::kColorEditorType);

  EXPECT_EQ(utils::DecorationRole(item).value<QColor>(), expected);
}

TEST_F(ViewModelUtilsTest, ExternalPropertyDataDecorationRole)
{
  SessionItem item;

  const QColor expected(Qt::green);
  item.SetData(ExternalProperty("text", expected.name().toStdString()));

  EXPECT_EQ(utils::DecorationRole(item).value<QColor>(), expected);
}

TEST_F(ViewModelUtilsTest, ItemToolTipRole)
{
  SessionItem item;

  auto variant = utils::ToolTipRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(utils::ToolTipRole(item).toString(), QString("abc"));
}

TEST_F(ViewModelUtilsTest, ItemFromIndex)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();
  const AllItemsViewModel view_model(&model);

  // Difference from ViewModel::GetSessionItemFromIndex, which returns root item.
  EXPECT_EQ(utils::ItemFromIndex(QModelIndex()), nullptr);

  auto parent_index = view_model.index(0, 0);
  EXPECT_EQ(utils::ItemFromIndex(parent_index), parent);
  EXPECT_EQ(utils::ItemFromIndex(view_model.index(0, 0, parent_index)),
            parent->GetItem(VectorItem::kX));
}

//! Check ItemsFromIndex in PropertyTableViewModel context. ViewItem with its three property x, y, z
//! forms one row. All corresponding indices of (x,y,z) should give us pointers to VectorItem's
//! properties.
TEST_F(ViewModelUtilsTest, ItemsFromIndex)
{
  // creating VectorItem and viewModel to see it as a table
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();
  const PropertyTableViewModel viewModel(&model);

  // it's a table with one row and x,y,z columns
  EXPECT_EQ(viewModel.rowCount(), 1);
  EXPECT_EQ(viewModel.columnCount(), 3);

  // empty index list doesn't lead to SessionItem's
  QModelIndexList index_list;
  EXPECT_EQ(utils::ItemsFromIndex(index_list).size(), 0);

  // index list populated with column of properties
  index_list.push_back(viewModel.index(0, 0));
  index_list.push_back(viewModel.index(0, 1));
  index_list.push_back(viewModel.index(0, 2));

  std::vector<SessionItem*> expected = {parent->GetItem(VectorItem::kX),
                                        parent->GetItem(VectorItem::kY),
                                        parent->GetItem(VectorItem::kZ)};
  EXPECT_EQ(utils::ItemsFromIndex(index_list), expected);
}

//! Check ParentItemsFromIndex in PropertyTableViewModel context. ViewItem with its three property
//! x, y, z forms one row. All corresponding indices of (x,y,z) should give us pointer to
//! VectorItem.
TEST_F(ViewModelUtilsTest, ParentItemsFromIndex)
{
  // creating VectorItem and viewModel to see it as a table
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();
  PropertyTableViewModel viewModel(&model);

  // it's a table with one row and x,y,z columns
  EXPECT_EQ(viewModel.rowCount(), 1);
  EXPECT_EQ(viewModel.columnCount(), 3);

  // empty index list doesn't lead to SessionItem's
  QModelIndexList index_list;
  EXPECT_EQ(utils::ParentItemsFromIndex(index_list).size(), 0);

  std::vector<SessionItem*> expected = {parent};

  // one cell in a list should give us pointer to original VectorItem
  index_list.push_back(viewModel.index(0, 1));
  EXPECT_EQ(utils::ParentItemsFromIndex(index_list), expected);

  index_list.clear();
  index_list.push_back(viewModel.index(0, 1));
  EXPECT_EQ(utils::ParentItemsFromIndex(index_list), expected);

  index_list.clear();
  index_list.push_back(viewModel.index(0, 2));
  EXPECT_EQ(utils::ParentItemsFromIndex(index_list), expected);

  // three cells (x, y, z) in a list should give us pointer to original VectorItem
  index_list.clear();
  index_list.push_back(viewModel.index(0, 0));
  index_list.push_back(viewModel.index(0, 1));
  index_list.push_back(viewModel.index(0, 2));
  EXPECT_EQ(utils::ParentItemsFromIndex(index_list), expected);
}

TEST_F(ViewModelUtilsTest, GetQtRoles)
{
  SessionItem item;
  item.SetData(42);

  auto view_item = CreateDataViewItem(&item);

  auto roles = utils::GetQtRoles(view_item.get(), DataRole::kData);
  EXPECT_EQ(roles, QVector<int>({Qt::DisplayRole, Qt::EditRole}));
}

TEST_F(ViewModelUtilsTest, ItemFromProxyIndexForModelWithoutProxy)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();
  const AllItemsViewModel view_model(&model);

  // Difference from ViewModel::GetSessionItemFromIndex, which returns root item.
  EXPECT_EQ(utils::ItemFromProxyIndex(QModelIndex()), nullptr);

  auto parent_index = view_model.index(0, 0);
  EXPECT_EQ(utils::ItemFromProxyIndex(parent_index), parent);
  EXPECT_EQ(utils::ItemFromProxyIndex(view_model.index(0, 0, parent_index)),
            parent->GetItem(VectorItem::kX));
}

TEST_F(ViewModelUtilsTest, ItemFromProxyIndexForModelProxy)
{
  ApplicationModel model;
  auto parent = model.InsertItem<VectorItem>();
  AllItemsViewModel view_model(&model);

  FilterNameViewModel filtered_model;
  filtered_model.setSourceModel(&view_model);
  filtered_model.SetPattern("Y");

  auto parent_index = filtered_model.index(0, 0);
  ASSERT_EQ(filtered_model.rowCount(), 1);  // only "Y" item remains

  EXPECT_EQ(utils::ItemFromProxyIndex(parent_index), parent);
  EXPECT_EQ(utils::ItemFromProxyIndex(filtered_model.index(0, 0, parent_index)),
            parent->GetItem(VectorItem::kY));
}
