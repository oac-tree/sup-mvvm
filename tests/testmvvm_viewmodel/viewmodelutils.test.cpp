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

#include "mvvm/viewmodel/viewmodelutils.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/standarditems/vectoritem.h"

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

class ViewModelUtilsTest : public ::testing::Test
{
public:
};

//! Validate Utils::GetItem
//! FIXME refactor GetContext and GetItem method, they are errorprone
//! The result depend on the way PresentationItem has been created

TEST_F(ViewModelUtilsTest, GetItem)
{
  VectorItem item;

  auto presentation1 = std::make_unique<PresentationItem<SessionItem>>(&item);
  mvvm::ViewItem view_item1(std::move(presentation1));

//  EXPECT_EQ(utils::GetContext<SessionItem>(&view_item1), &item);
//  EXPECT_NE(utils::GetContext<VectorItem>(&view_item1), &item);
  EXPECT_EQ(utils::GetItem<VectorItem>(&view_item1), &item);

  auto presentation2 = std::make_unique<PresentationItem<VectorItem>>(&item);
  mvvm::ViewItem view_item2(std::move(presentation2));

  EXPECT_NE(utils::GetContext<SessionItem>(&view_item2), &item);
//  EXPECT_EQ(utils::GetContext<VectorItem>(&view_item2), &item);
//  EXPECT_NE(utils::GetItem<VectorItem>(&view_item2), &item);
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

//! Testing decoration role of the item.
//! FIXME restore

// TEST_F(ViewModelUtilsTest, itemDecorationRole)
//{
//   SessionItem item("Something");

//  // no color defined for item by default
//  auto variant = Utils::DecorationRole(item);
//  EXPECT_FALSE(variant.isValid());

//  QColor expected(Qt::green);
//  item.setData(expected);
//  EXPECT_EQ(Utils::DecorationRole(item).value<QColor>(), expected);
//}

//! Testing tooltip role of the item.

TEST_F(ViewModelUtilsTest, ItemToolTipRole)
{
  SessionItem item;

  auto variant = utils::ToolTipRole(item);
  EXPECT_FALSE(variant.isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(utils::ToolTipRole(item).toString(), QString("abc"));
}
