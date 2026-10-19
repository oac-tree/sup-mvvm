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

#include "mvvm/utils/limited_integer.h"

#include <mvvm/core/mvvm_exceptions.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for LimitedInteger class.
 */
class LimitedIntegerTests : public ::testing::Test
{
};

TEST_F(LimitedIntegerTests, Constructor)
{
  {  // number without bounds defined
    const LimitedInteger<int> num(42, {}, {});
    EXPECT_EQ(num.GetValue(), 42);
    EXPECT_EQ(num.GetLowerBound(), std::numeric_limits<int>::min());
    EXPECT_EQ(num.GetUpperBound(), std::numeric_limits<int>::max());
  }

  {  // number with bounds
    const LimitedInteger<int> num(42, 1, 45);
    EXPECT_EQ(num.GetValue(), 42);
    EXPECT_EQ(num.GetLowerBound(), 1);
    EXPECT_EQ(num.GetUpperBound(), 45);
  }

  {  // attempt to mix types
    EXPECT_THROW(LimitedInteger<int>(42.1, {}, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger<int>(42, 42.1, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger<int>(42, 10, 64U), RuntimeException);
  }

  {  // attempt to ser upper limit smaller than lower limit
    EXPECT_THROW(LimitedInteger<int>(42, 10, 5), RuntimeException);
  }
}

TEST_F(LimitedIntegerTests, ConstructorWithValueShift)
{
  {  // value forced to upper limit
    const LimitedInteger<int> num(42, 10, 20);
    EXPECT_EQ(num.GetValue(), 20);
    EXPECT_EQ(num.GetLowerBound(), 10);
    EXPECT_EQ(num.GetUpperBound(), 20);
  }

  {  // value forced to lower limit
    const LimitedInteger<int> num(1, 10, 20);
    EXPECT_EQ(num.GetValue(), 10);
    EXPECT_EQ(num.GetLowerBound(), 10);
    EXPECT_EQ(num.GetUpperBound(), 20);
  }
}

TEST_F(LimitedIntegerTests, SetValue)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_FALSE(num.SetValue(42));  // same value
    EXPECT_TRUE(num.SetValue(43));
    EXPECT_EQ(num.GetValue(), 43);
  }

  {  // number with  upper limit
    LimitedInteger<int> num(42, {}, {50});
    EXPECT_TRUE(num.SetValue(49));
    EXPECT_TRUE(num.SetValue(50));
    EXPECT_FALSE(num.SetValue(51));
    EXPECT_EQ(num.GetValue(), 50);
  }

  {  // number with lower limit
    LimitedInteger<int> num(42, {40}, {});
    EXPECT_TRUE(num.SetValue(41));
    EXPECT_TRUE(num.SetValue(40));
    EXPECT_FALSE(num.SetValue(39));
    EXPECT_EQ(num.GetValue(), 40);
  }

  {  // number with upper and lower limits
    LimitedInteger<int> num(42, {40}, {50});
    EXPECT_TRUE(num.SetValue(40));
    EXPECT_TRUE(num.SetValue(50));
    EXPECT_FALSE(num.SetValue(39));
    EXPECT_FALSE(num.SetValue(51));
    EXPECT_EQ(num.GetValue(), 50);
  }
}

TEST_F(LimitedIntegerTests, SetValueFromVariant)
{
  LimitedInteger<int> num(42, {}, {});
  EXPECT_FALSE(num.SetValueFromVariant(variant_t{42}));  // same value
  EXPECT_TRUE(num.SetValueFromVariant(variant_t{43}));
  EXPECT_EQ(num.GetValue(), 43);

  EXPECT_EQ(num.GetValueAsVariant(), variant_t{43});
  EXPECT_EQ(num.GetLowerBoundAsVariant(), variant_t{std::numeric_limits<int>::min()});
  EXPECT_EQ(num.GetUpperBoundAsVariant(), variant_t{std::numeric_limits<int>::max()});
}

TEST_F(LimitedIntegerTests, Increment)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
    EXPECT_TRUE(num.Increment());
    EXPECT_EQ(num.GetValue(), 43);
  }

  {  // number with limits
    LimitedInteger<int> num(49, 40, 50);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Increment());

    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_TRUE(num.IsAtMaximum());

    EXPECT_FALSE(num.Increment());

    EXPECT_EQ(num.GetValue(), 50);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_TRUE(num.IsAtMaximum());
  }

  {  // uint8 without limits
    uint8 initial_value(254);
    LimitedInteger<uint8> num(initial_value, {}, {});

    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Increment());
    EXPECT_FALSE(num.Increment());

    EXPECT_EQ(num.GetValue(), 255);

    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_TRUE(num.IsAtMaximum());
  }
}

