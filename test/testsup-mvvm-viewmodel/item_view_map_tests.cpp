/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/providers/item_view_map.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/vector_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for ItemViewMap class.
 */
class ItemViewMapTest : public ::testing::Test
{
public:
  class TestView
  {
  };
};

TEST_F(ItemViewMapTest, InitialState)
{
  const ItemViewMap<SessionItem*, TestView*> map;
  EXPECT_EQ(map.GetSize(), 0U);
}

TEST_F(ItemViewMapTest, InsertThenFind)
{
  ItemViewMap<const SessionItem*, const TestView*> map;
  const SessionItem item;
  const TestView view;

  EXPECT_EQ(map.FindView(&item), nullptr);
  map.Insert(&item, &view);
  EXPECT_EQ(map.FindView(&item), &view);
  EXPECT_EQ(map.GetSize(), 1U);

  map.Clear();
  EXPECT_EQ(map.GetSize(), 0U);
}

TEST_F(ItemViewMapTest, InsertThenRemove)
{
  ItemViewMap<const SessionItem*, const TestView*> map;
  const SessionItem item;
  const TestView view;

  map.Insert(&item, &view);
  EXPECT_EQ(map.FindView(&item), &view);

  map.Remove(&item);
  EXPECT_EQ(map.FindView(&item), nullptr);
}

TEST_F(ItemViewMapTest, InsertThenRemoveByValue)
{
  ItemViewMap<const SessionItem*, const TestView*> map;
  const SessionItem item;
  const TestView view;

  map.Insert(&item, &view);
  EXPECT_EQ(map.FindView(&item), &view);

  map.Remove(&view);
  EXPECT_EQ(map.FindView(&item), nullptr);
}

TEST_F(ItemViewMapTest, OnItemRemove)
{
  ItemViewMap<const SessionItem*, const TestView*> map;
  const SessionItem item;
  const VectorItem vector;

  const TestView item_view;
  const TestView vector_view;
  const TestView x_view;
  const TestView y_view;
  const TestView z_view;

  map.Insert(&vector, &vector_view);
  map.Insert(vector.GetItem(VectorItem::kX), &x_view);
  map.Insert(&item, &item_view);
  map.Insert(vector.GetItem(VectorItem::kY), &y_view);
  map.Insert(vector.GetItem(VectorItem::kZ), &z_view);

  EXPECT_EQ(map.GetSize(), 5);

  auto removed = map.RemoveRecursively(&vector);
  EXPECT_EQ(removed.size(), 4);
  EXPECT_EQ(map.GetSize(), 1);
}

TEST_F(ItemViewMapTest, FindItemForView)
{
  ItemViewMap<const SessionItem*, const TestView*> map;
  const SessionItem item0;
  const SessionItem item1;

  const TestView view0;
  const TestView view1;
  const TestView view2;

  map.Insert(&item0, &view0);
  map.Insert(&item1, &view1);

  EXPECT_EQ(map.FindItem(&view0), &item0);
  EXPECT_EQ(map.FindItem(&view1), &item1);
  EXPECT_EQ(map.FindItem(&view2), nullptr);
}

}  // namespace mvvm::test
