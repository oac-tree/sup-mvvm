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

#include "mvvm/model/item_selection.h"

#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/vector_item.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests of ItemSelection class.
 */
class ItemSelectionTest : public ::testing::Test
{
};

TEST_F(ItemSelectionTest, InitialState)
{
  const ItemSelection selection;
  EXPECT_EQ(selection.GetSelectedItem(), nullptr);
  EXPECT_TRUE(selection.GetSelectedItems().empty());
}

TEST_F(ItemSelectionTest, SelectionFromNullptr)
{
  const ItemSelection selection(nullptr);

  EXPECT_EQ(selection.GetSelectedItem(), nullptr);
  EXPECT_TRUE(selection.GetSelectedItems().empty());
  EXPECT_TRUE(selection.IsEmpty());
}

TEST_F(ItemSelectionTest, SelectionFromSinglePointer)
{
  { // const pointer
    const SessionItem item;
    const ItemSelection selection(&item);
    EXPECT_EQ(selection.GetSelectedItem(), &item);
    const std::vector<const SessionItem*> expected{&item};
    EXPECT_EQ(selection.GetSelectedItems(), expected);
    EXPECT_FALSE(selection.IsEmpty());
  }

  { // non-const pointer
    SessionItem item; // non-const
    const ItemSelection selection(&item);
    EXPECT_EQ(selection.GetSelectedItem(), &item);
    const std::vector<const SessionItem*> expected{&item};
    EXPECT_EQ(selection.GetSelectedItems(), expected);
  }

  { // pointer to derived class
    // to validate that our templated constructor does not cause any issues
    const VectorItem vector_item;
    const ItemSelection selection(&vector_item);
    EXPECT_EQ(selection.GetSelectedItem(), &vector_item);
    const std::vector<const SessionItem*> expected{&vector_item};
    EXPECT_EQ(selection.GetSelectedItems(), expected);
  }
}

TEST_F(ItemSelectionTest, SelectionFromSinglePointerWhichIsNullptr)
{
  const SessionItem* item{nullptr};
  const ItemSelection selection(item);
  EXPECT_EQ(selection.GetSelectedItem(), nullptr);
  EXPECT_TRUE(selection.GetSelectedItems().empty());
}

TEST_F(ItemSelectionTest, TwoItemsInSelection)
{
  const SessionItem item1;
  const SessionItem item2;

  const ItemSelection selection({&item1, &item2});

  EXPECT_EQ(selection.GetSelectedItem(), &item1);
  const std::vector<const SessionItem*> expected{&item1, &item2};
  EXPECT_EQ(selection.GetSelectedItems(), expected);
}

TEST_F(ItemSelectionTest, CastedGetters)
{
  const VectorItem item1;
  const VectorItem item2;

  const ItemSelection selection({&item1, &item2});

  const std::vector<const SessionItem*> expected1{&item1, &item2};
  EXPECT_EQ(selection.GetSelectedItems(), expected1);

  // casting to type
  const std::vector<const VectorItem*> expected2{&item1, &item2};
  EXPECT_EQ(selection.GetSelectedItems<VectorItem>(), expected2);

  EXPECT_EQ(selection.GetSelected<VectorItem>(), &item1);
}

TEST_F(ItemSelectionTest, TemplatedConstructionFromVector)
{
  const VectorItem item1;
  const VectorItem item2;

  const std::vector<const VectorItem*> vector_items{&item1, &item2};

  // this invokes template constructor, which should properly cast items to SessionItem pointers
  const ItemSelection selection(vector_items);

  const std::vector<const SessionItem*> expected1{&item1, &item2};
  EXPECT_EQ(selection.GetSelectedItems(), expected1);
  EXPECT_EQ(selection.GetSelectedItems<VectorItem>(), vector_items);
}

TEST_F(ItemSelectionTest, IsSameSelection)
{
  const SessionItem item1;
  const SessionItem item2;

  const ItemSelection selection1({&item1, &item2});
  const ItemSelection selection2({&item2, &item1});
  const ItemSelection selection3({&item1});

  EXPECT_TRUE(selection1.IsSameSelection(selection2));
  EXPECT_FALSE(selection1.IsSameSelection(selection3));
}

}  // namespace mvvm::test
