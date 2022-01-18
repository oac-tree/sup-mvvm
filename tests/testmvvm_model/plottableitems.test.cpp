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

#include "mvvm/standarditems/plottableitems.h"

#include <gtest/gtest.h>

class PlottableItemsTest : public ::testing::Test
{
};

TEST_F(PlottableItemsTest, TextItemInitialState)
{
  mvvm::TextItem item;
  EXPECT_TRUE(item.GetText().empty());
  EXPECT_EQ(item.GetFont(), std::string("Noto Sans"));
  EXPECT_EQ(item.GetSize(), 10);
}

TEST_F(PlottableItemsTest, TextItemGetSet)
{
  mvvm::TextItem item;

  item.SetText("abc");
  EXPECT_EQ(item.GetText(), std::string("abc"));

  item.SetFont("def");
  EXPECT_EQ(item.GetFont(), std::string("def"));

  item.SetSize(42);
  EXPECT_EQ(item.GetSize(), 42);
}
