/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "mvvm/viewmodelbase/viewitem.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>
#include <testutils/test_container_helper.h>

using namespace mvvm;

//! Tests for ViewItem class.

class ViewItemTest : public ::testing::Test
{
public:
  using children_t = std::vector<std::unique_ptr<ViewItem>>;
  using expected_t = std::vector<ViewItem*>;

  //! Helper function to get two vectors, each ncolumns length, in the form of a pair.
  //! First vector contains unique_ptr objects, second vector bare pointers to same objects.
  //! First vector is intended to be moved inside a model, second vector is to validate
  //! the content of a model after the move.

  std::pair<children_t, expected_t> GetTestData(int ncolumns)
  {
    auto vector_of_unique = mvvm::test::CreateRow<ViewItem, ViewItem>(ncolumns);
    auto vector_of_pointers = mvvm::utils::GetVectorOfPtrs(vector_of_unique);
    return std::make_pair(std::move(vector_of_unique), std::move(vector_of_pointers));
  }
};

//! Initial state of RefViewItem.

TEST_F(ViewItemTest, InitialState)
{
  ViewItem view_item;

  EXPECT_EQ(view_item.GetRowCount(), 0);
  EXPECT_EQ(view_item.GetColumnCount(), 0);
  EXPECT_EQ(view_item.Row(), -1);
  EXPECT_EQ(view_item.Column(), -1);
  EXPECT_EQ(view_item.GetParent(), nullptr);
  EXPECT_THROW(view_item.GetChild(0, 0), RuntimeException);
  EXPECT_NE(view_item.GetItemData(), nullptr);  // has ViewItemData on board
  EXPECT_FALSE(view_item.HasChildren());
}

//! Append single item as Row.

TEST_F(ViewItemTest, AppendRow)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  // appending row with single item
  ViewItem view_item;
  view_item.AppendRow(std::move(children));
  EXPECT_TRUE(view_item.HasChildren());

  // checking parent
  EXPECT_EQ(view_item.GetRowCount(), 1);
  EXPECT_EQ(view_item.GetColumnCount(), 1);
  EXPECT_EQ(view_item.GetChild(0, 0), expected[0]);
  EXPECT_THROW(view_item.GetChild(0, 1), RuntimeException);

  // checking appended child
  EXPECT_EQ(expected[0]->GetParent(), &view_item);
  EXPECT_EQ(expected[0]->Row(), 0);
  EXPECT_EQ(expected[0]->Column(), 0);
}

//! Remove row.

TEST_F(ViewItemTest, RemoveRow)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  // appending row with single item
  ViewItem view_item;
  view_item.AppendRow(std::move(children));
  view_item.RemoveRow(0);

  // checking parent
  EXPECT_EQ(view_item.GetRowCount(), 0);
  EXPECT_EQ(view_item.GetColumnCount(), 0);
}

//! Append two rows with two items each.

TEST_F(ViewItemTest, AppendTwoRows)
{
  // preparing two rows of children, two columns each
  auto [children_row0, expected_row0] = GetTestData(/*ncolumns*/ 2);
  auto [children_row1, expected_row1] = GetTestData(/*ncolumns*/ 2);

  // appending rows
  ViewItem view_item;
  view_item.AppendRow(std::move(children_row0));
  view_item.AppendRow(std::move(children_row1));

  EXPECT_EQ(view_item.GetRowCount(), 2);
  EXPECT_EQ(view_item.GetColumnCount(), 2);
  EXPECT_EQ(view_item.GetChild(0, 0), expected_row0[0]);
  EXPECT_EQ(view_item.GetChild(0, 1), expected_row0[1]);
  EXPECT_EQ(view_item.GetChild(1, 0), expected_row1[0]);
  EXPECT_EQ(view_item.GetChild(1, 1), expected_row1[1]);
  EXPECT_THROW(view_item.GetChild(2, 2), RuntimeException);

  // checking parents
  EXPECT_EQ(expected_row0[0]->GetParent(), &view_item);
  EXPECT_EQ(expected_row0[1]->GetParent(), &view_item);
  EXPECT_EQ(expected_row1[0]->GetParent(), &view_item);
  EXPECT_EQ(expected_row1[1]->GetParent(), &view_item);

  // checking row and column of children
  EXPECT_EQ(expected_row0[0]->Row(), 0);
  EXPECT_EQ(expected_row0[1]->Row(), 0);
  EXPECT_EQ(expected_row1[0]->Row(), 1);
  EXPECT_EQ(expected_row1[1]->Row(), 1);
  EXPECT_EQ(expected_row0[0]->Column(), 0);
  EXPECT_EQ(expected_row0[1]->Column(), 1);
  EXPECT_EQ(expected_row1[0]->Column(), 0);
  EXPECT_EQ(expected_row1[1]->Column(), 1);

  // attempt to add row with different amount of children should fail
  auto [children_row2, expected_row2] = GetTestData(/*ncolumns*/ 1);
  EXPECT_THROW(view_item.AppendRow(std::move(children_row2)), RuntimeException);
  EXPECT_EQ(view_item.GetRowCount(), 2);
  EXPECT_EQ(view_item.GetColumnCount(), 2);
}

//! Append two rows with two items each.

