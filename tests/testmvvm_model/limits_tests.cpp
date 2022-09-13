/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/model/limits.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for Limits class.

class LimitsTests : public ::testing::Test
{
};

TEST_F(LimitsTests, InitialState)
{
  {
    RealLimits limits;

    EXPECT_FALSE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());
    EXPECT_TRUE(limits.IsInRange(-std::numeric_limits<double>::infinity()));
    EXPECT_TRUE(limits.IsInRange(0.0));
    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<double>::infinity()));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }

  {
    IntLimits limits;

    EXPECT_FALSE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());
    EXPECT_TRUE(limits.IsInRange(-std::numeric_limits<int>::infinity()));
    EXPECT_TRUE(limits.IsInRange(0));
    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<int>::infinity()));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, LowerLimited)
{
  {
    // [5.0, inf[
    auto limits = RealLimits::CreateLowerLimited(5.0);
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), 5.0);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_TRUE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }

  {
    // [5.0, inf[
    IntLimits limits = IntLimits::CreateLowerLimited(5);
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), 5);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_TRUE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, UpperLimited)
{
  {
    // [-inf, 5.0[
    auto limits = RealLimits::CreateUpperLimited(5.0);
    EXPECT_FALSE(limits.HasLowerLimit());
    EXPECT_TRUE(limits.HasUpperLimit());

    EXPECT_THROW(limits.GetLowerLimit(), std::bad_optional_access);
    EXPECT_EQ(5.0, limits.GetUpperLimit());

    EXPECT_TRUE(limits.IsInRange(-std::numeric_limits<double>::infinity()));
    EXPECT_TRUE(limits.IsInRange(-2.0));
    EXPECT_TRUE(limits.IsInRange(4.9));
    EXPECT_FALSE(limits.IsInRange(5.0));
    EXPECT_FALSE(limits.IsInRange(std::numeric_limits<double>::infinity()));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_TRUE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }

  {
    // [-inf, 5.0[
    auto limits = IntLimits::CreateUpperLimited(5);
    EXPECT_FALSE(limits.HasLowerLimit());
    EXPECT_TRUE(limits.HasUpperLimit());

    EXPECT_THROW(limits.GetLowerLimit(), std::bad_optional_access);
    EXPECT_EQ(5, limits.GetUpperLimit());

    EXPECT_TRUE(limits.IsInRange(-2));
    EXPECT_FALSE(limits.IsInRange(5));
    EXPECT_FALSE(limits.IsInRange(10));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_TRUE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, Limited)
{
  {
    // [-10.0, 2.0[
    auto limits = RealLimits::CreateLimited(-10.0, 2.0);
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_TRUE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), -10.0);
    EXPECT_EQ(limits.GetUpperLimit(), 2.0);

    EXPECT_FALSE(limits.IsInRange(-11.0));
    EXPECT_TRUE(limits.IsInRange(-9.0));
    EXPECT_TRUE(limits.IsInRange(0.0));
    EXPECT_TRUE(limits.IsInRange(1.0));
    EXPECT_FALSE(limits.IsInRange(2.0));
    EXPECT_FALSE(limits.IsInRange(3.0));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_TRUE(limits.IsLimited());
  }

  {
    // [-10.0, 2.0[
    auto limits = IntLimits::CreateLimited(-10, 2);
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_TRUE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), -10);
    EXPECT_EQ(limits.GetUpperLimit(), 2);

    EXPECT_FALSE(limits.IsInRange(-11));
    EXPECT_TRUE(limits.IsInRange(-9));
    EXPECT_TRUE(limits.IsInRange(0));
    EXPECT_TRUE(limits.IsInRange(1));
    EXPECT_FALSE(limits.IsInRange(2));
    EXPECT_FALSE(limits.IsInRange(3));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_TRUE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, Positive)
{
  {
    // ]0.0, 2.0[
    auto limits = RealLimits::CreatePositive();
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), std::numeric_limits<double>::min());
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_FALSE(limits.IsInRange(-11.0));
    EXPECT_FALSE(limits.IsInRange(0.0));
    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<double>::min()));
    EXPECT_TRUE(limits.IsInRange(1.0));

    EXPECT_TRUE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }

  {
    // ]0.0, 2.0[
    auto limits = IntLimits::CreatePositive();
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), 1);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_FALSE(limits.IsInRange(-11));
    EXPECT_FALSE(limits.IsInRange(0));
    EXPECT_FALSE(limits.IsInRange(std::numeric_limits<int>::min()));
    EXPECT_TRUE(limits.IsInRange(1));

    EXPECT_TRUE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, Nonnegative)
{
  {
    // [0.0, 2.0[
    auto limits = RealLimits::CreateNonnegative();
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), 0.0);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_FALSE(limits.IsInRange(-11.0));
    EXPECT_TRUE(limits.IsInRange(0.0));
    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<double>::min()));
    EXPECT_TRUE(limits.IsInRange(1.0));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_TRUE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }

  {
    // [0.0, 2.0[
    auto limits = IntLimits::CreateNonnegative();
    EXPECT_TRUE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_EQ(limits.GetLowerLimit(), 0);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_FALSE(limits.IsInRange(-11));
    EXPECT_TRUE(limits.IsInRange(0));
    EXPECT_TRUE(limits.IsInRange(1));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_TRUE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, Limitless)
{
  {
    auto limits = RealLimits::CreateLimitless();

    EXPECT_FALSE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_THROW(limits.GetLowerLimit(), std::bad_optional_access);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_TRUE(limits.IsInRange(-std::numeric_limits<double>::infinity()));
    EXPECT_TRUE(limits.IsInRange(0.0));
    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<double>::infinity()));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }

  {
    auto limits = IntLimits::CreateLimitless();

    EXPECT_FALSE(limits.HasLowerLimit());
    EXPECT_FALSE(limits.HasUpperLimit());

    EXPECT_THROW(limits.GetLowerLimit(), std::bad_optional_access);
    EXPECT_THROW(limits.GetUpperLimit(), std::bad_optional_access);

    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<int>::min()));
    EXPECT_TRUE(limits.IsInRange(0.0));
    EXPECT_TRUE(limits.IsInRange(std::numeric_limits<int>::max()));

    EXPECT_FALSE(limits.IsPositive());
    EXPECT_FALSE(limits.IsNonnegative());
    EXPECT_FALSE(limits.IsLowerLimited());
    EXPECT_FALSE(limits.IsUpperLimited());
    EXPECT_FALSE(limits.IsLimited());
  }
}

