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

#include "mvvm/utils/limited_float.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for LimitedFloat class.
 */
class LimitedFloatTests : public ::testing::Test
{
};

TEST_F(LimitedFloatTests, Constructor)
{
  {  // number without bounds defined
    const LimitedFloat<float64> num(42.1, {}, {});
    EXPECT_EQ(num.GetValue(), 42.1);
    EXPECT_EQ(num.GetLowerBound(), std::numeric_limits<float64>::lowest());
    EXPECT_EQ(num.GetUpperBound(), std::numeric_limits<float64>::max());
    EXPECT_FALSE(num.IsNullable());
    EXPECT_TRUE(num.HasValue());
    EXPECT_EQ(num.GetSingleStep(), 1.0);
  }

  {  // float32 without bounds defined
    const LimitedFloat<float32> num(42.1F, {}, {});
    EXPECT_EQ(num.GetValue(), 42.1F);
    EXPECT_EQ(num.GetLowerBound(), std::numeric_limits<float32>::lowest());
    EXPECT_EQ(num.GetUpperBound(), std::numeric_limits<float32>::max());
  }

  {  // number with bounds
    const LimitedFloat<float64> num(42.1, 1.0, 45.0);
    EXPECT_EQ(num.GetValue(), 42.1);
    EXPECT_EQ(num.GetLowerBound(), 1.0);
    EXPECT_EQ(num.GetUpperBound(), 45.0);
  }

  {  // value forced into bounds
    EXPECT_EQ(LimitedFloat<float64>(42.1, 10.0, 20.0).GetValue(), 20.0);
    EXPECT_EQ(LimitedFloat<float64>(1.0, 10.0, 20.0).GetValue(), 10.0);
  }

  // upper bound smaller than lower bound
  EXPECT_THROW(LimitedFloat<float64>(42.0, 10.0, 5.0), RuntimeException);
}

TEST_F(LimitedFloatTests, SetValue)
{
  LimitedFloat<float64> num(42.0, {40.0}, {50.0});
  EXPECT_FALSE(num.SetValue(42.0));  // same value
  EXPECT_TRUE(num.SetValue(40.0));
  EXPECT_TRUE(num.SetValue(50.0));
  EXPECT_FALSE(num.SetValue(39.9));
  EXPECT_FALSE(num.SetValue(50.1));
  EXPECT_EQ(num.GetValue(), 50.0);

  // the value is stored exactly
  EXPECT_TRUE(num.SetValue(42.123456789));
  EXPECT_EQ(num.GetValue(), 42.123456789);
}

TEST_F(LimitedFloatTests, SetValueFromVariant)
{
  LimitedFloat<float64> num(42.0, {}, {});
  EXPECT_FALSE(num.SetValueFromVariant(variant_t{42.0}));  // same value
  EXPECT_TRUE(num.SetValueFromVariant(variant_t{43.0}));
  EXPECT_EQ(num.GetValueAsVariant(), variant_t{43.0});

  // non-nullable number keeps the value
  EXPECT_FALSE(num.SetValueFromVariant(variant_t{}));
  EXPECT_EQ(num.GetValueAsVariant(), variant_t{43.0});

  EXPECT_EQ(num.GetLowerBoundAsVariant(), variant_t{std::numeric_limits<float64>::lowest()});
  EXPECT_EQ(num.GetUpperBoundAsVariant(), variant_t{std::numeric_limits<float64>::max()});

  LimitedFloat<float32> num32(42.0F, {}, {});
  EXPECT_TRUE(num32.SetValueFromVariant(variant_t{43.0F}));
  EXPECT_EQ(num32.GetValueAsVariant(), variant_t{43.0F});
}

