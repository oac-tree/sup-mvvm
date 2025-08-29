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

#include "mvvm/providers/abstract_row_strategy.h"

#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewitem.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

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
    explicit TestStrategy(std::size_t max_columns) : m_max_columns(max_columns) {}

    std::size_t GetSize() const override { return m_max_columns; }

  private:
    std::vector<std::unique_ptr<ViewItem>> ConstructRowImpl(SessionItem* item) override
    {
      (void)item;
      return {};
    }
    std::size_t m_max_columns{0};
  };

  /**
   * @brief The BrokenTestStrategy class creates the row of different lenth, than reports through
   * GetSize method.
   */
  class BrokenTestStrategy : public AbstractRowStrategy
  {
  public:
    BrokenTestStrategy() = default;

    std::size_t GetSize() const override { return 42U; }

  private:
    std::vector<std::unique_ptr<ViewItem>> ConstructRowImpl(SessionItem* item) override
    {
      std::vector<std::unique_ptr<ViewItem>> result;
      result.push_back(std::make_unique<ViewItem>());
      return result;
    }
  };
};

TEST_F(AbstractRowStrategyTest, CreatePlaceholderRow)
{
  const int expected_size(2);
  SessionItem item;
  item.SetDisplayName("abc");

  const TestStrategy strategy(expected_size);

  EXPECT_EQ(strategy.GetSize(), expected_size);
  EXPECT_TRUE(strategy.GetHorizontalHeaderLabels().isEmpty());

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

TEST_F(AbstractRowStrategyTest, ConstructRowUsingBrokenStrategy)
{
  SessionItem item;
  item.SetDisplayName("abc");

  BrokenTestStrategy strategy;

  EXPECT_THROW(strategy.ConstructRow(&item), RuntimeException);
}

}  // namespace mvvm::test
