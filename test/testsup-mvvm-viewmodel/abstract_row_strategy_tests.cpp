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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/abstract_row_strategy.h"

#include <mvvm/model/session_item.h>
#include <mvvm/viewmodelbase/viewitem.h>

using namespace mvvm;
#include <gtest/gtest.h>

//! Testing AbstractRowStrategy class.

class AbstractRowStrategyTest : public ::testing::Test
{
public:
  /**
   * @brief The TestStrategy class to validate AbstractRowStrategy::CreatePlaceholderRow method.
   */
  class TestStrategy : public AbstractRowStrategy
  {
  public:
    explicit TestStrategy(int max_columns) : m_max_columns(max_columns) {}

    int GetSize() const override { return m_max_columns; }

  private:
    std::vector<std::unique_ptr<ViewItem>> ConstructRowImpl(SessionItem* item) override
    {
      (void)item;
      return {};
    }
    int m_max_columns{0};
  };
};

TEST_F(AbstractRowStrategyTest, CreatePlaceholderRow)
{
  const int expected_size(2);
  SessionItem item;
  item.SetDisplayName("abc");

  TestStrategy strategy(expected_size);

  EXPECT_EQ(strategy.GetSize(), expected_size);

  auto row = strategy.CreatePlaceholderRow(&item);

  // the length  should match expected size
  ASSERT_EQ(row.size(), expected_size);
  EXPECT_EQ(row.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
  EXPECT_EQ(row.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
}

TEST_F(AbstractRowStrategyTest, ConstructRow)
{
  const int expected_size(2);
  SessionItem item;
  item.SetDisplayName("abc");

  TestStrategy strategy(expected_size);

  EXPECT_EQ(strategy.GetSize(), expected_size);

  // Our implementation of ConstructRowImpl returns an empty row. Check that it is silently replaced
  // with the placeholder row.
  auto row = strategy.ConstructRow(&item);

  // the length  should match expected size
  ASSERT_EQ(row.size(), expected_size);
  EXPECT_EQ(row.at(0)->Data(Qt::DisplayRole).toString(), QString("abc"));
  EXPECT_EQ(row.at(1)->Data(Qt::DisplayRole).toString(), QString(""));
}