TEST_F(LimitedFloatTests, GetValueAsText)
{
  const LimitedFloat<float64> num(42.123456, {}, {});
  EXPECT_EQ(num.GetValueAsText(4, false), std::string("42.1235"));
  EXPECT_EQ(num.GetValueAsText(2, false), std::string("42.12"));
  EXPECT_EQ(num.GetValueAsText(4, true), std::string("4.2123e+01"));

  const LimitedFloat<float64> tiny(4.4647e-34, {}, {});
  EXPECT_EQ(tiny.GetValueAsText(4, false), std::string("0.0000"));
  EXPECT_EQ(tiny.GetValueAsText(4, true), std::string("4.4647e-34"));

  const LimitedFloat<float32> num32(1.5F, {}, {});
  EXPECT_EQ(num32.GetValueAsText(4, false), std::string("1.5000"));
  EXPECT_EQ(num32.GetValueAsText(4, true), std::string("1.5e+00"));
}

TEST_F(LimitedFloatTests, ParseText)
{
  const LimitedFloat<float64> num(42.0, {-0.1}, {1e+7});

  // not a number
  EXPECT_FALSE(num.ParseText("").has_value());
  EXPECT_FALSE(num.ParseText("abc").has_value());
  EXPECT_FALSE(num.ParseText("1,2").has_value());
  EXPECT_FALSE(num.ParseText("100.000.000.2").has_value());
  EXPECT_FALSE(num.ParseText("--0.1").has_value());
  EXPECT_FALSE(num.ParseText("1e+2345").has_value());

  // out of bounds
  EXPECT_FALSE(num.ParseText("-0.2").has_value());
  EXPECT_FALSE(num.ParseText("1e+8").has_value());

  // fixed and exponential notation
  EXPECT_EQ(num.ParseText("0.123"), variant_t{0.123});
  EXPECT_EQ(num.ParseText("-.1e-12"), variant_t{-1e-13});
  EXPECT_EQ(num.ParseText("1.1e+6"), variant_t{1.1e+6});
  EXPECT_EQ(num.ParseText("0.123456789"), variant_t{0.123456789});

  // the value is not changed
  EXPECT_EQ(num.GetValue(), 42.0);
}

TEST_F(LimitedFloatTests, ParseTextFloat32)
{
  const LimitedFloat<float32> num(42.0F, {}, {});

  EXPECT_EQ(num.ParseText("1.5"), variant_t{1.5F});
  EXPECT_EQ(num.ParseText("3e+38"), variant_t{3e+38F});

  // outside of float32 range
  EXPECT_FALSE(num.ParseText("1e+39").has_value());
  EXPECT_FALSE(num.ParseText("-1e+39").has_value());
}

TEST_F(LimitedFloatTests, SetValueFromText)
{
  LimitedFloat<float64> num(42.0, {}, {100.0});
  EXPECT_FALSE(num.SetValueFromText("42.0"));  // same value
  EXPECT_TRUE(num.SetValueFromText("4.4647e-34"));
  EXPECT_EQ(num.GetValue(), 4.4647e-34);
  EXPECT_FALSE(num.SetValueFromText("abc"));
  EXPECT_FALSE(num.SetValueFromText("101"));
  EXPECT_FALSE(num.SetValueFromText(""));
  EXPECT_EQ(num.GetValue(), 4.4647e-34);
}

TEST_F(LimitedFloatTests, StepBy)
{
  LimitedFloat<float64> num(1.0, {0.0}, {2.0});
  num.SetSingleStep(0.5);
  EXPECT_EQ(num.GetSingleStep(), 0.5);

  EXPECT_FALSE(num.StepBy(0));

  EXPECT_TRUE(num.StepBy(1));
  EXPECT_EQ(num.GetValue(), 1.5);
  EXPECT_FALSE(num.IsAtMaximum());

  // forced into bounds
  EXPECT_TRUE(num.StepBy(3));
  EXPECT_EQ(num.GetValue(), 2.0);
  EXPECT_TRUE(num.IsAtMaximum());
  EXPECT_FALSE(num.StepBy(1));

  EXPECT_TRUE(num.StepBy(-10));
  EXPECT_EQ(num.GetValue(), 0.0);
  EXPECT_TRUE(num.IsAtMinimum());
  EXPECT_FALSE(num.StepBy(-1));
}

