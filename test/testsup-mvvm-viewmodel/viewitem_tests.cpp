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

#include "mvvm/viewmodelbase/viewitem.h"

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <stdexcept>

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
    auto vector_of_unique = testutils::CreateRow<ViewItem, ViewItem>(ncolumns);
    auto vector_of_pointers = testutils::GetPointers(vector_of_unique);
    return std::make_pair(std::move(vector_of_unique), std::move(vector_of_pointers));
  }
};

//! Initial state of RefViewItem.

TEST_F(ViewItemTest, InitialState)
{
  ViewItem view_item;

  EXPECT_EQ(view_item.rowCount(), 0);
  EXPECT_EQ(view_item.columnCount(), 0);
  EXPECT_EQ(view_item.row(), -1);
  EXPECT_EQ(view_item.column(), -1);
  EXPECT_EQ(view_item.parent(), nullptr);
  EXPECT_THROW(view_item.child(0, 0), std::runtime_error);
  EXPECT_NE(view_item.item(), nullptr);  // has ViewItemData on board
}

//! Append single item as row.

TEST_F(ViewItemTest, AppendRow)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  // appending row with single item
  ViewItem view_item;
  view_item.appendRow(std::move(children));

  // checking parent
  EXPECT_EQ(view_item.rowCount(), 1);
  EXPECT_EQ(view_item.columnCount(), 1);
  EXPECT_EQ(view_item.child(0, 0), expected[0]);
  EXPECT_THROW(view_item.child(0, 1), std::runtime_error);

  // checking appended child
  EXPECT_EQ(expected[0]->parent(), &view_item);
  EXPECT_EQ(expected[0]->row(), 0);
  EXPECT_EQ(expected[0]->column(), 0);
}

//! Remove row.

TEST_F(ViewItemTest, RemoveRow)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  // appending row with single item
  ViewItem view_item;
  view_item.appendRow(std::move(children));
  view_item.removeRow(0);

  // checking parent
  EXPECT_EQ(view_item.rowCount(), 0);
  EXPECT_EQ(view_item.columnCount(), 0);
}

//! Append two rows with two items each.

