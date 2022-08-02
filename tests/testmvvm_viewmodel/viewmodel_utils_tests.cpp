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

#include "mvvm/viewmodel/viewmodel_utils.h"

#include <gtest/gtest.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/property_table_viewmodel.h>
#include <mvvm/viewmodel/viewitem_factory.h>

#include <QColor>

using namespace mvvm;

class ViewModelUtilsTests : public ::testing::Test
{
public:
};

//! Validate Utils::GetItemFromView

TEST_F(ViewModelUtilsTests, GetItemFromView)
{
  VectorItem item;

  auto view = CreateDataViewItem(&item);
  EXPECT_EQ(utils::GetItemFromView<VectorItem>(view.get()), &item);
}

//! Validate Utils::iterate_model function with user callback.

TEST_F(ViewModelUtilsTests, ItemRoleToQtRole)
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

//! Testing color role of item.

TEST_F(ViewModelUtilsTests, ItemTextColorRole)
{
  SessionItem item;

  // no color defined for item by default
  auto variant = utils::TextColorRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetEnabled(false);
  variant = utils::TextColorRole(item);
  EXPECT_EQ(variant.value<QColor>(), QColor(Qt::gray));
}

//! Testing check state role of item.

TEST_F(ViewModelUtilsTests, ItemCheckStateRole)
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

//! Testing decoration role of the item when it carries a string

TEST_F(ViewModelUtilsTests, StringDataDecorationRole)
{
  SessionItem item;

  // no color defined for item by default
  auto variant = utils::DecorationRole(item);
  EXPECT_FALSE(variant.isValid());

  QColor expected(Qt::green);
  item.SetData(expected.name().toStdString());

  // just a color name doesn't generate color-based decoration role
  EXPECT_FALSE(utils::DecorationRole(item).isValid());

  // setting ColorEditor as default editor type, will generate proper decoration role
  item.SetEditorType(::mvvm::constants::kColorEditorType);

  EXPECT_EQ(utils::DecorationRole(item).value<QColor>(), expected);
}

//! Testing decoration role of the item when it carries ExtendedProperty

TEST_F(ViewModelUtilsTests, ExternalPropertyDataDecorationRole)
{
  SessionItem item;

  QColor expected(Qt::green);
  item.SetData(ExternalProperty("text", expected.name().toStdString()));

  EXPECT_EQ(utils::DecorationRole(item).value<QColor>(), expected);
}

//! Testing tooltip role of the item.

TEST_F(ViewModelUtilsTests, ItemToolTipRole)
{
  SessionItem item;

  auto variant = utils::ToolTipRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(utils::ToolTipRole(item).toString(), QString("abc"));
}

//! Check ItemsFromIndex in PropertyTableViewModel context.
//! ViewItem with its three property x, y, z forms one row. All corresponding
//! indices of (x,y,z) should give us pointers to VectorItem's properties.

TEST_F(ViewModelUtilsTests, itemsFromIndex)
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

//! Check ParentItemsFromIndex in PropertyTableViewModel context.
//! ViewItem with its three property x, y, z forms one row. All corresponding
//! indices of (x,y,z) should give us pointer to VectorItem.

TEST_F(ViewModelUtilsTests, parentItemsFromIndex)
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

  // tthree cells (x, y, z) in a list should give us pointer to original VectorItem
  index_list.clear();
  index_list.push_back(viewModel.index(0, 0));
  index_list.push_back(viewModel.index(0, 1));
  index_list.push_back(viewModel.index(0, 2));
  EXPECT_EQ(utils::ParentItemsFromIndex(index_list), expected);
}
