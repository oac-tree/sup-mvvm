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

#include "mvvm/providers/viewitem.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/providers/i_viewitem_data.h>
#include <mvvm/test/test_container_helper.h>
#include <mvvm/utils/container_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_viewitem_data.h>

namespace mvvm::test
{

/**
 * @brief Tests for ViewItem class.
 */
class ViewItemTest : public ::testing::Test
{
public:
  using children_t = std::vector<std::unique_ptr<ViewItem>>;
  using expected_t = std::vector<ViewItem*>;

  /**
   * @brief Helper function to get two vectors, each ncolumns length, in the form of a pair.
   *
   * First vector contains unique_ptr objects, second vector bare pointers to same objects.
   * First vector is intended to be moved inside a model, second vector is to validate
   * the content of a model after the move.
   */
  static std::pair<children_t, expected_t> GetTestData(int ncolumns)
  {
    auto vector_of_unique = mvvm::test::CreateRow<ViewItem, ViewItem>(ncolumns);
    auto vector_of_pointers = mvvm::utils::GetVectorOfPtrs(vector_of_unique);
    return std::make_pair(std::move(vector_of_unique), std::move(vector_of_pointers));
  }
};

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

TEST_F(ViewItemTest, AppendSingleItemAsRow)
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

TEST_F(ViewItemTest, AppendTwoRowsWithTwoItemsEach)
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

TEST_F(ViewItemTest, ClearItemChildren)
{
  auto [children, expected] = GetTestData(/*ncolumns*/ 1);

  ViewItem view_item;
  view_item.AppendRow(std::move(children));
  view_item.Clear();

  EXPECT_EQ(view_item.GetRowCount(), 0);
  EXPECT_EQ(view_item.GetColumnCount(), 0);
}

TEST_F(ViewItemTest, GetChildren)
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

TEST_F(ViewItemTest, GetFlagsForViewItemWithDefaultViewItemData)
{
  const ViewItem view_item;

  EXPECT_TRUE(view_item.Flags().testFlags(Qt::ItemIsSelectable));
  EXPECT_TRUE(view_item.Flags().testFlags(Qt::ItemIsEnabled));
  EXPECT_TRUE(view_item.Flags().testFlags(Qt::ItemIsEditable));
  EXPECT_FALSE(view_item.Flags().testFlags(Qt::ItemIsUserCheckable));
}

TEST_F(ViewItemTest, TestWithMockData)
{
  MockViewItemData mock_data;

  auto item_data = std::make_unique<ViewItemDataDecorator>(&mock_data);

  // creating view item with mock data
  ViewItem view_item(std::move(item_data));

  // In Qt::ItemIsEnabled flag means that the user can interact with the item (select, edit,
  // check/uncheck). It is always true for our viewmodel.

  // It is different from our item being enabled/disabled, which means only some different
  // appearance (i.e. gray/normal). Thus, we expect no calls to IsEnabled while querring flags.
  EXPECT_CALL(mock_data, IsEnabled()).Times(0);

  EXPECT_CALL(mock_data, IsEditable()).WillOnce(testing::Return(false));
  EXPECT_CALL(mock_data, IsCheckable()).WillOnce(testing::Return(true));

  const auto flags = view_item.Flags();
  EXPECT_TRUE(flags.testFlags(Qt::ItemIsEnabled));
  EXPECT_FALSE(flags.testFlags(Qt::ItemIsEditable));
  EXPECT_TRUE(flags.testFlags(Qt::ItemIsUserCheckable));

  // checking that view item forwards calls to data
  EXPECT_CALL(mock_data, Data(42)).WillOnce(testing::Return(QVariant(123)));
  EXPECT_CALL(mock_data, SetData(QVariant(456), 42)).WillOnce(testing::Return(true));

  EXPECT_EQ(view_item.Data(42), QVariant(123));
  EXPECT_TRUE(view_item.SetData(QVariant(456), 42));
}

}  // namespace mvvm::test
