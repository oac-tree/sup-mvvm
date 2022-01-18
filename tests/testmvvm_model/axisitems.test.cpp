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

//! Initial state

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

////! ViewportAxisItem::setRange

// TEST_F(AxisItemsTest, viewportAxisSetRange)
//{
//   ViewportAxisItem axis;

//  // default range
//  auto [lower, upper] = axis.GetRange();
//  EXPECT_EQ(lower, 0.0);
//  EXPECT_EQ(upper, 1.0);

//  axis.SetRange(1.0, 2.0);
//  EXPECT_EQ(axis.Property<double>(ViewportAxisItem::P_MIN), 1.0);
//  EXPECT_EQ(axis.Property<double>(ViewportAxisItem::P_MAX), 2.0);
//}

////! Factory method for FixedBinAxisItem.

// TEST_F(AxisItemsTest, fixedBinAxisInitialState)
//{
//   FixedBinAxisItem axis;
//   EXPECT_EQ(axis.Property<double>(FixedBinAxisItem::P_MIN), 0.0);
//   EXPECT_EQ(axis.Property<double>(FixedBinAxisItem::P_MAX), 1.0);
//   EXPECT_EQ(axis.Property<int>(FixedBinAxisItem::P_NBINS), 1);
//   EXPECT_EQ(axis.binCenters(), std::vector<double>{0.5});
//   EXPECT_EQ(axis.size(), 1);
//   auto [lower, upper] = axis.range();
//   EXPECT_EQ(lower, 0.0);
//   EXPECT_EQ(upper, 1.0);
// }

////! Factory method for FixedBinAxisItem.

// TEST_F(AxisItemsTest, fixedBinAxisSetParameters)
//{
//   FixedBinAxisItem axis;
//   axis.setParameters(3, 1.0, 4.0);

//  EXPECT_EQ(axis.Property<int>(FixedBinAxisItem::P_NBINS), 3);
//  EXPECT_EQ(axis.Property<double>(FixedBinAxisItem::P_MIN), 1.0);
//  EXPECT_EQ(axis.Property<double>(FixedBinAxisItem::P_MAX), 4.0);

//  std::vector<double> expected{1.5, 2.5, 3.5};
//  EXPECT_EQ(axis.binCenters(), expected);
//  EXPECT_EQ(axis.size(), 3);
//}

////! Factory method for FixedBinAxisItem.

// TEST_F(AxisItemsTest, fixedBinAxisFactory)
//{
//   auto axis = FixedBinAxisItem::create(3, 1.0, 4.0);

//  EXPECT_EQ(axis->Property<int>(FixedBinAxisItem::P_NBINS), 3);
//  EXPECT_EQ(axis->Property<double>(FixedBinAxisItem::P_MIN), 1.0);
//  EXPECT_EQ(axis->Property<double>(FixedBinAxisItem::P_MAX), 4.0);

//  std::vector<double> expected{1.5, 2.5, 3.5};
//  EXPECT_EQ(axis->binCenters(), expected);
//  EXPECT_EQ(axis->size(), 3);
//}

////! Range method.

// TEST_F(AxisItemsTest, fixedBinAxisRange)
//{
//   auto axis = FixedBinAxisItem::create(3, 1.0, 4.0);

//  auto [lower, upper] = axis->range();
//  EXPECT_EQ(lower, 1.0);
//  EXPECT_EQ(upper, 4.0);
//}

// TEST_F(AxisItemsTest, PointwiseAxisInitialState)
//{
//   PointwiseAxisItem axis;
//   std::vector<double> expected_centers = {0.0, 1.0};
//   EXPECT_EQ(axis.binCenters(), expected_centers);
//   EXPECT_EQ(axis.size(), 2);
// }

// TEST_F(AxisItemsTest, PointwiseAxisSetParameters)
//{
//   std::vector<double> expected_centers{1.0, 2.0, 3.0};
//   PointwiseAxisItem axis;
//   axis.setParameters(expected_centers);
//   EXPECT_EQ(axis.binCenters(), expected_centers);
//   EXPECT_EQ(axis.size(), 3);
// }

// TEST_F(AxisItemsTest, PointwiseAxisCreate)
//{
//   std::vector<double> expected_centers{1.0, 2.0, 3.0};
//   auto axis = PointwiseAxisItem::create(expected_centers);
//   EXPECT_EQ(axis->binCenters(), expected_centers);
//   EXPECT_EQ(axis->size(), 3);
// }
