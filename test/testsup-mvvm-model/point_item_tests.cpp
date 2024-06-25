/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/point_item.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for LineSeriesItem class.

class PointItemTest : public ::testing::Test
{
};

TEST_F(PointItemTest, GetAndSet)
{
  PointItem item;
  EXPECT_EQ(item.GetX(), 0.0);
  EXPECT_EQ(item.GetY(), 0.0);

  item.SetX(42.0);
  item.SetY(43.0);
  EXPECT_DOUBLE_EQ(item.GetX(), 42.0);
  EXPECT_DOUBLE_EQ(item.GetY(), 43.0);
}
