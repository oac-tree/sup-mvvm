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

#include "mvvm/viewmodel/viewmodelutils.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/standarditems/editor_constants.h"
#include "mvvm/standarditems/vectoritem.h"
#include "mvvm/viewmodel/viewitemfactory.h"

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

class ViewModelUtilsTest : public ::testing::Test
{
public:
};

//! Validate Utils::GetItemFromView

TEST_F(ViewModelUtilsTest, GetItemFromView)
{
  VectorItem item;

  auto view = CreateDataViewItem(&item);
  EXPECT_EQ(utils::GetItemFromView<VectorItem>(view.get()), &item);
}

//! Validate Utils::iterate_model function with user callback.

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

//! Testing color role of item.

TEST_F(ViewModelUtilsTest, ItemTextColorRole)
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

//! Testing decoration role of the item when it carries a string

TEST_F(ViewModelUtilsTest, StringDataDecorationRole)
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

TEST_F(ViewModelUtilsTest, ExternalPropertyDataDecorationRole)
{
  SessionItem item;

  QColor expected(Qt::green);
  item.SetData(ExternalProperty("text", expected.name().toStdString()));

  EXPECT_EQ(utils::DecorationRole(item).value<QColor>(), expected);
}

//! Testing tooltip role of the item.

TEST_F(ViewModelUtilsTest, ItemToolTipRole)
{
  SessionItem item;

  auto variant = utils::ToolTipRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(utils::ToolTipRole(item).toString(), QString("abc"));
}