TEST_F(ViewItemTest, AppendTwoRows)
{
  // preparing two rows of children, two columns each
  auto [children_row0, expected_row0] = GetTestData(/*ncolumns*/ 2);
  auto [children_row1, expected_row1] = GetTestData(/*ncolumns*/ 2);

  // appending rows
  ViewItem view_item;
  view_item.appendRow(std::move(children_row0));
  view_item.appendRow(std::move(children_row1));

  EXPECT_EQ(view_item.rowCount(), 2);
  EXPECT_EQ(view_item.columnCount(), 2);
  EXPECT_EQ(view_item.child(0, 0), expected_row0[0]);
  EXPECT_EQ(view_item.child(0, 1), expected_row0[1]);
  EXPECT_EQ(view_item.child(1, 0), expected_row1[0]);
  EXPECT_EQ(view_item.child(1, 1), expected_row1[1]);
  EXPECT_THROW(view_item.child(2, 2), std::runtime_error);

  // checking parents
  EXPECT_EQ(expected_row0[0]->parent(), &view_item);
  EXPECT_EQ(expected_row0[1]->parent(), &view_item);
  EXPECT_EQ(expected_row1[0]->parent(), &view_item);
  EXPECT_EQ(expected_row1[1]->parent(), &view_item);

  // checking row and column of children
  EXPECT_EQ(expected_row0[0]->row(), 0);
  EXPECT_EQ(expected_row0[1]->row(), 0);
  EXPECT_EQ(expected_row1[0]->row(), 1);
  EXPECT_EQ(expected_row1[1]->row(), 1);
  EXPECT_EQ(expected_row0[0]->column(), 0);
  EXPECT_EQ(expected_row0[1]->column(), 1);
  EXPECT_EQ(expected_row1[0]->column(), 0);
  EXPECT_EQ(expected_row1[1]->column(), 1);

  // attempt to add row with different amount of children should fail
  auto [children_row2, expected_row2] = GetTestData(/*ncolumns*/ 1);
  EXPECT_THROW(view_item.appendRow(std::move(children_row2)), std::runtime_error);
  EXPECT_EQ(view_item.rowCount(), 2);
  EXPECT_EQ(view_item.columnCount(), 2);
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
  view_item.appendRow(std::move(children_row0));
  view_item.appendRow(std::move(children_row1));
  view_item.insertRow(1, std::move(children_row2));  // inserting in-between

  EXPECT_EQ(view_item.rowCount(), 3);
  EXPECT_EQ(view_item.columnCount(), 2);
  EXPECT_EQ(view_item.child(0, 0), expected_row0[0]);
  EXPECT_EQ(view_item.child(0, 1), expected_row0[1]);
  EXPECT_EQ(view_item.child(1, 0), expected_row2[0]);
  EXPECT_EQ(view_item.child(1, 1), expected_row2[1]);
  EXPECT_EQ(view_item.child(2, 0), expected_row1[0]);
  EXPECT_EQ(view_item.child(2, 1), expected_row1[1]);

  // checking parents
  EXPECT_EQ(expected_row0[0]->parent(), &view_item);
  EXPECT_EQ(expected_row0[1]->parent(), &view_item);
  EXPECT_EQ(expected_row1[0]->parent(), &view_item);
  EXPECT_EQ(expected_row1[1]->parent(), &view_item);
  EXPECT_EQ(expected_row2[0]->parent(), &view_item);
  EXPECT_EQ(expected_row2[1]->parent(), &view_item);

  // checking row and column of children
  EXPECT_EQ(expected_row0[0]->row(), 0);
  EXPECT_EQ(expected_row0[1]->row(), 0);
  EXPECT_EQ(expected_row1[0]->row(), 2);
  EXPECT_EQ(expected_row1[1]->row(), 2);
  EXPECT_EQ(expected_row2[0]->row(), 1);
  EXPECT_EQ(expected_row2[1]->row(), 1);
  EXPECT_EQ(expected_row0[0]->column(), 0);
  EXPECT_EQ(expected_row0[1]->column(), 1);
  EXPECT_EQ(expected_row1[0]->column(), 0);
  EXPECT_EQ(expected_row1[1]->column(), 1);
  EXPECT_EQ(expected_row2[0]->column(), 0);
  EXPECT_EQ(expected_row2[1]->column(), 1);

  // removing middle row
  view_item.removeRow(1);
  EXPECT_EQ(view_item.child(0, 0), expected_row0[0]);
  EXPECT_EQ(view_item.child(0, 1), expected_row0[1]);
  EXPECT_EQ(view_item.child(1, 0), expected_row1[0]);
  EXPECT_EQ(view_item.child(1, 1), expected_row1[1]);
}

//! Clean item's children.

TEST_F(ViewItemTest, Clear)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  ViewItem view_item;
  view_item.appendRow(std::move(children));
  view_item.clear();

  EXPECT_EQ(view_item.rowCount(), 0);
  EXPECT_EQ(view_item.columnCount(), 0);
}

TEST_F(ViewItemTest, Children)
{
  auto [children_row0, expected_row0] = GetTestData(/*ncolumns*/ 2);
  auto [children_row1, expected_row1] = GetTestData(/*ncolumns*/ 2);

  ViewItem view_item;
  view_item.appendRow(std::move(children_row0));
  view_item.appendRow(std::move(children_row1));

  std::vector<ViewItem*> expected;
  std::copy(expected_row0.begin(), expected_row0.end(), std::back_inserter(expected));
  std::copy(expected_row1.begin(), expected_row1.end(), std::back_inserter(expected));

  EXPECT_EQ(view_item.children(), expected);
}

TEST_F(ViewItemTest, GetFlags)
{
  ViewItem view_item;

  EXPECT_TRUE(view_item.flags() & Qt::ItemIsSelectable);
  EXPECT_TRUE(view_item.flags() & Qt::ItemIsEnabled);

  // Current implementation is that ViewItem should have a data role set, to be editable
  EXPECT_FALSE(view_item.flags() & Qt::ItemIsEditable);
  view_item.setData(42, Qt::EditRole);
  EXPECT_TRUE(view_item.flags() & Qt::ItemIsEditable);
}
