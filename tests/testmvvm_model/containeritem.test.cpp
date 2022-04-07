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

#include "mvvm/standarditems/containeritem.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Test of ContainerItem.

class ContainerItemTest : public ::testing::Test
{
};

TEST_F(ContainerItemTest, InitialState)
{
  ContainerItem item;
  EXPECT_TRUE(item.IsEmpty());
}

TEST_F(ContainerItemTest, IsEmpty)
{
  ContainerItem item;
  item.InsertItem<PropertyItem>({"", 0});
  EXPECT_FALSE(item.IsEmpty());
}

TEST_F(ContainerItemTest, GetSize)
{
  ContainerItem item;
  EXPECT_EQ(item.GetSize(), 0);
  item.InsertItem<PropertyItem>({"", 0});
  EXPECT_FALSE(item.IsEmpty());
  EXPECT_EQ(item.GetSize(), 1);
}
