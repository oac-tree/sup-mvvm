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

#include "mvvm/viewmodel/viewitemmap.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodelbase/viewitem.h"

#include <gtest/gtest.h>

using namespace ModelView;

//! Tests for InstructionViewItemMap class.

class ViewItemMapTest : public ::testing::Test
{
};

//! The map is initially empty.

TEST_F(ViewItemMapTest, InsertThenFind)
{
  ViewItemMap map;
  SessionItem item;
  ViewItem view;

  // initially no view for instruction
  EXPECT_FALSE(map.FindView(&item));

  // adding pair instruction/view
  map.Insert(&item, &view);
  EXPECT_TRUE(map.FindView(&item));
  EXPECT_EQ(map.FindView(&item), &view);

  // clearing, should be empty now
  map.Clear();
  EXPECT_FALSE(map.FindView(&item));
}

TEST_F(ViewItemMapTest, InsertThenRemove)
{
  ViewItemMap map;
  SessionItem item;
  ViewItem view;

  // adding pair instruction/view
  map.Insert(&item, &view);
  EXPECT_TRUE(map.FindView(&item));

  map.Remove(&item);
  EXPECT_FALSE(map.FindView(&item));
}

TEST_F(ViewItemMapTest, Update)
{
  ViewItemMap map;
  SessionItem item;
  ViewItem view1;
  ViewItem view2;

  map.Update(&item, &view1);
  EXPECT_EQ(map.FindView(&item), &view1);

  map.Update(&item, &view2);
  EXPECT_EQ(map.FindView(&item), &view2);
}
