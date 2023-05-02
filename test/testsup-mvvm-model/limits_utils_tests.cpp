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

#include "mvvm/serialization/limits_utils.h"

#include <mvvm/model/limits.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for Limits class.

class LimitsUtilsTests : public ::testing::Test
{
};

TEST_F(LimitsUtilsTests, IntLimitsToString)
{
  EXPECT_EQ(utils::ToString(IntLimits::CreateLimitless()), "limitless");
  EXPECT_EQ(utils::ToString(IntLimits::CreatePositive()), "positive");
  EXPECT_EQ(utils::ToString(IntLimits::CreateNonnegative()), "nonnegative");
  EXPECT_EQ(utils::ToString(IntLimits::CreateLowerLimited(42)), "lowerlimited;42");
  EXPECT_EQ(utils::ToString(IntLimits::CreateUpperLimited(1)), "upperlimited;1");
  EXPECT_EQ(utils::ToString(IntLimits::CreateLimited(-1, 2)), "limited;-1;2");
}

TEST_F(LimitsUtilsTests, RealLimitsToString)
{
  EXPECT_EQ(utils::ToString(RealLimits::CreateLimitless()), "limitless");
  EXPECT_EQ(utils::ToString(RealLimits::CreatePositive()), "positive");
  EXPECT_EQ(utils::ToString(RealLimits::CreateNonnegative()), "nonnegative");
  EXPECT_EQ(utils::ToString(RealLimits::CreateLowerLimited(42.1)), "lowerlimited;42.1");
  EXPECT_EQ(utils::ToString(RealLimits::CreateUpperLimited(1.1)), "upperlimited;1.1");
  EXPECT_EQ(utils::ToString(RealLimits::CreateUpperLimited(1.1e-05)), "upperlimited;1.1e-05");
  EXPECT_EQ(utils::ToString(RealLimits::CreateLimited(-1.1, 2.2)), "limited;-1.1;2.2");
}

TEST_F(LimitsUtilsTests, IntLimitsFromString)
{
  EXPECT_EQ(IntLimits::CreateLimitless(), utils::IntLimitsFromString("limitless"));
  EXPECT_EQ(IntLimits::CreatePositive(), utils::IntLimitsFromString("positive"));
  EXPECT_EQ(IntLimits::CreateNonnegative(), utils::IntLimitsFromString("nonnegative"));
  EXPECT_EQ(IntLimits::CreateLowerLimited(42), utils::IntLimitsFromString("lowerlimited;42"));
  EXPECT_EQ(IntLimits::CreateUpperLimited(1), utils::IntLimitsFromString("upperlimited;1"));
  EXPECT_EQ(IntLimits::CreateLimited(-1, 2), utils::IntLimitsFromString("limited;-1;2"));
}

TEST_F(LimitsUtilsTests, RealLimitsFromString)
{
  EXPECT_EQ(RealLimits::CreateLimitless(), utils::RealLimitsFromString("limitless"));
  EXPECT_EQ(RealLimits::CreatePositive(), utils::RealLimitsFromString("positive"));
  EXPECT_EQ(RealLimits::CreateNonnegative(), utils::RealLimitsFromString("nonnegative"));
  EXPECT_EQ(RealLimits::CreateLowerLimited(42.1), utils::RealLimitsFromString("lowerlimited;42.1"));
  EXPECT_EQ(RealLimits::CreateUpperLimited(1.1), utils::RealLimitsFromString("upperlimited;1.1"));
  EXPECT_EQ(RealLimits::CreateUpperLimited(1.1e-05),
            utils::RealLimitsFromString("upperlimited;1.1e-05"));
  EXPECT_EQ(RealLimits::CreateLimited(-1.1, 2.2), utils::RealLimitsFromString("limited;-1.1;2.2"));
}
