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
  EXPECT_EQ(selection.GetItem(), nullptr);
  EXPECT_TRUE(selection.GetItems().empty());
}

TEST_F(ItemSelectionTest, SelectionFromNullptr)
{
  const ItemSelection selection(nullptr);

  EXPECT_EQ(selection.GetItem(), nullptr);
  EXPECT_TRUE(selection.GetItems().empty());
  EXPECT_TRUE(selection.IsEmpty());
}

TEST_F(ItemSelectionTest, SelectionFromSinglePointer)
{
  {  // const pointer: selecting an item only reads it, so const items are accepted
    const SessionItem item;
    const ItemSelection selection(&item);
    EXPECT_EQ(selection.GetItem(), &item);
    EXPECT_EQ(selection.GetItems().size(), 1);
    EXPECT_FALSE(selection.IsEmpty());
  }

  {  // a const selection still reports mutable items, it refers to them rather than owning them
    SessionItem item;
    const ItemSelection selection(&item);
    EXPECT_EQ(selection.GetItem(), &item);
    const std::vector<SessionItem*> expected{&item};
    EXPECT_EQ(selection.GetItems(), expected);
  }

  {  // pointer to derived class
    // to validate that our templated constructor does not cause any issues
    VectorItem vector_item;
    const ItemSelection selection(&vector_item);
    EXPECT_EQ(selection.GetItem(), &vector_item);
    const std::vector<SessionItem*> expected{&vector_item};
    EXPECT_EQ(selection.GetItems(), expected);
  }
}

TEST_F(ItemSelectionTest, SelectionFromSinglePointerWhichIsNullptr)
{
  const SessionItem* item{nullptr};
  const ItemSelection selection(item);
  EXPECT_EQ(selection.GetItem(), nullptr);
  EXPECT_TRUE(selection.GetItems().empty());
}

TEST_F(ItemSelectionTest, TwoItemsInSelection)
{
  SessionItem item1;
  SessionItem item2;

  const ItemSelection selection({&item1, &item2});

  EXPECT_EQ(selection.GetItem(), &item1);
  const std::vector<SessionItem*> expected{&item1, &item2};
  EXPECT_EQ(selection.GetItems(), expected);
}

TEST_F(ItemSelectionTest, CastedGetters)
{
  VectorItem item1;
  VectorItem item2;

  const ItemSelection selection({&item1, &item2});

  const std::vector<SessionItem*> expected1{&item1, &item2};
  EXPECT_EQ(selection.GetItems(), expected1);

  // casting to type
  const std::vector<VectorItem*> expected2{&item1, &item2};
  EXPECT_EQ(selection.GetItems<VectorItem>(), expected2);

  EXPECT_EQ(selection.GetItem<VectorItem>(), &item1);
}

TEST_F(ItemSelectionTest, TemplatedConstructionFromVector)
{
  VectorItem item1;
  VectorItem item2;

  // the template constructor accepts const items too, selecting them only reads them
  const std::vector<const VectorItem*> const_vector_items{&item1, &item2};
  const ItemSelection const_selection(const_vector_items);
  EXPECT_EQ(const_selection.GetItems().size(), 2);

  const std::vector<VectorItem*> vector_items{&item1, &item2};

  // this invokes template constructor, which should properly cast items to SessionItem pointers
  const ItemSelection selection(vector_items);

  const std::vector<SessionItem*> expected1{&item1, &item2};
  EXPECT_EQ(selection.GetItems(), expected1);
  EXPECT_EQ(selection.GetItems<VectorItem>(), vector_items);
}

TEST_F(ItemSelectionTest, IsSameSelection)
{
  SessionItem item1;
  SessionItem item2;

  const ItemSelection selection1({&item1, &item2});
  const ItemSelection selection2({&item2, &item1});
  const ItemSelection selection3({&item1});

  EXPECT_TRUE(selection1.IsSameSelection(selection2));
  EXPECT_FALSE(selection1.IsSameSelection(selection3));
}

}  // namespace mvvm::test