TEST_F(LimitedIntegerTests, Decrement)
{
  {  // number without limits
    LimitedInteger<int> num(42, {}, {});
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Decrement());

    EXPECT_EQ(num.GetValue(), 41);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
  }

  {  // number with limits
    LimitedInteger<int> num(41, 40, 50);
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_TRUE(num.Decrement());
    EXPECT_TRUE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());

    EXPECT_FALSE(num.Decrement());

    EXPECT_EQ(num.GetValue(), 40);
    EXPECT_TRUE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
  }

  {  // uint8 without limits
    uint8 initial_value(1);
    LimitedInteger<uint8> num(initial_value, {}, {});
    EXPECT_TRUE(num.Decrement());
    EXPECT_FALSE(num.Decrement());
    EXPECT_EQ(num.GetValue(), 0);
  }
}

TEST_F(LimitedIntegerTests, SetValueFromText)
{
  LimitedInteger<int> num(42, {}, {});

  EXPECT_TRUE(num.SetValueFromText("43"));
  EXPECT_EQ(num.GetValue(), 43);
  EXPECT_EQ(num.GetValueAsText(), std::string("43"));

  EXPECT_FALSE(num.SetValueFromText("43"));  // // same value

  // Our current implementation allows such conversions. We rely on validators on board of cell
  // editors.
  EXPECT_TRUE(num.SetValueFromText("44.1"));
  EXPECT_EQ(num.GetValue(), 44);

  EXPECT_FALSE(num.SetValueFromText("aaaa"));
}

TEST_F(LimitedIntegerTests, StepBy)
{
  {  // positive ulimited integer
    LimitedInteger<int> num(42, {}, {});

    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 43);

    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 42);
  }

  {  // negative ulimited integer
    LimitedInteger<int> num(-42, {}, {});

    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), -41);

    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), -42);
  }

  {  // limited integer, several incrementing steps
    LimitedInteger<int> num(42, 40, 50);

    EXPECT_FALSE(num.StepBy(0));
    EXPECT_EQ(num.GetValue(), 42);

    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 43);

    EXPECT_TRUE(num.StepBy(2));
    EXPECT_EQ(num.GetValue(), 45);

    EXPECT_TRUE(num.StepBy(5));
    EXPECT_EQ(num.GetValue(), 50);

    EXPECT_FALSE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 50);
  }

  {  // limited integer, several decrementing steps
    LimitedInteger<int> num(45, 40, 50);

    EXPECT_FALSE(num.StepBy(0));
    EXPECT_EQ(num.GetValue(), 45);

    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 44);

    EXPECT_TRUE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), 42);

    EXPECT_TRUE(num.StepBy(-5));
    EXPECT_EQ(num.GetValue(), 40);  // stopped at minimum border

    EXPECT_FALSE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 40);
  }

  {  // unlimited int8, several steps
    int8 initial_value{0};
    LimitedInteger<int8> num(initial_value, {}, {});

    EXPECT_TRUE(num.StepBy(300));
    EXPECT_EQ(num.GetValue(), 127);
    EXPECT_FALSE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 127);

    EXPECT_TRUE(num.StepBy(-300));
    EXPECT_EQ(num.GetValue(), -128);
    EXPECT_FALSE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), -128);
  }

  {  // unlimited uint8, several steps
    uint8 initial_value{0};
    LimitedInteger<uint8> num(initial_value, {}, {});

    EXPECT_TRUE(num.StepBy(2));
    EXPECT_EQ(num.GetValue(), static_cast<mvvm::uint8>(2));
    EXPECT_TRUE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), static_cast<mvvm::uint8>(0));
    EXPECT_FALSE(num.StepBy(-2));
    EXPECT_EQ(num.GetValue(), static_cast<mvvm::uint8>(0));
  }
}

TEST_F(LimitedIntegerTests, NonNullable)
{
  LimitedInteger<int> num(42, {}, {});
  EXPECT_FALSE(num.IsNullable());
  EXPECT_TRUE(num.HasValue());

  // value can't be removed
  EXPECT_FALSE(num.ResetValue());
  EXPECT_FALSE(num.SetValueFromText(""));
  EXPECT_FALSE(num.SetValueFromVariant(variant_t{}));
  EXPECT_EQ(num.GetValue(), 42);
}

