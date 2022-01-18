/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/axisitems.h"

#include "mvvm/standarditems/plottableitems.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Testing AxisItems.

class AxisItemsTest : public ::testing::Test
{
};

TEST_F(AxisItemsTest, ViewportAxisInitialState)
{
  ViewportAxisItem axis;
  EXPECT_EQ(axis.GetMin(), 0.0);
  EXPECT_EQ(axis.GetMax(), 1.0);
  EXPECT_TRUE(axis.GetTitle()->GetText().empty());
  EXPECT_FALSE(axis.IsInLog());

  auto [lower, upper] = axis.GetRange();
  EXPECT_EQ(lower, 0.0);
  EXPECT_EQ(upper, 1.0);
}

TEST_F(AxisItemsTest, ViewportAxisGetSet)
{
  ViewportAxisItem axis;

  axis.SetRange(1.0, 2.0);
  auto [lower, upper] = axis.GetRange();
  EXPECT_EQ(lower, 1.0);
  EXPECT_EQ(upper, 2.0);

  axis.SetInLog(true);
  EXPECT_TRUE(axis.IsInLog());
}

TEST_F(AxisItemsTest, FixedBinAxisInitialState)
{
  FixedBinAxisItem axis;
  EXPECT_EQ(axis.GetMin(), 0.0);
  EXPECT_EQ(axis.GetMax(), 1.0);
  EXPECT_EQ(axis.GetSize(), 1);
  EXPECT_EQ(axis.GetBinCenters(), std::vector<double>{0.5});
  auto [lower, upper] = axis.GetRange();
  EXPECT_EQ(lower, 0.0);
  EXPECT_EQ(upper, 1.0);
}

//! Factory method for FixedBinAxisItem.

TEST_F(AxisItemsTest, FixedBinAxisSetParameters)
{
  FixedBinAxisItem axis;
  axis.SetParameters(3, 1.0, 4.0);

  EXPECT_EQ(axis.GetSize(), 3);
  EXPECT_EQ(axis.GetMin(), 1.0);
  EXPECT_EQ(axis.GetMax(), 4.0);

  std::vector<double> expected{1.5, 2.5, 3.5};
  EXPECT_EQ(axis.GetBinCenters(), expected);
}

TEST_F(AxisItemsTest, FixedBinAxisFactoryMethod)
{
  auto axis = FixedBinAxisItem::Create(3, 1.0, 4.0);

  EXPECT_EQ(axis->GetSize(), 3);
  EXPECT_EQ(axis->GetMin(), 1.0);
  EXPECT_EQ(axis->GetMax(), 4.0);

  std::vector<double> expected{1.5, 2.5, 3.5};
  EXPECT_EQ(axis->GetBinCenters(), expected);
}

//! Range method.

TEST_F(AxisItemsTest, FixedBinAxisRange)
{
  auto axis = FixedBinAxisItem::Create(3, 1.0, 4.0);

  auto [lower, upper] = axis->GetRange();
  EXPECT_EQ(lower, 1.0);
  EXPECT_EQ(upper, 4.0);
}

TEST_F(AxisItemsTest, PointwiseAxisInitialState)
{
  PointwiseAxisItem axis;
  std::vector<double> expected_centers = {0.0, 1.0};
  EXPECT_EQ(axis.GetBinCenters(), expected_centers);
  EXPECT_EQ(axis.GetSize(), 2);
}

TEST_F(AxisItemsTest, PointwiseAxisSetParameters)
{
  std::vector<double> expected_centers{1.0, 2.0, 3.0};
  PointwiseAxisItem axis;
  axis.SetParameters(expected_centers);
  EXPECT_EQ(axis.GetBinCenters(), expected_centers);
  EXPECT_EQ(axis.GetSize(), 3);
}

TEST_F(AxisItemsTest, PointwiseAxisFactoryMethod)
{
  std::vector<double> expected_centers{1.0, 2.0, 3.0};
  auto axis = PointwiseAxisItem::Create(expected_centers);
  EXPECT_EQ(axis->GetBinCenters(), expected_centers);
  EXPECT_EQ(axis->GetSize(), 3);
}