TEST_F(ViewItemTest, InsertRowsThenRemove)
{
  // preparing two rows of children, two columns each
  auto [children_row0, expected_row0] = GetTestData(/*ncolumns*/ 2);
  auto [children_row1, expected_row1] = GetTestData(/*ncolumns*/ 2);
  auto [children_row2, expected_row2] = GetTestData(/*ncolumns*/ 2);

  // appending rows
  ViewItem view_item;
  view_item.AppendRow(std::move(children_row0));
  view_item.AppendRow(std::move(children_row1));
  view_item.InsertRow(1, std::move(children_row2));  // inserting in-between

  EXPECT_EQ(view_item.GetRowCount(), 3);
  EXPECT_EQ(view_item.GetColumnCount(), 2);
  EXPECT_EQ(view_item.GetChild(0, 0), expected_row0[0]);
  EXPECT_EQ(view_item.GetChild(0, 1), expected_row0[1]);
  EXPECT_EQ(view_item.GetChild(1, 0), expected_row2[0]);
  EXPECT_EQ(view_item.GetChild(1, 1), expected_row2[1]);
  EXPECT_EQ(view_item.GetChild(2, 0), expected_row1[0]);
  EXPECT_EQ(view_item.GetChild(2, 1), expected_row1[1]);

  // checking parents
  EXPECT_EQ(expected_row0[0]->GetParent(), &view_item);
  EXPECT_EQ(expected_row0[1]->GetParent(), &view_item);
  EXPECT_EQ(expected_row1[0]->GetParent(), &view_item);
  EXPECT_EQ(expected_row1[1]->GetParent(), &view_item);
  EXPECT_EQ(expected_row2[0]->GetParent(), &view_item);
  EXPECT_EQ(expected_row2[1]->GetParent(), &view_item);

  // checking row and column of children
  EXPECT_EQ(expected_row0[0]->Row(), 0);
  EXPECT_EQ(expected_row0[1]->Row(), 0);
  EXPECT_EQ(expected_row1[0]->Row(), 2);
  EXPECT_EQ(expected_row1[1]->Row(), 2);
  EXPECT_EQ(expected_row2[0]->Row(), 1);
  EXPECT_EQ(expected_row2[1]->Row(), 1);
  EXPECT_EQ(expected_row0[0]->Column(), 0);
  EXPECT_EQ(expected_row0[1]->Column(), 1);
  EXPECT_EQ(expected_row1[0]->Column(), 0);
  EXPECT_EQ(expected_row1[1]->Column(), 1);
  EXPECT_EQ(expected_row2[0]->Column(), 0);
  EXPECT_EQ(expected_row2[1]->Column(), 1);

  // removing middle row
  view_item.RemoveRow(1);
  EXPECT_EQ(view_item.GetChild(0, 0), expected_row0[0]);
  EXPECT_EQ(view_item.GetChild(0, 1), expected_row0[1]);
  EXPECT_EQ(view_item.GetChild(1, 0), expected_row1[0]);
  EXPECT_EQ(view_item.GetChild(1, 1), expected_row1[1]);

  // checking row and column of children again
  EXPECT_EQ(expected_row0[0]->Row(), 0);
  EXPECT_EQ(expected_row0[1]->Row(), 0);
  EXPECT_EQ(expected_row1[0]->Row(), 1);
  EXPECT_EQ(expected_row1[1]->Row(), 1);
  EXPECT_EQ(expected_row0[0]->Column(), 0);
  EXPECT_EQ(expected_row0[1]->Column(), 1);
  EXPECT_EQ(expected_row1[0]->Column(), 0);
  EXPECT_EQ(expected_row1[1]->Column(), 1);
}

//! Clean item's children.

TEST_F(ViewItemTest, Clear)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  ViewItem view_item;
  view_item.AppendRow(std::move(children));
  view_item.Clear();

  EXPECT_EQ(view_item.GetRowCount(), 0);
  EXPECT_EQ(view_item.GetColumnCount(), 0);
}

TEST_F(ViewItemTest, Children)
{
  auto [children_row0, expected_row0] = GetTestData(/*ncolumns*/ 2);
  auto [children_row1, expected_row1] = GetTestData(/*ncolumns*/ 2);

  ViewItem view_item;
  view_item.AppendRow(std::move(children_row0));
  view_item.AppendRow(std::move(children_row1));

  std::vector<ViewItem*> expected;
  std::copy(expected_row0.begin(), expected_row0.end(), std::back_inserter(expected));
  std::copy(expected_row1.begin(), expected_row1.end(), std::back_inserter(expected));

  EXPECT_EQ(view_item.GetChildren(), expected);
}

TEST_F(ViewItemTest, GetFlags)
{
  ViewItem view_item;

  EXPECT_TRUE(view_item.Flags() & Qt::ItemIsSelectable);
  EXPECT_TRUE(view_item.Flags() & Qt::ItemIsEnabled);

  // Current implementation is that ViewItem is editable by default
  EXPECT_TRUE(view_item.Flags() & Qt::ItemIsEditable);
  view_item.SetData(42, Qt::EditRole);
  EXPECT_TRUE(view_item.Flags() & Qt::ItemIsEditable);
}
