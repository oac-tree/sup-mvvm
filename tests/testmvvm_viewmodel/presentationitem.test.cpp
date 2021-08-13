/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
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

#include "mvvm/viewmodel/presentationitem.h"

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

TEST_F(PresentationItemTest, initialState)
{
  ModelView::PresentationItem<TestContext> item{nullptr};
  EXPECT_EQ(item.context(), nullptr);

  EXPECT_FALSE(item.data().isValid());
  EXPECT_FALSE(item.data(Qt::EditRole).isValid());
  EXPECT_FALSE(item.data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(item.data(Qt::ToolTipRole).isValid());

  EXPECT_TRUE(item.isEnabled());
  EXPECT_TRUE(item.isEditable());

  EXPECT_FALSE(item.setData(42, Qt::EditRole));
}

//! Callbacks validation.

TEST_F(PresentationItemTest, onData)
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

  ModelView::PresentationItem<TestContext> item{&context, on_data, on_setdata};
  EXPECT_EQ(item.context(), &context);

  // getting back m_data as defined in the context
  EXPECT_EQ(item.data(Qt::DisplayRole).toInt(), 42);

  // setting data
  EXPECT_TRUE(item.setData(QVariant(43), Qt::DisplayRole));
  EXPECT_EQ(context.m_data, 43);
}
