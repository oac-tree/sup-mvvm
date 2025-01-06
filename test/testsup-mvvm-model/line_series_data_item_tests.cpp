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

#include "mvvm/standarditems/line_series_data_item.h"

#include <mvvm/standarditems/point_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for LineSeriesDataItem class.

class LineSeriesDataItemTest : public ::testing::Test
{
};

TEST_F(LineSeriesDataItemTest, InitialState)
{
  const LineSeriesDataItem item;

  EXPECT_TRUE(item.GetWaveform().empty());
  EXPECT_EQ(item.GetPointCount(), 0);
}

TEST_F(LineSeriesDataItemTest, GetSetWaveform)
{
  LineSeriesDataItem item;

  const std::vector<std::pair<double, double>> expected({{1.0, 10.0}, {2.0, 20.0}});
  item.SetWaveform(expected);

  EXPECT_EQ(item.GetWaveform(), expected);
  EXPECT_EQ(item.GetPointCount(), 2);
  auto points = item.GetPoints();
  ASSERT_EQ(points.size(), 2);
  EXPECT_EQ(item.GetPoint(0)->GetX(), 1.0);
  EXPECT_EQ(item.GetPoint(0)->GetY(), 10.0);

  const std::vector<std::pair<double, double>> expected2({{3.0, 30.0}, {4.0, 40.0}, {5.0, 50.0}});
  item.SetWaveform(expected2);
  EXPECT_EQ(item.GetWaveform(), expected2);
}

TEST_F(LineSeriesDataItemTest, GetAndSetPointCoordinates)
{
  LineSeriesDataItem item;

  const std::vector<std::pair<double, double>> expected({{1.0, 10.0}, {2.0, 20.0}});
  item.SetWaveform(expected);

  EXPECT_EQ(item.GetPointCoordinates(0), std::make_pair(1.0, 10.0));
  EXPECT_EQ(item.GetPointCoordinates(1), std::make_pair(2.0, 20.0));
  EXPECT_THROW(item.GetPointCoordinates(2), RuntimeException);

  item.SetPointCoordinates(0, {3.0, 30.0});
  EXPECT_EQ(item.GetPointCoordinates(0), std::make_pair(3.0, 30.0));
}

TEST_F(LineSeriesDataItemTest, RemovePoint)
{
  LineSeriesDataItem item;

  const std::vector<std::pair<double, double>> waveform({{1.0, 10.0}, {2.0, 20.0}});
  item.SetWaveform(waveform);

  item.RemovePoint(0);

  const std::vector<std::pair<double, double>> expected({{2.0, 20.0}});
  EXPECT_EQ(item.GetWaveform(), expected);
}

TEST_F(LineSeriesDataItemTest, InsertPoint)
{
  LineSeriesDataItem item;

  const std::vector<std::pair<double, double>> waveform({{1.0, 10.0}, {3.0, 30.0}});
  item.SetWaveform(waveform);

  item.InsertPoint(1, {2.0, 20.0});

  const std::vector<std::pair<double, double>> expected({{1.0, 10.0}, {2.0, 20.0}, {3.0, 30.0}});
  EXPECT_EQ(item.GetWaveform(), expected);
}
