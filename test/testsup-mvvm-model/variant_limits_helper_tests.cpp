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

#include "mvvm/utils/variant_limits_helper.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

#include <limits>

using namespace mvvm;

/**
 * @brief Tests for helper methods in variant_limits_helper.h
 */
class VariantLimitsHelperTest : public ::testing::Test
{
};

TEST_F(VariantLimitsHelperTest, GetInt32MinMaxNumeric)
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
  EXPECT_THROW(GetInt32MinMaxNumeric(std::string()), RuntimeException);
}

TEST_F(VariantLimitsHelperTest, GetInt32MinMaxNumericWithUserDefinedLimits)
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

TEST_F(VariantLimitsHelperTest, GetFloat64MinMaxNumeric)
{
  {  // float64
    float64 num(42.1);
    auto limits = GetFloat64MinMaxNumeric(num);
    EXPECT_EQ(limits.first, std::numeric_limits<float64>::lowest());
    EXPECT_EQ(limits.second, std::numeric_limits<float64>::max());
  }

  {  // float32
    float32 num(42.1);
    auto limits = GetFloat64MinMaxNumeric(num);
    EXPECT_EQ(limits.first, std::numeric_limits<float32>::lowest());
    EXPECT_EQ(limits.second, std::numeric_limits<float32>::max());
  }
}

TEST_F(VariantLimitsHelperTest, GetFloat64MinMaxNumericWithUserDefinedLimits)
{
  {  // float64 bounded from left
    float64 num(42.1);
    auto limits = GetFloat64MinMaxNumeric(num, float64{10.0});
    EXPECT_EQ(limits.first, 10.0);
    EXPECT_EQ(limits.second, std::numeric_limits<float64>::max());
  }

  {  // float32 bounded from right
    float32 num(42.1);
    auto limits = GetFloat64MinMaxNumeric(num, {}, float32{60});
    EXPECT_EQ(limits.first, std::numeric_limits<float32>::lowest());
    EXPECT_EQ(limits.second, 60);
  }

  {  // float32 bounded from left and right
    float32 num(42.1);
    auto limits = GetFloat64MinMaxNumeric(num, float32{50}, float32{60});
    EXPECT_EQ(limits.first, 50);
    EXPECT_EQ(limits.second, 60);
  }
}