TEST_F(LimitedIntegerTests, NullableConstructor)
{
  {  // without value
    const LimitedInteger<int> num({}, 42, {}, {});
    EXPECT_TRUE(num.IsNullable());
    EXPECT_FALSE(num.HasValue());
    EXPECT_FALSE(num.GetValue().has_value());
    EXPECT_EQ(num.GetDefaultValue(), 42);
    EXPECT_EQ(num.GetValueAsVariant(), variant_t{});
    EXPECT_EQ(num.GetValueAsText(), std::string());
    EXPECT_FALSE(num.IsAtMinimum());
    EXPECT_FALSE(num.IsAtMaximum());
  }

  {  // with value
    const LimitedInteger<int> num(10, 42, {}, {});
    EXPECT_TRUE(num.HasValue());
    EXPECT_EQ(num.GetValue(), 10);
    EXPECT_EQ(num.GetDefaultValue(), 42);
  }

  {  // value and default value forced into bounds
    const LimitedInteger<int> num(1, 42, 10, 20);
    EXPECT_EQ(num.GetValue(), 10);
    EXPECT_EQ(num.GetDefaultValue(), 20);
  }

  {  // attempt to mix types, or to define no default value
    EXPECT_THROW(LimitedInteger<int>(42U, 42, {}, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger<int>({}, 42, 10U, {}), RuntimeException);
    EXPECT_THROW(LimitedInteger<int>({}, {}, {}, {}), RuntimeException);
  }
}

TEST_F(LimitedIntegerTests, NullableResetValue)
{
  LimitedInteger<int> num(42, 0, {}, {});

  EXPECT_TRUE(num.ResetValue());
  EXPECT_FALSE(num.HasValue());
  EXPECT_FALSE(num.ResetValue());  // no value already
  EXPECT_EQ(num.GetValueAsText(), std::string());

  EXPECT_TRUE(num.SetValue(42));
  EXPECT_EQ(num.GetValue(), 42);
  EXPECT_EQ(num.GetValueAsText(), std::string("42"));
}

TEST_F(LimitedIntegerTests, NullableSetValueFromVariant)
{
  LimitedInteger<int> num(42, 0, {}, {});

  EXPECT_TRUE(num.SetValueFromVariant(variant_t{}));
  EXPECT_FALSE(num.HasValue());
  EXPECT_FALSE(num.SetValueFromVariant(variant_t{}));  // no value already

  EXPECT_TRUE(num.SetValueFromVariant(variant_t{43}));
  EXPECT_EQ(num.GetValueAsVariant(), variant_t{43});
}

TEST_F(LimitedIntegerTests, NullableSetValueFromText)
{
  LimitedInteger<int> num(42, 0, {}, {});

  EXPECT_TRUE(num.SetValueFromText(""));
  EXPECT_FALSE(num.HasValue());
  EXPECT_EQ(num.GetValueAsText(), std::string());
  EXPECT_FALSE(num.SetValueFromText(""));  // no value already

  EXPECT_FALSE(num.SetValueFromText("aaaa"));
  EXPECT_FALSE(num.HasValue());

  EXPECT_TRUE(num.SetValueFromText("43"));
  EXPECT_EQ(num.GetValueAsText(), std::string("43"));
}

TEST_F(LimitedIntegerTests, NullableStepBy)
{
  {  // zero steps keep no value
    LimitedInteger<int> num({}, 42, {}, {});
    EXPECT_FALSE(num.StepBy(0));
    EXPECT_FALSE(num.HasValue());
  }

  {  // first step up sets default value
    LimitedInteger<int> num({}, 42, {}, {});
    EXPECT_TRUE(num.StepBy(1));
    EXPECT_EQ(num.GetValue(), 42);
  }

  {  // first step down sets default value
    LimitedInteger<int> num({}, 42, {}, {});
    EXPECT_TRUE(num.StepBy(-1));
    EXPECT_EQ(num.GetValue(), 42);
  }

  {  // several steps
    LimitedInteger<int> num({}, 42, {}, {});
    EXPECT_TRUE(num.StepBy(3));
    EXPECT_EQ(num.GetValue(), 44);

    EXPECT_TRUE(num.ResetValue());
    EXPECT_TRUE(num.StepBy(-3));
    EXPECT_EQ(num.GetValue(), 40);
  }

  {  // default value at upper bound
    LimitedInteger<int> num({}, 50, 40, 50);
    EXPECT_TRUE(num.Increment());
    EXPECT_EQ(num.GetValue(), 50);
    EXPECT_TRUE(num.IsAtMaximum());
    EXPECT_FALSE(num.Increment());
  }

  {  // uint8 text after the first step
    LimitedInteger<uint8> num({}, uint8{200}, {}, {});
    EXPECT_TRUE(num.Decrement());
    EXPECT_EQ(num.GetValueAsText(), std::string("200"));
  }
}

}  // namespace mvvm::test