TEST_F(LimitedFloatTests, StepByAtTypeLimits)
{
  {  // no overflow for float64
    LimitedFloat<float64> num(std::numeric_limits<float64>::max(), {}, {});
    num.SetSingleStep(std::numeric_limits<float64>::max());
    EXPECT_FALSE(num.StepBy(1));
    EXPECT_TRUE(num.IsAtMaximum());
  }

  {  // no overflow for float32
    LimitedFloat<float32> num(0.0F, {}, {});
    num.SetSingleStep(1e+39);
    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), std::numeric_limits<float32>::max());
    EXPECT_TRUE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), std::numeric_limits<float32>::lowest());
  }
}

TEST_F(LimitedFloatTests, NullableConstructor)
{
  {  // without value
    const LimitedFloat<float64> num(std::nullopt, 42.0, {}, {});
    EXPECT_TRUE(num.IsNullable());
    EXPECT_FALSE(num.HasValue());
    EXPECT_FALSE(num.GetValue().has_value());
    EXPECT_EQ(num.GetDefaultValue(), 42.0);
    EXPECT_FALSE(utils::IsValid(num.GetValueAsVariant()));
    EXPECT_TRUE(num.GetValueAsText(4, false).empty());
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
  }

  {  // value and default value forced into bounds
    const LimitedFloat<float64> num(1.0, 42.0, 10.0, 20.0);
    EXPECT_EQ(num.GetValue(), 10.0);
    EXPECT_EQ(num.GetDefaultValue(), 20.0);
  }
}

TEST_F(LimitedFloatTests, NullableResetValue)
{
  LimitedFloat<float64> num(1.0, 42.0, {}, {});
  EXPECT_TRUE(num.ResetValue());
  EXPECT_FALSE(num.HasValue());
  EXPECT_FALSE(num.ResetValue());

  LimitedFloat<float64> non_nullable(1.0, {}, {});
  EXPECT_FALSE(non_nullable.ResetValue());
  EXPECT_TRUE(non_nullable.HasValue());
}

TEST_F(LimitedFloatTests, NullableSetValueFromVariant)
{
  LimitedFloat<float64> num(1.0, 42.0, {}, {});
  EXPECT_TRUE(num.SetValueFromVariant(variant_t{}));
  EXPECT_FALSE(num.HasValue());
  EXPECT_TRUE(num.SetValueFromVariant(variant_t{2.0}));
  EXPECT_EQ(num.GetValue(), 2.0);
}

TEST_F(LimitedFloatTests, NullableSetValueFromText)
{
  LimitedFloat<float64> num(1.0, 42.0, {}, {});

  EXPECT_TRUE(num.ParseText("").has_value());
  EXPECT_FALSE(utils::IsValid(num.ParseText("").value()));

  EXPECT_TRUE(num.SetValueFromText(""));
  EXPECT_FALSE(num.HasValue());
  EXPECT_FALSE(num.SetValueFromText(""));

  EXPECT_TRUE(num.SetValueFromText("2.5"));
  EXPECT_EQ(num.GetValue(), 2.5);
}

TEST_F(LimitedFloatTests, NullableStepBy)
{
  LimitedFloat<float64> num(std::nullopt, 42.0, {}, {});
  EXPECT_FALSE(num.StepBy(0));
  EXPECT_FALSE(num.HasValue());

  // the first step sets the default value, in both directions
  EXPECT_TRUE(num.StepBy(-5));
  EXPECT_EQ(num.GetValue(), 42.0);

  EXPECT_TRUE(num.StepBy(1));
  EXPECT_EQ(num.GetValue(), 43.0);
}

}  // namespace mvvm::test
