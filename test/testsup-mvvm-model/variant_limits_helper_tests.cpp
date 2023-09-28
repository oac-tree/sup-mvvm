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

#include "mvvm/utils/variant_limits_helper.h"

#include <gtest/gtest.h>

#include <limits>

using namespace mvvm;

//! Unit tests for helper methods in variant_limits_helper.h

class VariantLimitsHelper : public ::testing::Test
{
};

//! Testing GetInt32MinMaxNumeric function. We test numeric limits for various types casted to
//! integer.

TEST_F(VariantLimitsHelper, GetInt32MinMaxNumeric)
{
  {  // int32
    int32 num(42);
    auto limits = GetInt32MinMaxNumeric(num);
    EXPECT_EQ(limits.first, std::numeric_limits<int32>::min());
    EXPECT_EQ(limits.second, std::numeric_limits<int32>::max());
  }

  {  // uint8
    uint8 num(1);
    auto limits = GetInt32MinMaxNumeric(num);
    EXPECT_EQ(limits.first, std::numeric_limits<uint8>::min());
    EXPECT_EQ(limits.second, std::numeric_limits<uint8>::max());
    EXPECT_EQ(limits.first, 0);
    EXPECT_EQ(limits.second, 255);
  }

  // uint32, double limits can't be handled by given helper function
  EXPECT_THROW(GetInt32MinMaxNumeric(42U), RuntimeException);
  EXPECT_THROW(GetInt32MinMaxNumeric(42.2), RuntimeException);
}

//! Testing GetInt32MinMaxNumeric function with additional user bounds.

TEST_F(VariantLimitsHelper, GetInt32MinMaxNumericWithUserDefinedLimits)
{
  {  // int32, additionally bounded from left
    int32 num(42);
    auto limits = GetInt32MinMaxNumeric(num, 10);
    EXPECT_EQ(limits.first, 10);
    EXPECT_EQ(limits.second, std::numeric_limits<int32>::max());
  }

  {  // int32, additionally bounded from left and right
    int32 num(42);
    auto limits = GetInt32MinMaxNumeric(num, 10, 20);
    EXPECT_EQ(limits.first, 10);
    EXPECT_EQ(limits.second, 20);
  }

  {  // uint8 additionally bounded from right
    uint8 num(1);
    auto limits = GetInt32MinMaxNumeric(num, {}, uint8{20});
    EXPECT_EQ(limits.first, std::numeric_limits<uint8>::min());
    EXPECT_EQ(limits.first, 0);
    EXPECT_EQ(limits.second, 20);
  }
}
