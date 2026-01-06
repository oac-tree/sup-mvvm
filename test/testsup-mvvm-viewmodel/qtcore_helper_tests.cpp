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

#include "mvvm/providers/qtcore_helper.h"

#include <gtest/gtest.h>

namespace mvvm::test
{

class QtCoreHelperTest : public ::testing::Test
{
};

TEST_F(QtCoreHelperTest, GetStringList)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(utils::GetStringList(vec_t()), QStringList());
  EXPECT_EQ(utils::GetStringList(vec_t({"abc", "cde"})), QStringList({"abc", "cde"}));
}

TEST_F(QtCoreHelperTest, GetStdStringVector)
{
  using vec_t = std::vector<std::string>;
  EXPECT_EQ(utils::GetStdStringVector(QStringList()), vec_t());
  EXPECT_EQ(utils::GetStdStringVector(QStringList({"abc", "cde"})), vec_t({"abc", "cde"}));
}

TEST_F(QtCoreHelperTest, GetByteArray)
{
  QStringList expected = QStringList() << "aaa"
                                       << "bbb"
                                       << "ccc";

  auto array = utils::GetByteArray(expected);
  EXPECT_EQ(utils::GetStringList(array), expected);
}

TEST_F(QtCoreHelperTest, GetDistanceSquared)
{
  const QPointF p1(1.0, 2.0);
  const QPointF p2(4.0, 6.0);
  const double dist_sq = utils::GetDistanceSquared(p1, p2);
  EXPECT_DOUBLE_EQ(dist_sq, 25.0);
}

TEST_F(QtCoreHelperTest, GetNearestPointIndex)
{
  // for empty array
  {
    const QList<QPointF> points;
    const std::int32_t index = utils::GetNearestPointIndex(points, QPointF(0.0, 0.0));
    EXPECT_EQ(index, -1);
  }

  // for one element array
  {
    const QList<QPointF> points = {QPointF(1.0, 2.0)};
    const std::int32_t index = utils::GetNearestPointIndex(points, QPointF(3.0, 5.0));
    EXPECT_EQ(index, 0);
  }

  {
    const QList<QPointF> points = {QPointF(1.0, 2.0), QPointF(4.0, 6.0), QPointF(-1.0, -1.0)};
    const std::int32_t index = utils::GetNearestPointIndex(points, QPointF(3.0, 5.0));
    EXPECT_EQ(index, 1);
  }

  {
    const QList<QPointF> points = {QPointF(1.0, 2.0), QPointF(4.0, 6.0), QPointF(-1.0, -1.0)};
    const std::int32_t index = utils::GetNearestPointIndex(points, QPointF(-1.0, -1.0));
    EXPECT_EQ(index, 2);
  }
}

}  // namespace mvvm::test