TEST_F(LimitsTests, ComparisonOperators)
{
  {
    auto lim1 = RealLimits::CreateLimited(1.0, 2.0);
    auto lim2 = RealLimits::CreateLimited(1.0, 2.0);
    EXPECT_TRUE(lim1 == lim2);
    EXPECT_FALSE(lim1 != lim2);

    auto lim3 = RealLimits::CreateLimitless();
    auto lim4 = RealLimits::CreateLimitless();
    EXPECT_TRUE(lim3 == lim4);
    EXPECT_FALSE(lim3 != lim4);

    auto lim5 = RealLimits::CreateLowerLimited(1.0);
    auto lim6 = RealLimits::CreateLowerLimited(1.0);
    EXPECT_TRUE(lim5 == lim6);
    EXPECT_FALSE(lim5 != lim6);

    auto lim7 = RealLimits::CreateUpperLimited(1.0);
    auto lim8 = RealLimits::CreateUpperLimited(1.0);
    EXPECT_TRUE(lim7 == lim8);
    EXPECT_FALSE(lim7 != lim8);

    EXPECT_TRUE(RealLimits::CreatePositive() == RealLimits::CreatePositive());
    EXPECT_TRUE(RealLimits::CreateNonnegative() == RealLimits::CreateNonnegative());

    EXPECT_FALSE(RealLimits::CreatePositive() == RealLimits::CreateNonnegative());
  }

  {
    auto lim1 = IntLimits::CreateLimited(1.0, 2.0);
    auto lim2 = IntLimits::CreateLimited(1.0, 2.0);
    EXPECT_TRUE(lim1 == lim2);
    EXPECT_FALSE(lim1 != lim2);

    auto lim3 = IntLimits::CreateLimitless();
    auto lim4 = IntLimits::CreateLimitless();
    EXPECT_TRUE(lim3 == lim4);
    EXPECT_FALSE(lim3 != lim4);

    auto lim5 = IntLimits::CreateLowerLimited(1.0);
    auto lim6 = IntLimits::CreateLowerLimited(1.0);
    EXPECT_TRUE(lim5 == lim6);
    EXPECT_FALSE(lim5 != lim6);

    auto lim7 = IntLimits::CreateUpperLimited(1.0);
    auto lim8 = IntLimits::CreateUpperLimited(1.0);
    EXPECT_TRUE(lim7 == lim8);
    EXPECT_FALSE(lim7 != lim8);

    EXPECT_TRUE(IntLimits::CreatePositive() == IntLimits::CreatePositive());
    EXPECT_TRUE(IntLimits::CreateNonnegative() == IntLimits::CreateNonnegative());

    EXPECT_FALSE(IntLimits::CreatePositive() == IntLimits::CreateNonnegative());
  }
}

TEST_F(LimitsTests, CopyConstructor)
{
  RealLimits lim1 = RealLimits::CreateLimited(1.0, 2.0);
  RealLimits lim2 = lim1;
  EXPECT_TRUE(lim1 == lim2);
  EXPECT_FALSE(lim1 != lim2);

  RealLimits lim3(lim1);
  EXPECT_TRUE(lim1 == lim3);
  EXPECT_FALSE(lim1 != lim3);
}
