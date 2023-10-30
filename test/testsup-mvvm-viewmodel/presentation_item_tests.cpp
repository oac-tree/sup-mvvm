/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodelbase/presentation_item.h"

#include <gtest/gtest.h>

//! Tests for PresentationItem class.

class PresentationItemTest : public ::testing::Test
{
public:
  class TestContext
  {
  public:
    int m_data{42};
  };
};

//! PresentationItem without context can't be used to modify/access the data.

TEST_F(PresentationItemTest, InitialState)
{
  mvvm::PresentationItem<TestContext> item{nullptr};
  EXPECT_EQ(item.GetContext(), nullptr);

  EXPECT_FALSE(item.Data().isValid());
  EXPECT_FALSE(item.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(item.Data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(item.Data(Qt::ToolTipRole).isValid());

  EXPECT_TRUE(item.IsEnabled());
  EXPECT_TRUE(item.IsEditable());

  EXPECT_FALSE(item.SetData(42, Qt::EditRole));
}

//! Callbacks validation.

TEST_F(PresentationItemTest, OnData)
{
  TestContext context;

  auto on_data = [](TestContext* context, int role)
  {
    (void)role;
    return QVariant(context->m_data);
  };

  auto on_setdata = [](TestContext* context, QVariant data, int role)
  {
    (void)role;
    context->m_data = data.toInt();
    return true;
  };

  mvvm::PresentationItem<TestContext> item{&context, on_data, on_setdata};
  EXPECT_EQ(item.GetContext(), &context);

  // getting back m_data as defined in the context
  EXPECT_EQ(item.Data(Qt::DisplayRole).toInt(), 42);

  // setting data
  EXPECT_TRUE(item.SetData(QVariant(43), Qt::DisplayRole));
  EXPECT_EQ(context.m_data, 43);
}
