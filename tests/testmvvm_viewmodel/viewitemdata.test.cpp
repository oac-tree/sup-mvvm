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

#include "mvvm/viewmodelbase/viewitem_data.h"

#include <gtest/gtest.h>

//! Tests for ViewItemData class.

class ViewItemDataTest : public ::testing::Test
{
public:
};

//! Initial state.

TEST_F(ViewItemDataTest, InitialState)
{
  mvvm::ViewItemData item;
  EXPECT_FALSE(item.Data().isValid());
  EXPECT_FALSE(item.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(item.Data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(item.Data(Qt::ToolTipRole).isValid());

  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
}

TEST_F(ViewItemDataTest, SetAppearance)
{
  mvvm::ViewItemData item;

  item.SetEditable(false);
  EXPECT_TRUE(item.IsEnabled());
  EXPECT_FALSE(item.IsEditable());

  item.SetEnabled(false);
  EXPECT_FALSE(item.IsEnabled());
  EXPECT_FALSE(item.IsEditable());
}

TEST_F(ViewItemDataTest, SetData)
{
  mvvm::ViewItemData item;

  EXPECT_FALSE(item.Data().isValid());

  EXPECT_TRUE(item.SetData(QVariant::fromValue(42)));
  EXPECT_EQ(item.Data().toInt(), 42);

  EXPECT_TRUE(item.SetData(QVariant::fromValue(42)));
  EXPECT_EQ(item.Data().toInt(), 42);

  EXPECT_TRUE(item.SetData(QVariant::fromValue(1.2), Qt::EditRole));
  EXPECT_EQ(item.Data().toDouble(), 1.2);

  EXPECT_TRUE(item.SetData(QString("abc"), Qt::DisplayRole));
  EXPECT_EQ(item.Data(Qt::DisplayRole).toString(), QString("abc"));

  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());
}
